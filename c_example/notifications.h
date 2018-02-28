#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include "linphone/linphonecore.h"

void call_state_changed(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg);
void registration_state_changed(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message);

#endif
