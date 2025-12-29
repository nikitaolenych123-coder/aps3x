# APS3E High-Performance Build for Poco F6

## Overview

This repository has been optimized specifically for the **Poco F6** (Snapdragon 8s Gen 3) to deliver maximum PS3 emulation performance. The optimizations target the Cortex-X4 CPU cores, Adreno 735 GPU, and LPDDR5X RAM configuration.

## Device Specifications

- **SoC**: Qualcomm Snapdragon 8s Gen 3 (SM8635)
- **CPU**: 
  - 1x Cortex-X4 @ 3.0 GHz (Prime)
  - 4x Cortex-A720 @ 2.8 GHz (Performance)
  - 3x Cortex-A520 @ 2.0 GHz (Efficiency)
- **GPU**: Adreno 735
- **RAM**: LPDDR5X @ 8533 MT/s
- **Architecture**: ARMv9-A with SVE2 support

## Applied Optimizations

### 1. CPU Optimizations

#### Compiler Flags
- **Architecture**: `-march=armv9-a+sve+sve2`
- **CPU Tuning**: `-mtune=cortex-x4 -mcpu=cortex-x4`
- **Optimization Level**: `-Ofast` (maximum speed)
- **Additional Flags**:
  - `-flto=thin` - Link Time Optimization for whole-program optimization
  - `-fomit-frame-pointer` - Free up an extra register
  - `-ffast-math` - Aggressive floating-point optimizations
  - `-funroll-loops` - Loop unrolling for better pipelining
  - `-fvectorize` - Auto-vectorization with SVE2
  - `-fassociative-math` - Allow math reassociation

#### Build Configuration
Location: [app/src/main/cpp/CMakeLists.txt](app/src/main/cpp/CMakeLists.txt)

```cmake
# ARMv9-A with Cortex-X4 optimizations
add_compile_options(-march=armv9-a+sve+sve2)
add_compile_options(-mtune=cortex-x4)
add_compile_options(-Ofast)
add_compile_options(-flto=thin)
```

### 2. GPU Optimizations (Adreno 735)

#### Vulkan Pipeline Optimizations
Location: [app/src/main/cpp/adreno735_optimizations.h](app/src/main/cpp/adreno735_optimizations.h)

**Features**:
- Shader pre-caching system to avoid runtime compilation stutters
- Optimized descriptor pool sizes (4096 sets vs typical 1024)
- Reduced CPU-GPU synchronization overhead
- Optimal present mode selection (Mailbox > Immediate > FIFO)
- 128-wide wavefront optimization (Adreno 735 native size)
- Qualcomm-specific extensions enabled

**Key Settings**:
```cpp
WAVE_SIZE = 128                    // Adreno 735 native wavefront size
MAX_DESCRIPTOR_SETS = 4096         // Increased for PS3 emulation
PIPELINE_CACHE_SIZE = 64MB         // Large pipeline cache
MAX_FRAMES_IN_FLIGHT = 2           // Double buffering
```

### 3. Memory Optimizations (LPDDR5X)

#### Enhanced Memory Management
Location: [app/src/main/cpp/poco_f6_optimizations.h](app/src/main/cpp/poco_f6_optimizations.h)

**JIT Cache Sizes** (Increased 4-8x):
- **PPU JIT Cache**: 512 MB (was ~64-128 MB)
- **SPU JIT Cache**: 256 MB (was ~32-64 MB)
- **SPU LLVM Cache**: 1024 MB (was ~128-256 MB)
- **Shader Cache**: 256 MB (was ~64 MB)

**Translation Buffers**:
- PPU Translation Buffer: 256 MB
- SPU Translation Buffer: 128 MB

**Memory Alignment**:
- Cache line: 128 bytes (Snapdragon 8s Gen 3)
- Page size: 16 KB
- Huge pages: 2 MB

**LLVM Compilation**:
- Threads: 6 (optimized for 8-core config)
- Optimization level: 3 (maximum)

