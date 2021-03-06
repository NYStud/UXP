/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_quota_SerializationHelpers_h
#define mozilla_dom_quota_SerializationHelpers_h

#include "ipc/IPCMessageUtils.h"

#include "mozilla/dom/quota/PersistenceType.h"

namespace IPC {

template <>
struct ParamTraits<mozilla::dom::quota::PersistenceType> :
  public ContiguousEnumSerializer<
                               mozilla::dom::quota::PersistenceType,
                               mozilla::dom::quota::PERSISTENCE_TYPE_PERSISTENT,
                               mozilla::dom::quota::PERSISTENCE_TYPE_INVALID>
{ };

} // namespace IPC

#endif // mozilla_dom_quota_SerializationHelpers_h
