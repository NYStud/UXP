/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DocumentStyleRootIterator.h"

#include "nsContentUtils.h"

namespace mozilla {

DocumentStyleRootIterator::DocumentStyleRootIterator(nsIDocument* aDocument)
  : mPosition(0)
{
  MOZ_COUNT_CTOR(DocumentStyleRootIterator);
  if (Element* root = aDocument->GetRootElement()) {
    mStyleRoots.AppendElement(root);
  }
  nsContentUtils::AppendDocumentLevelNativeAnonymousContentTo(
      aDocument, mStyleRoots);
}

Element*
DocumentStyleRootIterator::GetNextStyleRoot()
{
  for (;;) {
    if (mPosition >= mStyleRoots.Length()) {
      return nullptr;
    }

    nsIContent* next = mStyleRoots[mPosition];
    ++mPosition;

    if (next->IsElement()) {
      return next->AsElement();
    }
  }
}

} // namespace mozilla
