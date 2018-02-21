#include "notifications.h"
/*
 * Call state notification callback
 */
void call_state_changed(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg)
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

void registration_state_changed(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message)
{
        printf("New registration state %s for user id [%s] at proxy [%s]\n"
                        ,linphone_registration_state_to_string(cstate)
                        ,linphone_proxy_config_get_identity(cfg)
                        ,linphone_proxy_config_get_addr(cfg));
}
