
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("aps3e")

# === Poco F6 ARMv9-A Extreme Optimizations ===
add_compile_options(-march=armv9-a+sve+sve2)
add_compile_options(-mcpu=cortex-x4)
add_compile_options(-mtune=cortex-x4)
add_compile_options(-O3)
add_compile_options(-Ofast)
add_compile_options(-flto=full)
add_compile_options(-fomit-frame-pointer)
add_compile_options(-ffast-math)
add_compile_options(-funroll-loops)

# Disable debugging
add_compile_definitions(NDEBUG RELEASE_BUILD NO_DEBUG_LOG DISABLE_TELEMETRY)
add_compile_definitions(TARGET_FPS=30 FRAME_PACING_STRICT)

# Additional Adreno 735 GPU optimizations
add_compile_options(-DADRENO_735_OPTIMIZED=1)
add_compile_options(-DVK_ENABLE_BETA_EXTENSIONS=1)

# Memory optimizations for LPDDR5X
add_compile_options(-DLPDDR5X_OPTIMIZED=1)
add_compile_options(-DCACHE_LINE_SIZE=128)  # Snapdragon 8s Gen 3 cache line

add_compile_options(-stdlib=libc++)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_link_options(-Wl,-z,max-page-size=16384)
add_link_options(-Wl,--gc-sections)
add_link_options(-flto=full)
add_link_options(-fuse-ld=lld)
add_link_options(-Wl,--lto-O3)
add_link_options(-Wl,--icf=all)

set(CMAKE_CXX_STANDARD 20)

project(aps3e LANGUAGES C CXX)

add_library(lang_System SHARED lang_System.cpp)

add_library(hardware_ProcessorInfo SHARED hardware_ProcessorInfo.cpp)
target_link_libraries(hardware_ProcessorInfo PRIVATE vulkan)

add_subdirectory(rpcs3 EXCLUDE_FROM_ALL)

add_library(emu SHARED)
set_target_properties(emu PROPERTIES OUTPUT_NAME "e")

target_link_options(emu PRIVATE -Wl,--exclude-libs,ALL)

# Enable LTO for maximum performance on Poco F6
target_compile_options(emu PRIVATE -flto=thin)
target_link_options(emu PRIVATE -flto=thin)

target_sources(emu
        PRIVATE
        rpcs3/rpcs3/Input/raw_mouse_config.cpp
        rpcs3/rpcs3/Input/raw_mouse_handler.cpp
        rpcs3/rpcs3/Input/product_info.cpp
        rpcs3/rpcs3/rpcs3_version.cpp
        rpcs3/rpcs3/stb_image.cpp
        rpcs3/rpcs3/stdafx.cpp
        aps3e_rp3_impl.cpp
        aps3e_emu.cpp
        aps3e.cpp
        vkapi.cpp
        vkutil.cpp
        cpuinfo.cpp
        glsl2spv.cpp
        meminfo.cpp
        emulator.cpp
        emulator_aps3e.cpp
)

target_link_libraries(emu PRIVATE rpcs3_emu)
target_link_libraries(emu PRIVATE 3rdparty::yaml-cpp 3rdparty::hidapi 3rdparty::libusb 3rdparty::wolfssl 3rdparty::libcurl 3rdparty::glslang 3rdparty::ffmpeg)
target_link_libraries(emu PRIVATE android)
