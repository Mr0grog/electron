// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ELECTRON_BROWSER_API_ATOM_API_WEB_REQUEST_H_
#define ELECTRON_BROWSER_API_ATOM_API_WEB_REQUEST_H_

#include "electron/browser/api/trackable_object.h"
#include "electron/browser/net/atom_network_delegate.h"
#include "native_mate/arguments.h"
#include "native_mate/handle.h"

namespace electron {

class AtomBrowserContext;

namespace api {

class WebRequest : public mate::TrackableObject<WebRequest> {
 public:
  static mate::Handle<WebRequest> Create(v8::Isolate* isolate,
                                         AtomBrowserContext* browser_context);

  // mate::TrackableObject:
  static void BuildPrototype(v8::Isolate* isolate,
                             v8::Local<v8::ObjectTemplate> prototype);

 protected:
  explicit WebRequest(AtomBrowserContext* browser_context);
  ~WebRequest();

  // C++ can not distinguish overloaded member function.
  template<AtomNetworkDelegate::SimpleEvent type>
  void SetSimpleListener(mate::Arguments* args);
  template<AtomNetworkDelegate::ResponseEvent type>
  void SetResponseListener(mate::Arguments* args);
  template<typename Listener, typename Method, typename Event>
  void SetListener(Method method, Event type, mate::Arguments* args);

 private:
  scoped_refptr<AtomBrowserContext> browser_context_;

  DISALLOW_COPY_AND_ASSIGN(WebRequest);
};

}  // namespace api

}  // namespace electron

#endif  // ELECTRON_BROWSER_API_ATOM_API_WEB_REQUEST_H_
