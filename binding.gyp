{
  "targets": [
    {
      "target_name": "liblinphone-js",
      "sources": [
        "binding.cpp",
        "linphone.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "libraries": [
        "/usr/lib/x86_64-linux-gnu/liblinphone.so.5",
        "/usr/lib/x86_64-linux-gnu/libmediastreamer_base.so.3"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_CPP_EXCEPTIONS"]
    }
  ]
}
