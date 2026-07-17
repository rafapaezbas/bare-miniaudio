include_guard(GLOBAL)

set(lib lib/libminiaudio.a)

declare_port(
  "github:mackron/miniaudio#master"
  miniaudio
  ARGS
    -DBUILD_SHARED_LIBS=OFF
  BYPRODUCTS
    ${lib}
)

add_library(miniaudio STATIC IMPORTED GLOBAL)

add_dependencies(miniaudio ${miniaudio})

set_target_properties(
  miniaudio
  PROPERTIES
  IMPORTED_LOCATION "${miniaudio_PREFIX}/${lib}"
)

file(MAKE_DIRECTORY "${miniaudio_PREFIX}/include")

target_include_directories(
  miniaudio
  INTERFACE "${miniaudio_PREFIX}/include"
)
