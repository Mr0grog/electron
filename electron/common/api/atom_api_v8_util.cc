// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include <string>

#include "electron/common/api/object_life_monitor.h"
#include "electron/common/node_includes.h"
#include "native_mate/dictionary.h"
#include "v8/include/v8-profiler.h"

namespace {

v8::Local<v8::Value> GetHiddenValue(v8::Isolate* isolate,
                                    v8::Local<v8::Object> object,
                                    v8::Local<v8::String> key) {
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Private> privateKey = v8::Private::ForApi(isolate, key);
  v8::Local<v8::Value> value;
  v8::Maybe<bool> result = object->HasPrivate(context, privateKey);
  if (!(result.IsJust() && result.FromJust()))
    return v8::Local<v8::Value>();
  if (object->GetPrivate(context, privateKey).ToLocal(&value))
    return value;
  return v8::Local<v8::Value>();
}

void SetHiddenValue(v8::Isolate* isolate,
                    v8::Local<v8::Object> object,
                    v8::Local<v8::String> key,
                    v8::Local<v8::Value> value) {
  if (value.IsEmpty())
    return;
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Private> privateKey = v8::Private::ForApi(isolate, key);
  object->SetPrivate(context, privateKey, value);
}

void DeleteHiddenValue(v8::Isolate* isolate,
                       v8::Local<v8::Object> object,
                       v8::Local<v8::String> key) {
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Private> privateKey = v8::Private::ForApi(isolate, key);
  // Actually deleting the value would make force the object into
  // dictionary mode which is unnecessarily slow. Instead, we replace
  // the hidden value with "undefined".
  object->SetPrivate(context, privateKey, v8::Undefined(isolate));
}

int32_t GetObjectHash(v8::Local<v8::Object> object) {
  return object->GetIdentityHash();
}

void SetDestructor(v8::Isolate* isolate,
                   v8::Local<v8::Object> object,
                   v8::Local<v8::Function> callback) {
  electron::ObjectLifeMonitor::BindTo(isolate, object, callback);
}

void TakeHeapSnapshot(v8::Isolate* isolate) {
  isolate->GetHeapProfiler()->TakeHeapSnapshot();
}

void Initialize(v8::Local<v8::Object> exports, v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context, void* priv) {
  mate::Dictionary dict(context->GetIsolate(), exports);
  dict.SetMethod("getHiddenValue", &GetHiddenValue);
  dict.SetMethod("setHiddenValue", &SetHiddenValue);
  dict.SetMethod("deleteHiddenValue", &DeleteHiddenValue);
  dict.SetMethod("getObjectHash", &GetObjectHash);
  dict.SetMethod("setDestructor", &SetDestructor);
  dict.SetMethod("takeHeapSnapshot", &TakeHeapSnapshot);
}

}  // namespace

NODE_MODULE_CONTEXT_AWARE_BUILTIN(atom_common_v8_util, Initialize)
