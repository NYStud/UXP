/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_mips64_LOpcodes_mips64_h__
#define jit_mips64_LOpcodes_mips64_h__

#include "jit/shared/LOpcodes-shared.h"

#define LIR_CPU_OPCODE_LIST(_)  \
    _(ModMaskI)                 \
    _(DivOrModI64)              \
    _(UDivOrMod)                \
    _(UDivOrModI64)             \
    _(WasmUnalignedLoad)        \
    _(WasmUnalignedStore)       \
    _(WasmUnalignedLoadI64)     \
    _(WasmUnalignedStoreI64)    \
    _(WasmTruncateToInt64)      \
    _(Int64ToFloatingPoint)

#endif // jit_mips64_LOpcodes_mips64_h__
