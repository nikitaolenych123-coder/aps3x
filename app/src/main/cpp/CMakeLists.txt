
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

# === Poco F6 Optimizations (AGGRESSIVE - Real Steel 60 FPS) ===
# ARMv9-A architecture with Cortex-X4 optimizations for Snapdragon 8s Gen 3
add_compile_options(-march=armv9-a+sve+sve2+crypto+dotprod+fp16)
add_compile_options(-mcpu=cortex-x4)
add_compile_options(-mtune=cortex-x4)

# Maximum optimization level
add_compile_options(-Ofast)  # Aggressive optimizations beyond -O3
add_compile_options(-flto=thin)  # Link Time Optimization
add_compile_options(-fomit-frame-pointer)
add_compile_options(-ffast-math)  # Fast floating-point math
add_compile_options(-funroll-loops)  # Loop unrolling
add_compile_options(-fvectorize)  # Auto-vectorization with SVE2
add_compile_options(-fassociative-math)  # Allow math reassociation
add_compile_options(-freciprocal-math)  # Fast reciprocal approximations
add_compile_options(-fno-signed-zeros)  # Treat signed zeros as unsigned
add_compile_options(-fno-trapping-math)  # Assume no FP exceptions

# Disable debugging for maximum performance
add_compile_definitions(NDEBUG RELEASE_BUILD NO_DEBUG_LOG DISABLE_TELEMETRY)
add_compile_definitions(TARGET_FPS=60 FRAME_PACING_RELAXED)  # 60 FPS target for Real Steel
add_compile_definitions(REAL_STEEL_FPS_UNLOCK=1)

# Adreno 735 GPU optimizations
add_compile_definitions(ADRENO_735_OPTIMIZED=1)
add_compile_definitions(VK_ENABLE_BETA_EXTENSIONS=1)
add_compile_definitions(ENABLE_SHADER_CACHE=1)
add_compile_definitions(ADRENO_WAVEFRONT_SIZE=128)

# Memory optimizations for LPDDR5X (8533 MT/s)
add_compile_definitions(LPDDR5X_OPTIMIZED=1)
add_compile_definitions(CACHE_LINE_SIZE=128)  # Snapdragon 8s Gen 3 cache line
add_compile_definitions(ENABLE_HUGE_PAGES=1)
add_compile_definitions(PREFETCH_ENABLED=1)

add_compile_options(-stdlib=libc++)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Aggressive link-time optimizations for Poco F6
add_link_options(-flto=thin)  # Link Time Optimization
add_link_options(-Wl,-z,max-page-size=16384)  # 16KB pages for ARM64
add_link_options(-Wl,--gc-sections)  # Remove unused sections
add_link_options(-Wl,--icf=all)  # Identical Code Folding
add_link_options(-Wl,-O3)  # Linker optimization level
add_link_options(-Wl,--as-needed)  # Only link needed libraries
add_link_options(-Wl,--strip-all)  # Strip all symbols for smaller binary

set(CMAKE_CXX_STANDARD 20)

project(aps3e LANGUAGES C CXX)

add_library(lang_System SHARED lang_System.cpp)
target_compile_options(lang_System PRIVATE -Ofast -flto=thin)
target_link_options(lang_System PRIVATE -flto=thin)

add_library(hardware_ProcessorInfo SHARED hardware_ProcessorInfo.cpp)
target_link_libraries(hardware_ProcessorInfo PRIVATE vulkan)
target_compile_options(hardware_ProcessorInfo PRIVATE -Ofast -flto=thin)
target_link_options(hardware_ProcessorInfo PRIVATE -flto=thin)

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
