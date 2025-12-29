// SPDX-License-Identifier: WTFPL
// Memory and JIT optimizations for Poco F6 (LPDDR5X)
// Optimized for high-performance PS3 emulation

#ifndef POCO_F6_MEMORY_OPTIMIZATIONS_H
#define POCO_F6_MEMORY_OPTIMIZATIONS_H

#include <cstdint>
#include <cstddef>

// LPDDR5X Memory Configuration for Poco F6
namespace PocoF6Memory {
    
    // LPDDR5X specific optimizations (8533 MT/s on Poco F6)
    constexpr size_t CACHE_LINE_SIZE = 128;  // Snapdragon 8s Gen 3
    constexpr size_t PAGE_SIZE = 16384;      // 16KB pages
    constexpr size_t HUGE_PAGE_SIZE = 2 * 1024 * 1024;  // 2MB huge pages
    
    // Increased JIT cache sizes for fast LPDDR5X RAM
    // Original values are typically much smaller, we're 4-8x increasing them
    constexpr size_t PPU_JIT_CACHE_SIZE = 512 * 1024 * 1024;      // 512MB for PPU JIT (was ~64-128MB)
    constexpr size_t SPU_JIT_CACHE_SIZE = 256 * 1024 * 1024;      // 256MB for SPU JIT (was ~32-64MB)
    constexpr size_t SPU_LLVM_CACHE_SIZE = 1024 * 1024 * 1024;    // 1GB for SPU LLVM cache (was ~128-256MB)
    
    // Translation buffer sizes (for dynamic recompilation)
    constexpr size_t PPU_TRANSLATION_BUFFER = 256 * 1024 * 1024;  // 256MB
    constexpr size_t SPU_TRANSLATION_BUFFER = 128 * 1024 * 1024;  // 128MB
    
    // LLVM compilation thread count optimized for Snapdragon 8s Gen 3
    // Cortex-X4 (1 core) + Cortex-A720 (4 cores) + Cortex-A520 (3 cores) = 8 cores
    // Reserve 2 cores for system/rendering, use 6 for compilation
    constexpr uint32_t LLVM_COMPILE_THREADS = 6;
    
    // Memory pool sizes for PS3 emulation
    constexpr size_t RSX_MEMORY_POOL = 512 * 1024 * 1024;         // 512MB for RSX (GPU emulation)
    constexpr size_t MAIN_MEMORY_POOL = 512 * 1024 * 1024;        // 512MB for main PS3 RAM
    
    // Shader compilation cache
    constexpr size_t SHADER_CACHE_SIZE = 256 * 1024 * 1024;       // 256MB shader cache
    constexpr uint32_t MAX_CACHED_SHADERS = 8192;                 // Up from typical ~2048
    
    // Descriptor set pool sizes (for Vulkan)
    constexpr uint32_t DESCRIPTOR_POOL_SIZE = 4096;                // Increased from ~1024
    
    // Command buffer pool
    constexpr uint32_t COMMAND_BUFFER_COUNT = 256;                 // Increased from ~64
    
    // Frame buffering configuration
    constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;                   // Double buffering
    
    // Memory alignment helpers for optimal LPDDR5X performance
    inline constexpr size_t align_to_cache_line(size_t size) {
        return (size + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1);
    }
    
    inline constexpr size_t align_to_page(size_t size) {
        return (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    }
    
    inline constexpr size_t align_to_huge_page(size_t size) {
        return (size + HUGE_PAGE_SIZE - 1) & ~(HUGE_PAGE_SIZE - 1);
    }
    
    // Prefetch hints for LPDDR5X
    inline void prefetch_read(const void* addr) {
        __builtin_prefetch(addr, 0, 3);  // Read, high temporal locality
    }
    
    inline void prefetch_write(const void* addr) {
        __builtin_prefetch(addr, 1, 3);  // Write, high temporal locality
    }
    
    // Memory barrier optimizations for ARMv9-A
    inline void memory_barrier_acquire() {
        __atomic_thread_fence(__ATOMIC_ACQUIRE);
    }
    
    inline void memory_barrier_release() {
        __atomic_thread_fence(__ATOMIC_RELEASE);
    }
    
    inline void memory_barrier_full() {
        __atomic_thread_fence(__ATOMIC_SEQ_CST);
    }
}

// JIT Optimization Settings
namespace JITOptimizations {
    // Enable aggressive optimizations
    constexpr bool ENABLE_AGGRESSIVE_INLINING = true;
    constexpr bool ENABLE_LOOP_UNROLLING = true;
    constexpr bool ENABLE_VECTORIZATION = true;
    constexpr bool ENABLE_FAST_MATH = true;
    
    // LLVM optimization level (0-3, 3 is most aggressive)
    constexpr uint32_t LLVM_OPT_LEVEL = 3;
    
    // Pre-compilation settings
    constexpr bool ENABLE_PRECOMPILATION = true;
    constexpr bool ENABLE_CACHE_VALIDATION = false;  // Skip for speed
    
    // Block sizes for recompilation
    constexpr uint32_t PPU_BLOCK_SIZE = 512;  // Larger blocks = better optimization
    constexpr uint32_t SPU_BLOCK_SIZE = 512;
    
    // Compilation queue depth
    constexpr uint32_t COMPILATION_QUEUE_DEPTH = 128;
}

// Real Steel Game-Specific Optimizations
namespace RealSteelOptimizations {
    // FPS limiter bypass settings
    constexpr bool BYPASS_30FPS_LIMITER = true;
    constexpr bool UNLOCK_FRAMERATE = true;
    constexpr uint32_t TARGET_FPS = 60;  // Target 60 FPS instead of 30
    
    // VSync settings
    constexpr bool DISABLE_VSYNC = false;  // Keep VSync but increase refresh
    constexpr bool TRIPLE_BUFFERING = false;  // Use double buffering for lower latency
    
    // Frame pacing
    constexpr bool ENABLE_FRAME_PACING = true;
    constexpr double FRAME_TIME_MS = 16.666;  // ~60 FPS
    
    // Game-specific memory optimizations
    constexpr size_t GAME_TEXTURE_CACHE = 384 * 1024 * 1024;  // 384MB for textures
    constexpr size_t GAME_VERTEX_CACHE = 64 * 1024 * 1024;     // 64MB for vertices
}

// Performance monitoring (can be disabled in release builds)
namespace PerfMonitoring {
    constexpr bool ENABLE_PERF_COUNTERS = false;  // Disable for max performance
    constexpr bool LOG_FRAME_TIMES = false;
    constexpr bool LOG_MEMORY_USAGE = false;
}

#endif // POCO_F6_MEMORY_OPTIMIZATIONS_H
