/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/HoldDropJSObjects.h"

#include "mozilla/Assertions.h"
#include "mozilla/CycleCollectedJSContext.h"

namespace mozilla {
namespace cyclecollector {

void
HoldJSObjectsImpl(void* aHolder, nsScriptObjectTracer* aTracer)
{
  CycleCollectedJSContext* cx = CycleCollectedJSContext::Get();
  MOZ_ASSERT(cx, "Should have a CycleCollectedJSContext by now");
  cx->AddJSHolder(aHolder, aTracer);
}

void
HoldJSObjectsImpl(nsISupports* aHolder)
{
  nsXPCOMCycleCollectionParticipant* participant = nullptr;
  CallQueryInterface(aHolder, &participant);
  MOZ_ASSERT(participant, "Failed to QI to nsXPCOMCycleCollectionParticipant!");
  MOZ_ASSERT(participant->CheckForRightISupports(aHolder),
             "The result of QIing a JS holder should be the same as ToSupports");

  HoldJSObjectsImpl(aHolder, participant);
}

void
DropJSObjectsImpl(void* aHolder)
{
  CycleCollectedJSContext* cx = CycleCollectedJSContext::Get();
  MOZ_ASSERT(cx, "Should have a CycleCollectedJSContext by now");
  cx->RemoveJSHolder(aHolder);
}

void
DropJSObjectsImpl(nsISupports* aHolder)
{
#ifdef DEBUG
  nsXPCOMCycleCollectionParticipant* participant = nullptr;
  CallQueryInterface(aHolder, &participant);
  MOZ_ASSERT(participant, "Failed to QI to nsXPCOMCycleCollectionParticipant!");
  MOZ_ASSERT(participant->CheckForRightISupports(aHolder),
             "The result of QIing a JS holder should be the same as ToSupports");
#endif
  DropJSObjectsImpl(static_cast<void*>(aHolder));
}

} // namespace cyclecollector

#ifdef DEBUG
bool
IsJSHolder(void* aHolder)
{
  CycleCollectedJSContext* cx = CycleCollectedJSContext::Get();
  MOZ_ASSERT(cx, "Should have a CycleCollectedJSContext by now");
  return cx->IsJSHolder(aHolder);
}
#endif

} // namespace mozilla
