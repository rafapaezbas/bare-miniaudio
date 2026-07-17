#include <bare.h>
#include <js.h>
#include <miniaudio/miniaudio.h>
#include <stdio.h>

static js_value_t *
bare_miniaudio_exports(js_env_t *env, js_value_t *exports) {
  printf("Hello, world from binding.c!\n");
  printf("miniaudio version: %s\n", ma_version_string());
  return exports;
}

BARE_MODULE(bare_miniaudio, bare_miniaudio_exports)
