#ifndef LINPHONE_H
#define LINPHONE_H

#include <napi.h>
#include "linphone/linphonecore.h"


class Linphone : public Napi::ObjectWrap<Linphone> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Linphone(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  void EnableLog(const Napi::CallbackInfo& info);
  /**
     @brief initialize internal library and register linphone client
     @param info : with at least 3 strings:
     - 1st string: the configuration file path
     - 2nd string : identity
     - 3rd string: password
   */
  void Register(const Napi::CallbackInfo& info);
  /**
    @brief de-register linphone client and de-init internal library.
   */
  void Unegister(const Napi::CallbackInfo& info);
  /**
   @brief initialize internal liblinphone library
   @param config : string the configuration file path
   */
  void Init(const std::string& config);
  /**
   @brief deinitialize internal liblinphone library
   */
  void DeInit();
  Napi::Value GetValue(const Napi::CallbackInfo& info);
  Napi::Value PlusOne(const Napi::CallbackInfo& info);
  Napi::Value Multiply(const Napi::CallbackInfo& info);
  static void CallStateChanged(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg);
  static void RegistrationStateChanged(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message);

  double value_;
  LinphoneCoreVTable vtable_;
  LinphoneCore *lc_;

};

#endif
