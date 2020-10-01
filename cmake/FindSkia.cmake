set(SKIA_INCLUDE_DIRS
        "$ENV{SKIA_BUILD_DIR}"
        "$ENV{SKIA_BUILD_DIR}/include/core")

find_library(SKIA_LIBRARY
        NAMES skia libskia
        HINTS "$ENV{SKIA_BUILD_DIR}/out/Static"
        HINTS "$ENV{SKIA_BUILD_DIR}/out/Shared")

message(${SKIA_INCLUDE_DIRS})
message(${SKIA_LIBRARY})