### 4. Game-Specific: Real Steel FPS Unlock

#### Frame Limiter Bypass
Location: [app/src/main/cpp/real_steel_fps_patch.h](app/src/main/cpp/real_steel_fps_patch.h)

**Features**:
- Bypasses 30 FPS limiter
- Targets 60 FPS gameplay
- Physics compensation for higher frame rates
- VSync optimization

**Implementation**:
- Pattern-based memory patching
- Runtime frame time adjustment
- Game speed compensation

## Building the Optimized APK

### Prerequisites

- **Linux** (Ubuntu 20.04+ recommended)
- **Java Development Kit** (JDK 17 or later)
- **8GB+ RAM** for parallel compilation
- **20GB+ free disk space**

### Automated Build

Run the provided build script:

```bash
./build_poco_f6.sh
```

The script will:
1. ✓ Install Android SDK and NDK (v27.2.12479018)
2. ✓ Install build dependencies (CMake, Ninja)
3. ✓ Configure Gradle with optimal settings
4. ✓ Apply Poco F6 specific optimizations
5. ✓ Build native libraries with -Ofast
6. ✓ Generate optimized Release APK
7. ✓ Create detailed build report

### Manual Build

If you prefer manual control:

```bash
# 1. Install NDK
export ANDROID_HOME=$HOME/Android/Sdk
sdkmanager "ndk;27.2.12479018"

# 2. Clean previous builds
./gradlew clean

# 3. Build Release APK
./gradlew assembleRelease \
    -Pandroid.injected.build.abi=arm64-v8a \
    --max-workers=$(nproc) \
    --parallel
```

### Output Location

- **APK**: `build_output/aps3e-poco-f6-optimized.apk`
- **Build Report**: `build_output/BUILD_REPORT.txt`

## Installation

1. **Enable Unknown Sources**:
   - Settings → Security → Unknown Sources

2. **Transfer APK**:
   ```bash
   adb install build_output/aps3e-poco-f6-optimized.apk
   ```
   Or copy manually to device

3. **Grant Permissions**:
   - Storage access
   - External storage management

## Recommended In-App Settings

### For Best Performance

**CPU Settings**:
- CPU Decoder: **LLVM (Recompiler)**
- SPU Decoder: **LLVM (ASMJIT)**
- SPU Threads: **3-4** (depends on game)
- LLVM CPU: **cortex-x4** (or native)

**GPU Settings**:
- Renderer: **Vulkan**
- Resolution Scale: **1x-2x** (native to 2x depending on game)
- Anisotropic Filter: **Automatic**
- Anti-Aliasing: **Disabled** or **FXAA**
- VSync: **Off** for minimum latency

**Memory Settings**:
- Shader Cache: **Enabled**
- SPU Cache: **Enabled**
- Async Shader Compilation: **Enabled**

### Real Steel Specific Settings

Since Real Steel FPS limiter bypass is included:

- **Frame Limit**: **60 FPS** or **Auto**
- **Resolution**: **Native** or **1.5x**
- **Enable Shader Cache**: **Yes**
- **VSync**: **Off** for unlocked FPS

## Performance Expectations

### General PS3 Emulation

- **2D/Light 3D Games**: 60 FPS (native) with 2x resolution
- **Moderate 3D Games**: 30-60 FPS at native resolution
- **Heavy 3D Games**: 20-40 FPS at native resolution

### Real Steel Performance

With optimizations:
- **Target**: 60 FPS (up from 30 FPS locked)
- **Resolution**: Native to 1.5x
- **Stability**: Improved frame times, reduced stuttering

## Technical Details

### Compiler Optimizations Applied

