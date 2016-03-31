// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ELECTRON_BROWSER_UI_TRAY_ICON_OBSERVER_H_
#define ELECTRON_BROWSER_UI_TRAY_ICON_OBSERVER_H_

#include <string>
#include <vector>

namespace gfx {
class Rect;
}

namespace electron {

class TrayIconObserver {
 public:
  virtual void OnClicked(const gfx::Rect& bounds, int modifiers) {}
  virtual void OnDoubleClicked(const gfx::Rect& bounds, int modifiers) {}
  virtual void OnBalloonShow() {}
  virtual void OnBalloonClicked() {}
  virtual void OnBalloonClosed() {}
  virtual void OnRightClicked(const gfx::Rect& bounds, int modifiers) {}
  virtual void OnDrop() {}
  virtual void OnDropFiles(const std::vector<std::string>& files) {}
  virtual void OnDragEntered() {}
  virtual void OnDragExited() {}
  virtual void OnDragEnded() {}

 protected:
  virtual ~TrayIconObserver() {}
};

}  // namespace electron

#endif  // ELECTRON_BROWSER_UI_TRAY_ICON_OBSERVER_H_
