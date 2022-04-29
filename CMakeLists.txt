project(sample LANGUAGES CUDA C CXX)
cmake_minimum_required(VERSION 3.17)
set(OWL_BUILD_SAMPLES OFF CACHE BOOL "Build OWL Samples")
add_subdirectory(owl EXCLUDE_FROM_ALL)
include(embed_ptx)

embed_ptx(
    OUTPUT_TARGET
        embedded
    EMBEDDED_SYMBOL_NAMES
        deviceCode_ptx
    PTX_LINK_LIBRARIES
        owl::owl
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/deviceCode.cu
    )
add_executable(owl-buffer-sample hostCode.cpp deviceCode.h ${embedded})
target_link_libraries(owl-buffer-sample owl::owl embedded)
