// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "electron/browser/ui/win/message_handler_delegate.h"

namespace electron {

bool MessageHandlerDelegate::PreHandleMSG(
    UINT message, WPARAM w_param, LPARAM l_param, LRESULT* result) {
  return false;
}

}  // namespace electron
