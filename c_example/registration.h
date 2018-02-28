#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "linphone/linphonecore.h"

int registration(LinphoneCore *lc, char* identity, char* password);
int unregistration(LinphoneCore *lc);

#endif
