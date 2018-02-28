
#include <napi.h>
#include "linphone.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return Linphone::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
