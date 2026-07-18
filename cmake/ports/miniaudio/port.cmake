include_guard(GLOBAL)

set(lib lib/libminiaudio.a)

set(args -DBUILD_SHARED_LIBS=OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  list(APPEND args
    "-DCMAKE_C_FLAGS=-x objective-c"
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
  )
endif()

declare_port(
  "github:mackron/miniaudio#master"
  miniaudio
  ARGS
    ${args}
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
