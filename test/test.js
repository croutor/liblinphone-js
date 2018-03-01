
const linphone = require('bindings')('liblinphone-js');

let phone = new linphone.Linphone('/tmp/linphone.conf');
phone.enablelog();
