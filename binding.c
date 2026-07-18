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

static js_threadsafe_function_t *end_tsfn;

static void
on_end_audio(void *user_data, ma_sound *snd) {
  js_call_threadsafe_function(end_tsfn, NULL, js_threadsafe_function_nonblocking);
}

static void
on_end_js(js_env_t *env, js_value_t *fn, void *context, void *data) {
  js_value_t *global;
  js_get_global(env, &global);
  js_call_function(env, global, fn, 0, NULL, NULL);
}

static js_value_t *
bare_miniaudio_play(js_env_t *env, js_callback_info_t *info) {
  if (sound_loaded) {
    ma_sound_uninit(&sound);
    sound_loaded = false;
  }

  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

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

  js_create_threadsafe_function(
    env,
    argv[1],
    0,
    1,
    NULL,
    NULL,
    NULL,
    on_end_js,
    &end_tsfn
  );

  ma_sound_start(&sound);
  ma_sound_set_end_callback(&sound, on_end_audio, NULL);
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
  js_release_threadsafe_function(end_tsfn, js_threadsafe_function_release);
  end_tsfn = NULL;
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
