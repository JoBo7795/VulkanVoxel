#include "DepthRessources.h"
//#include "VulkanDevices.h"
DepthRessources::~DepthRessources() {

    // auto device = VulkanDevices::GetInstance()->GetDevice();
    // 
    // vkDestroyImageView(device, depthImageView, nullptr);
    // vkDestroyImage(device, depthImage, nullptr);
    // vkFreeMemory(device, depthImageMemory, nullptr);
}

void DepthRessources::CreateDepthResources(float extentWidth, float extentHeight) {
    VkFormat depthFormat = FindDepthFormat();
    Image::CreateImage(extentWidth, extentHeight, 1, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = Image::CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    //transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}

VkFormat DepthRessources::FindDepthFormat() {
    return Image::FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool DepthRessources::HasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkImageView& DepthRessources::GetImageView() {
    return depthImageView;
}