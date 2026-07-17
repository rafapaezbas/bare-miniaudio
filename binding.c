#include <bare.h>
#include <js.h>
#include <miniaudio/miniaudio.h>
#include <stdio.h>

ma_engine engine;
ma_sound sound;

bool sound_loaded = false;

static js_value_t *
bare_miniaudio_init(js_env_t *env, js_callback_info_t *info) {
  js_value_t *result;
  int err = js_create_int32(env, ma_engine_init(NULL, &engine), &result);
  assert(err == 0);
  return result;
}

static js_value_t *
bare_miniaudio_play(js_env_t *env, js_callback_info_t *info) {
  if (sound_loaded) {
    ma_sound_uninit(&sound);
    sound_loaded = false;
  }

  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  size_t len;
  err = js_get_value_string_utf8(env, argv[0], NULL, 0, &len);
  assert(err == 0);

  len += 1;

  char *path = malloc(len);
  err = js_get_value_string_utf8(env, argv[0], (utf8_t *) path, len, &len);

  ma_result r = ma_sound_init_from_file(&engine, path, 0, NULL, NULL, &sound);
  free(path);

  js_value_t *result;
  js_get_undefined(env, &result);

  if (r != MA_SUCCESS) {
    return result;
  }

  ma_sound_start(&sound);
  sound_loaded = true;

  return result;
}

static js_value_t *
bare_miniaudio_stop(js_env_t *env, js_callback_info_t *info) {

  js_value_t *result;
  js_get_undefined(env, &result);

  if (!sound_loaded) {
    return result;
  }

  ma_sound_uninit(&sound);
  sound_loaded = false;

  return result;
}

static js_value_t *
bare_miniaudio_exports(js_env_t *env, js_value_t *exports) {
  int err;
  js_value_t *val;

  err = js_create_function(env, "init", -1, bare_miniaudio_init, NULL, &val);
  assert(err == 0);
  err = js_set_named_property(env, exports, "init", val);
  assert(err == 0);

  err = js_create_function(env, "play", -1, bare_miniaudio_play, NULL, &val);
  assert(err == 0);
  err = js_set_named_property(env, exports, "play", val);
  assert(err == 0);

  err = js_create_function(env, "stop", -1, bare_miniaudio_stop, NULL, &val);
  assert(err == 0);
  err = js_set_named_property(env, exports, "stop", val);
  assert(err == 0);

  return exports;
}

BARE_MODULE(bare_miniaudio, bare_miniaudio_exports)
