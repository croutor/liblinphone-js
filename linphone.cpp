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
  constructor.SuppressDestruct();

  exports.Set("Linphone", func);
  return exports;
}

Linphone::Linphone(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Linphone>(info)  {
}

void Linphone::Register(const Napi::CallbackInfo& info) {
  LinphoneAddress *from;
  LinphoneAuthInfo *authinfo;
  LinphoneProxyConfig* proxy_cfg;
  const char* server_addr;

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  /* expect 3 strings as parameters:
     @param config : string the configuration file path
     @param identity : string
     @param password : string
   */
  if (length <= 2 || !info[0].IsString() ||
      !info[1].IsString() ||
      !info[2].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
  }
  std::string config = info[0].As<Napi::String>().Utf8Value();
  std::string identity = info[1].As<Napi::String>().Utf8Value();
  std::string password = info[2].As<Napi::String>().Utf8Value();
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

  /*create proxy config*/
  proxy_cfg = linphone_proxy_config_new();

  /*parse identity*/
  from = linphone_address_new(identity.c_str());
  if (from==NULL){
          printf("%s not a valid sip uri, must be like sip:toto@sip.linphone.org \n",identity.c_str());
          return;
  }
  if (password.length()!=0){
          authinfo=linphone_auth_info_new(linphone_address_get_username(from),NULL,password.c_str(),NULL,NULL/*,NULL*/); /*create authentication structure from identity*/
          linphone_core_add_auth_info(this->lc_,authinfo); /*add authentication info to LinphoneCore*/
  }
  /* configure proxy entries */
  linphone_proxy_config_set_identity(proxy_cfg,identity.c_str()); /*set identity with user name and domain*/
  server_addr = linphone_address_get_domain(from); /*extract domain address from identity*/
  linphone_proxy_config_set_server_addr(proxy_cfg,server_addr); /* we assume domain = proxy server address*/
  linphone_proxy_config_enable_register(proxy_cfg,TRUE); /*activate registration for this proxy config*/
  linphone_address_destroy(from); //DEPRACTED...
  //... will be replaced by linphone_address_unref(from); /*release resource*/
  linphone_core_add_proxy_config(this->lc_,proxy_cfg); /*add proxy config to linphone core*/
  linphone_core_set_default_proxy(this->lc_,proxy_cfg); /*set to default proxy*/

}

void Linphone::Unegister(const Napi::CallbackInfo& info) {
  LinphoneProxyConfig* proxy_cfg;
  linphone_core_get_default_proxy(this->lc_, &proxy_cfg); // DEPRECATED...
  //... will be replaced by  proxy_cfg = linphone_core_get_default_proxy_config(lc); /* get default proxy config*/
  linphone_proxy_config_edit(proxy_cfg); /*start editing proxy configuration*/
  linphone_proxy_config_enable_register(proxy_cfg,FALSE); /*de-activate registration for this proxy config*/
  linphone_proxy_config_done(proxy_cfg); /*initiate REGISTER with expire = 0*/
  while(linphone_proxy_config_get_state(proxy_cfg) !=  LinphoneRegistrationCleared){
          linphone_core_iterate(this->lc_); /*to make sure we receive call backs before shutting down*/
          ms_usleep(50000);
  }
  /* Finally de-init liblinphone and free its internal structure */
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