| Flag | Purpose | Benefit |
|------|---------|---------|
| `-Ofast` | Maximum speed optimization | ~10-20% performance gain |
| `-flto=thin` | Link Time Optimization | ~5-10% overall improvement |
| `-march=armv9-a+sve+sve2` | ARMv9 with SVE2 | SIMD optimizations |
| `-mtune=cortex-x4` | Cortex-X4 tuning | CPU-specific optimizations |
| `-fomit-frame-pointer` | Remove frame pointer | Extra register available |
| `-ffast-math` | Fast floating-point | ~5-15% in FP-heavy code |
| `-funroll-loops` | Loop unrolling | Better instruction pipelining |

### Modified Files

1. **Build Configuration**:
   - [app/src/main/cpp/CMakeLists.txt](app/src/main/cpp/CMakeLists.txt) - CPU optimizations
   - [app/build.gradle](app/build.gradle) - Android build settings
   - [gradle.properties](gradle.properties) - Gradle performance

2. **Optimization Headers** (NEW):
   - [app/src/main/cpp/adreno735_optimizations.h](app/src/main/cpp/adreno735_optimizations.h)
   - [app/src/main/cpp/poco_f6_optimizations.h](app/src/main/cpp/poco_f6_optimizations.h)
   - [app/src/main/cpp/real_steel_fps_patch.h](app/src/main/cpp/real_steel_fps_patch.h)

3. **Build Script**:
   - [build_poco_f6.sh](build_poco_f6.sh) - Automated build

## Troubleshooting

### Build Issues

**Issue**: CMake version too old
```bash
# Solution: Install latest CMake
sudo apt-get update
sudo apt-get install cmake
```

**Issue**: Out of memory during build
```bash
# Solution: Reduce parallel jobs
./gradlew assembleRelease --max-workers=2
```

**Issue**: NDK not found
```bash
# Solution: Set NDK path explicitly
export ANDROID_NDK_HOME=$HOME/Android/Sdk/ndk/27.2.12479018
```

### Runtime Issues

**Issue**: App crashes on startup
- Ensure Android API level 28+ (Android 9.0+)
- Grant all storage permissions
- Clear app data and cache

**Issue**: Poor performance
- Check thermal throttling (Poco F6 may throttle under sustained load)
- Lower resolution scale
- Disable per-game enhancements
- Enable frame skip if needed

**Issue**: Real Steel still locked to 30 FPS
- FPS unlock is automatic but may need game restart
- Check in-game settings for frame limiter
- Some game versions may require additional patches

## Benchmarking

### Test Methodology

1. Use built-in performance overlay
2. Monitor FPS, frame time, CPU/GPU usage
3. Test with demanding scenes
4. Run for 15+ minutes to check thermal stability

### Expected Results (Poco F6)

| Game Category | FPS (Native Res) | FPS (2x Res) |
|---------------|------------------|--------------|
| 2D Games | 60 (stable) | 60 (stable) |
| Light 3D | 50-60 | 40-50 |
| Medium 3D | 30-45 | 25-35 |
| Heavy 3D | 20-30 | 15-25 |

## Contributing

If you discover additional optimizations or game-specific patches:

1. Test thoroughly on Poco F6
2. Document performance impact
3. Submit pull request with:
   - Description of optimization
   - Before/after benchmarks
   - Affected games

## Credits

- **Base Emulator**: APS3E
- **RPCS3 Core**: RPCS3 Team
- **Optimizations**: Poco F6 specific enhancements
- **Build Script**: Automated build system

## License

This project maintains the original APS3E license (WTFPL).

## Disclaimer

- PS3 emulation is resource-intensive
- Not all games will run at full speed
- Poco F6 thermal management may affect sustained performance
- Use original game files only (legal backups)

## Support

For issues specific to these optimizations:
- Create an issue with device info, game title, and performance logs
- Include build_output/BUILD_REPORT.txt

For general APS3E support:
- Refer to original project documentation

---

**Last Updated**: December 2025  
**Target Device**: Poco F6 (Snapdragon 8s Gen 3)  
**Optimization Level**: Maximum Performance
