include_guard(GLOBAL)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(lib lib/miniaudio.lib)
else()
  set(lib lib/libminiaudio.a)
endif()

set(args -DBUILD_SHARED_LIBS=OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  list(APPEND args
    "-DCMAKE_C_FLAGS=-x objective-c"
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  list(APPEND args
    -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
    -DCMAKE_POLICY_DEFAULT_CMP0091=NEW
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
