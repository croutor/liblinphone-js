/*
linphone
Copyright (C) 2010  Belledonne Communications SARL
 (simon.morlat@linphone.org)
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "linphone/linphonecore.h"
#include <signal.h>

#include "notifications.h"

static bool_t running=TRUE;
static void stop(int signum){
        running=FALSE;
}

int main(int argc, char *argv[]){
        LinphoneCoreVTable vtable={0};
        LinphoneCore *lc;
        LinphoneCall *call=NULL;
        const char *dest="sip:thetestcall@sip2sip.info";
        /* take the destination sip uri from the command line arguments */
        if (argc>1){
                dest=argv[1];
        }
        signal(SIGINT,stop);
#ifdef DEBUG
        linphone_core_enable_logs(NULL); /*enable liblinphone logs.*/
#endif
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

        const char** devices = linphone_core_get_sound_devices 	(lc);
        for(int i = 0; devices[i]!=NULL; i++)
        {
          printf("device[%d] = %s\n", i , devices[i]);
        }
        printf("capture device : %s\n",
                linphone_core_get_capture_device(lc));
        /*

        linphone_core_set_capture_device( 	LinphoneCore *  	lc,
		                                          const char *  	devid );
        */
        if (dest){
                /*
                 Place an outgoing call
                */
                call=linphone_core_invite(lc,dest);
                if (call==NULL){
                        printf("Could not place call to %s\n",dest);
                        goto end;
                }else printf("Call to %s is in progress...",dest);
                linphone_call_ref(call);
        }
        /* main loop for receiving notifications and doing background linphonecore work: */
        while(running){
                linphone_core_iterate(lc);
                ms_usleep(50000);
        }
        if (call && linphone_call_get_state(call)!=LinphoneCallEnd){
                /* terminate the call */
                printf("Terminating the call...\n");
                linphone_core_terminate_call(lc,call);
                /*at this stage we don't need the call object */
                linphone_call_unref(call);
        }
end:
        printf("Shutting down...\n");
        linphone_core_destroy(lc);
        printf("Exited\n");
        return 0;
}
