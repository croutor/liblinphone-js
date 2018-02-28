#include <napi.h>
#include "linphone/linphonecore.h"


Napi::String Hello(const Napi::CallbackInfo& info) {
  return Napi::String::New(info.Env(), "world");
}

void CoreEnableLog() {
  linphone_core_enable_logs(NULL);
}

Napi::Object CoreNew(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  lc;
  LinphoneCoreVTable vtable={0};

  lc=linphone_core_new(&vtable,"/tmp/linphone.conf",NULL,NULL);
  Napi::Object val = Napi::ObjectWrap<LinphoneCore *>(lc)
  return val;
}

Napi::Value CoreDestroy(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // expect one argument
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsValue()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  linphone_core_destroy(info[0].Value());

  return env.Null();
}

#if 0
/*
/*
 Fill the LinphoneCoreVTable with application callbacks.
 All are optional. Here we only use the call_state_changed callbacks
 in order to get notifications about the progress of the call.
 */
vtable.call_state_changed=call_state_changed;
/*
 Fill the LinphoneCoreVTable with application callbacks.
 All are optional. Here we only use the registration_state_changed callbacks
 in order to get notifications about the progress of the registration.
 */
vtable.registration_state_changed=registration_state_changed;
/*
 Instanciate a LinphoneCore object given the LinphoneCoreVTable
*/
lc=linphone_core_new(&vtable,"/tmp/linphone.conf",NULL,NULL);
...

linphone_core_destroy(lc);
#endif

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("hello", Napi::Function::New(env, Hello));
  exports.Set("core_enable_log", Napi::Function::New(env, CoreEnableLog));
  exports.Set("core_new", Napi::Function::New(env, CoreNew));
  exports.Set("core_destroy", Napi::Function::New(env, CoreDestroy));
  return exports;
}

NODE_API_MODULE(addon, Init)
