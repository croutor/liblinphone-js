#include "linphone.h"

Napi::FunctionReference Linphone::constructor;

Napi::Object Linphone::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Linphone", {
    InstanceMethod("enablelog", &Linphone::EnableLog),
    InstanceMethod("plusOne", &Linphone::PlusOne),
    InstanceMethod("value", &Linphone::GetValue),
    InstanceMethod("multiply", &Linphone::Multiply)
  });

  constructor = Napi::Persistent(func);
  //constructor.SuppressDestruct();

  exports.Set("Linphone", func);
  return exports;
}

Linphone::Linphone(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Linphone>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
  }

  std::string config = info[0].As<Napi::String>().Utf8Value();
  //this->value_ = value.String();
   /*
   Fill the LinphoneCoreVTable with application callbacks.
   All are optional. Here we only use the call_state_changed callbacks
   in order to get notifications about the progress of the call.
   */
  this->vtable_.call_state_changed=Linphone::CallStateChanged;
  /*
   Fill the LinphoneCoreVTable with application callbacks.
   All are optional. Here we only use the registration_state_changed callbacks
   in order to get notifications about the progress of the registration.
   */
  this->vtable_.registration_state_changed=Linphone::RegistrationStateChanged;
  /*
   Instanciate a LinphoneCore object given the LinphoneCoreVTable
  */
  this->lc_=linphone_core_new(&this->vtable_,config.c_str(),NULL,NULL);
}

Linphone::~Linphone() {
  linphone_core_destroy(this->lc_);
}

void Linphone::EnableLog(const Napi::CallbackInfo& info) {
  linphone_core_enable_logs(NULL);
}

Napi::Value Linphone::GetValue(const Napi::CallbackInfo& info) {
  double num = this->value_;

  return Napi::Number::New(info.Env(), num);
}

Napi::Value Linphone::PlusOne(const Napi::CallbackInfo& info) {
  this->value_ = this->value_ + 1;

  return Linphone::GetValue(info);
}

Napi::Value Linphone::Multiply(const Napi::CallbackInfo& info) {
  Napi::Number multiple;
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    multiple = Napi::Number::New(info.Env(), 1);
  } else {
    multiple = info[0].As<Napi::Number>();
  }

  Napi::Object obj = constructor.New({ Napi::Number::New(info.Env(), this->value_ * multiple.DoubleValue()) });

  return obj;
}

/*
 * Call state notification callback
 */
void Linphone::CallStateChanged(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg)
{
        switch(cstate){
                case LinphoneCallOutgoingInit:
                        printf("Outgoing call init !\n");
                break;
                case LinphoneCallOutgoingProgress:
                        printf("Outgoing call progress !\n");
                break;
                case LinphoneCallOutgoingRinging:
                        printf("It is now ringing remotely !\n");
                break;
                case LinphoneCallOutgoingEarlyMedia:
                        printf("Receiving some early media\n");
                break;
                case LinphoneCallConnected:
                        printf("We are connected !\n");
                break;
                case LinphoneCallStreamsRunning:
                        printf("Media streams established !\n");
                break;
                case LinphoneCallEnd:
                        printf("Call is terminated.\n");
                break;
                case LinphoneCallPausedByRemote:
                        printf("Call paused by remote\n");
                break;
                case LinphoneCallUpdatedByRemote:
                        printf("Call updated by remote\n");
                break;
                case LinphoneCallIncomingEarlyMedia:
                        printf("We are proposing early media to an incoming call\n");
                break;
                case LinphoneCallUpdating:
                        printf("A call update has been initiated by us\n");
                break;
                case LinphoneCallReleased:
                        printf("The call object is no more retained by the core\n");
                break;
                case LinphoneCallError:
                        printf("Call failure !");
                break;
                default:
                        printf("Unhandled notification %i\n",cstate);
        }
}

void Linphone::RegistrationStateChanged(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message)
{
        printf("New registration state %s for user id [%s] at proxy [%s]\n"
                        ,linphone_registration_state_to_string(cstate)
                        ,linphone_proxy_config_get_identity(cfg)
                        ,linphone_proxy_config_get_addr(cfg));
}
