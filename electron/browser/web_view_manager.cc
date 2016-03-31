// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "electron/browser/web_view_manager.h"

#include "electron/browser/atom_browser_context.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/web_contents.h"

namespace electron {

WebViewManager::WebViewManager() {
}

WebViewManager::~WebViewManager() {
}

void WebViewManager::AddGuest(int guest_instance_id,
                              int element_instance_id,
                              content::WebContents* embedder,
                              content::WebContents* web_contents) {
  web_contents_embedder_map_[guest_instance_id] = { web_contents, embedder };

  // Map the element in embedder to guest.
  int owner_process_id = embedder->GetRenderProcessHost()->GetID();
  ElementInstanceKey key(owner_process_id, element_instance_id);
  element_instance_id_to_guest_map_[key] = guest_instance_id;
}

void WebViewManager::RemoveGuest(int guest_instance_id) {
  if (!ContainsKey(web_contents_embedder_map_, guest_instance_id))
    return;

  web_contents_embedder_map_.erase(guest_instance_id);

  // Remove the record of element in embedder too.
  for (const auto& element : element_instance_id_to_guest_map_)
    if (element.second == guest_instance_id) {
      element_instance_id_to_guest_map_.erase(element.first);
      break;
    }
}

content::WebContents* WebViewManager::GetGuestByInstanceID(
    int owner_process_id,
    int element_instance_id) {
  ElementInstanceKey key(owner_process_id, element_instance_id);
  if (!ContainsKey(element_instance_id_to_guest_map_, key))
    return nullptr;

  int guest_instance_id = element_instance_id_to_guest_map_[key];
  if (ContainsKey(web_contents_embedder_map_, guest_instance_id))
    return web_contents_embedder_map_[guest_instance_id].web_contents;
  else
    return nullptr;
}

bool WebViewManager::ForEachGuest(content::WebContents* embedder_web_contents,
                                  const GuestCallback& callback) {
  for (auto& item : web_contents_embedder_map_)
    if (item.second.embedder == embedder_web_contents &&
        callback.Run(item.second.web_contents))
      return true;
  return false;
}

}  // namespace electron
