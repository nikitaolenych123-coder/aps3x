// SPDX-License-Identifier: WTFPL
// Adreno 735 GPU Optimizations for Poco F6
// Created for maximum PS3 emulation performance

#ifndef ADRENO735_OPTIMIZATIONS_H
#define ADRENO735_OPTIMIZATIONS_H

#include <vulkan/vulkan.h>
#include <unordered_map>
#include <string>
#include <vector>

// Adreno 735 specific features and limits
#define ADRENO_735_VENDOR_ID 0x5143  // Qualcomm
#define ADRENO_735_DEVICE_ID 0x43051200  // Adreno 735

// Optimal configuration for Adreno 735
namespace Adreno735 {
    
    // Shader cache for pre-compilation
    class ShaderCache {
    private:
        std::unordered_map<std::string, VkShaderModule> cache_;
        VkDevice device_;
        
    public:
        ShaderCache(VkDevice dev) : device_(dev) {}
        
        ~ShaderCache() {
            for (auto& [key, module] : cache_) {
                vkDestroyShaderModule(device_, module, nullptr);
            }
        }
        
        VkShaderModule get(const std::string& key) {
            auto it = cache_.find(key);
            return (it != cache_.end()) ? it->second : VK_NULL_HANDLE;
        }
        
        void put(const std::string& key, VkShaderModule module) {
            cache_[key] = module;
        }
        
        size_t size() const { return cache_.size(); }
        void clear() { cache_.clear(); }
    };
    
    // Adreno 735 optimal settings
    struct OptimalSettings {
        // Wave size optimization (Adreno 735 uses 128-wide wavefronts)
        static constexpr uint32_t WAVE_SIZE = 128;
        
        // Optimal workgroup sizes for compute shaders
        static constexpr uint32_t COMPUTE_WORKGROUP_SIZE_X = 16;
        static constexpr uint32_t COMPUTE_WORKGROUP_SIZE_Y = 16;
        static constexpr uint32_t COMPUTE_WORKGROUP_SIZE_Z = 1;
        
        // Memory alignment (optimal for Adreno cache lines)
        static constexpr VkDeviceSize BUFFER_ALIGNMENT = 256;
        static constexpr VkDeviceSize IMAGE_ALIGNMENT = 256;
        
        // Descriptor pool sizes optimized for PS3 emulation
        static constexpr uint32_t MAX_DESCRIPTOR_SETS = 4096;
        static constexpr uint32_t UNIFORM_BUFFER_DESCRIPTORS = 2048;
        static constexpr uint32_t STORAGE_BUFFER_DESCRIPTORS = 2048;
        static constexpr uint32_t SAMPLED_IMAGE_DESCRIPTORS = 2048;
        static constexpr uint32_t STORAGE_IMAGE_DESCRIPTORS = 1024;
        
        // Command buffer pool size
        static constexpr uint32_t COMMAND_BUFFER_POOL_SIZE = 256;
        
        // Frame buffering
        static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
        
        // Pipeline cache size (MB)
        static constexpr size_t PIPELINE_CACHE_SIZE = 64 * 1024 * 1024;
    };
    
    // Recommended device features for Adreno 735
    inline VkPhysicalDeviceFeatures getRecommendedFeatures() {
        VkPhysicalDeviceFeatures features = {};
        features.robustBufferAccess = VK_TRUE;
        features.fullDrawIndexUint32 = VK_TRUE;
        features.imageCubeArray = VK_TRUE;
        features.independentBlend = VK_TRUE;
        features.geometryShader = VK_TRUE;
        features.tessellationShader = VK_TRUE;
        features.sampleRateShading = VK_TRUE;
        features.dualSrcBlend = VK_TRUE;
        features.logicOp = VK_TRUE;
        features.multiDrawIndirect = VK_TRUE;
        features.drawIndirectFirstInstance = VK_TRUE;
        features.depthClamp = VK_TRUE;
        features.depthBiasClamp = VK_TRUE;
        features.fillModeNonSolid = VK_TRUE;
        features.depthBounds = VK_TRUE;
        features.wideLines = VK_TRUE;
        features.largePoints = VK_TRUE;
        features.alphaToOne = VK_TRUE;
        features.multiViewport = VK_TRUE;
        features.samplerAnisotropy = VK_TRUE;
        features.textureCompressionETC2 = VK_TRUE;
        features.textureCompressionASTC_LDR = VK_TRUE;
        features.textureCompressionBC = VK_FALSE;  // Not supported on mobile
        features.occlusionQueryPrecise = VK_TRUE;
        features.pipelineStatisticsQuery = VK_TRUE;
        features.vertexPipelineStoresAndAtomics = VK_TRUE;
        features.fragmentStoresAndAtomics = VK_TRUE;
        features.shaderTessellationAndGeometryPointSize = VK_TRUE;
        features.shaderImageGatherExtended = VK_TRUE;
        features.shaderStorageImageExtendedFormats = VK_TRUE;
        features.shaderStorageImageMultisample = VK_TRUE;
        features.shaderStorageImageReadWithoutFormat = VK_TRUE;
        features.shaderStorageImageWriteWithoutFormat = VK_TRUE;
        features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
        features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
        features.shaderStorageBufferArrayDynamicIndexing = VK_TRUE;
        features.shaderStorageImageArrayDynamicIndexing = VK_TRUE;
        features.shaderClipDistance = VK_TRUE;
        features.shaderCullDistance = VK_TRUE;
        features.shaderFloat64 = VK_FALSE;  // Not typically on mobile
        features.shaderInt64 = VK_TRUE;
        features.shaderInt16 = VK_TRUE;
        return features;
    }
    
