/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DOMStorageDBUpdater_h___
#define DOMStorageDBUpdater_h___

namespace mozilla {
namespace dom {

namespace DOMStorageDBUpdater {

nsresult Update(mozIStorageConnection *aWorkerConnection);

} // DOMStorageDBUpdater

} // dom
} // mozilla

#endif
