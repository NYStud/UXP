/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "jit/WasmBCE.h"
#include "jit/MIRGenerator.h"
#include "jit/MIRGraph.h"

using namespace js;
using namespace js::jit;
using namespace mozilla;

typedef js::HashMap<uint32_t, MDefinition*, DefaultHasher<uint32_t>, SystemAllocPolicy>
    LastSeenMap;

// The Wasm Bounds Check Elimination (BCE) pass looks for bounds checks
// on SSA values that have already been checked. (in the same block or in a
// dominating block). These bounds checks are redundant and thus eliminated.
//
// Note: This is safe in the presense of dynamic memory sizes as long as they
// can ONLY GROW. If we allow SHRINKING the heap, this pass should be
// RECONSIDERED.
//
// TODO (dbounov): Are there a lot of cases where there is no single dominating
// check, but a set of checks that together dominate a redundant check?
//
// TODO (dbounov): Generalize to constant additions relative to one base
bool
jit::EliminateBoundsChecks(MIRGenerator* mir, MIRGraph& graph)
{
    // Map for dominating block where a given definition was checked
    LastSeenMap lastSeen;
    if (!lastSeen.init())
        return false;

    for (ReversePostorderIterator bIter(graph.rpoBegin()); bIter != graph.rpoEnd(); bIter++) {
        MBasicBlock* block = *bIter;
        for (MDefinitionIterator dIter(block); dIter;) {
            MDefinition* def = *dIter++;

            switch (def->op()) {
              case MDefinition::Op_WasmBoundsCheck: {
                MWasmBoundsCheck* bc = def->toWasmBoundsCheck();
                MDefinition* addr = def->getOperand(0);

                LastSeenMap::AddPtr ptr = lastSeen.lookupForAdd(addr->id());
                if (ptr) {
                    if (ptr->value()->block()->dominates(block))
                        bc->setRedundant(true);
                } else {
                    if (!lastSeen.add(ptr, addr->id(), def))
                        return false;
                }
                break;
              }
              case MDefinition::Op_Phi: {
                MPhi* phi = def->toPhi();
                bool phiChecked = true;

                MOZ_ASSERT(phi->numOperands() > 0);

                // If all incoming values to a phi node are safe (i.e. have a
                // check that dominates this block) then we can consider this
                // phi node checked.
                //
                // Note that any phi that is part of a cycle
                // will not be "safe" since the value coming on the backedge
                // cannot be in lastSeen because its block hasn't been traversed yet.
                for (int i = 0, nOps = phi->numOperands(); i < nOps; i++) {
                    MDefinition* src = phi->getOperand(i);

                    LastSeenMap::Ptr checkPtr = lastSeen.lookup(src->id());
                    if (!checkPtr || !checkPtr->value()->block()->dominates(block)) {
                        phiChecked = false;
                        break;
                    }
                }

                if (phiChecked) {
                    if (!lastSeen.put(def->id(), def))
                        return false;
                }

                break;
              }
              default:
                break;
            }
        }
    }

    return true;
}
