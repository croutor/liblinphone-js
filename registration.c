#include "registration.h"

int registration(LinphoneCore *lc, char* identity, char* password)
{
  LinphoneAddress *from;
  LinphoneAuthInfo *info;
  LinphoneProxyConfig* proxy_cfg;
  const char* server_addr;
    /*create proxy config*/
  proxy_cfg = linphone_proxy_config_new();
  /*parse identity*/
  from = linphone_address_new(identity);
  if (from==NULL){
          printf("%s not a valid sip uri, must be like sip:toto@sip.linphone.org \n",identity);
          return -1;
  }
  if (password!=NULL){
          info=linphone_auth_info_new(linphone_address_get_username(from),NULL,password,NULL,NULL/*,NULL*/); /*create authentication structure from identity*/
          linphone_core_add_auth_info(lc,info); /*add authentication info to LinphoneCore*/
  }
  // configure proxy entries
  linphone_proxy_config_set_identity(proxy_cfg,identity); /*set identity with user name and domain*/
  server_addr = linphone_address_get_domain(from); /*extract domain address from identity*/
  linphone_proxy_config_set_server_addr(proxy_cfg,server_addr); /* we assume domain = proxy server address*/
  linphone_proxy_config_enable_register(proxy_cfg,TRUE); /*activate registration for this proxy config*/
  linphone_address_destroy(from); //DEPRACTED...
  //... will be replaced by linphone_address_unref(from); /*release resource*/

  linphone_core_add_proxy_config(lc,proxy_cfg); /*add proxy config to linphone core*/
  linphone_core_set_default_proxy(lc,proxy_cfg); /*set to default proxy*/
  return 0;
}

int unregistration(LinphoneCore *lc)
{
  LinphoneProxyConfig* proxy_cfg;
  linphone_core_get_default_proxy(lc, &proxy_cfg); // DEPRECATED...
  //... will be replaced by  proxy_cfg = linphone_core_get_default_proxy_config(lc); /* get default proxy config*/
  linphone_proxy_config_edit(proxy_cfg); /*start editing proxy configuration*/
  linphone_proxy_config_enable_register(proxy_cfg,FALSE); /*de-activate registration for this proxy config*/
  linphone_proxy_config_done(proxy_cfg); /*initiate REGISTER with expire = 0*/
  while(linphone_proxy_config_get_state(proxy_cfg) !=  LinphoneRegistrationCleared){
          linphone_core_iterate(lc); /*to make sure we receive call backs before shutting down*/
          ms_usleep(50000);
  }
  return 0;
}