    // Get recommended device extensions for Adreno 735
    inline std::vector<const char*> getRecommendedExtensions() {
        return {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_MAINTENANCE1_EXTENSION_NAME,
            VK_KHR_MAINTENANCE2_EXTENSION_NAME,
            VK_KHR_MAINTENANCE3_EXTENSION_NAME,
            VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME,
            VK_KHR_16BIT_STORAGE_EXTENSION_NAME,
            VK_KHR_8BIT_STORAGE_EXTENSION_NAME,
            VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME,
            VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,
            VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME,
            VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
            VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
            VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME,
            VK_EXT_SHADER_SUBGROUP_BALLOT_EXTENSION_NAME,
            VK_EXT_SHADER_SUBGROUP_VOTE_EXTENSION_NAME,
            // Qualcomm specific extensions
            "VK_QCOM_render_pass_shader_resolve",
            "VK_QCOM_render_pass_transform",
            "VK_QCOM_rotated_copy_commands",
        };
    }
    
    // Memory allocation hints for LPDDR5X
    inline VkMemoryAllocateInfo getOptimalMemoryAllocInfo(VkDeviceSize size, uint32_t memoryTypeIndex) {
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = size;
        allocInfo.memoryTypeIndex = memoryTypeIndex;
        return allocInfo;
    }
    
    // Create optimized command pool
    inline VkCommandPoolCreateInfo getOptimalCommandPoolCreateInfo(uint32_t queueFamilyIndex) {
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | 
                        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;  // For better performance
        poolInfo.queueFamilyIndex = queueFamilyIndex;
        return poolInfo;
    }
    
    // Minimize CPU-GPU synchronization
    inline VkSubmitInfo getOptimalSubmitInfo(
        uint32_t commandBufferCount,
        const VkCommandBuffer* pCommandBuffers,
        uint32_t waitSemaphoreCount = 0,
        const VkSemaphore* pWaitSemaphores = nullptr,
        const VkPipelineStageFlags* pWaitDstStageMask = nullptr,
        uint32_t signalSemaphoreCount = 0,
        const VkSemaphore* pSignalSemaphores = nullptr
    ) {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = waitSemaphoreCount;
        submitInfo.pWaitSemaphores = pWaitSemaphores;
        submitInfo.pWaitDstStageMask = pWaitDstStageMask;
        submitInfo.commandBufferCount = commandBufferCount;
        submitInfo.pCommandBuffers = pCommandBuffers;
        submitInfo.signalSemaphoreCount = signalSemaphoreCount;
        submitInfo.pSignalSemaphores = pSignalSemaphores;
        return submitInfo;
    }
    
    // FPS optimization: Reduce present mode latency
    inline VkPresentModeKHR getOptimalPresentMode(const std::vector<VkPresentModeKHR>& availableModes) {
        // Prefer mailbox for lowest latency and no tearing
        for (const auto& mode : availableModes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return mode;
            }
        }
        // Fallback to immediate for maximum FPS
        for (const auto& mode : availableModes) {
            if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                return mode;
            }
        }
        // FIFO is always available as fallback
        return VK_PRESENT_MODE_FIFO_KHR;
    }
}

#endif // ADRENO735_OPTIMIZATIONS_H
