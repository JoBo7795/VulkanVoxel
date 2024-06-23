#include "SwapChain.h"
#include "VulkanDevices.h"

SwapChainManager* SwapChainManager::instance = nullptr;

SwapChainManager::SwapChainManager() {

}

SwapChainManager::~SwapChainManager() {
    CleanupSwapChain();

}

SwapChainManager* SwapChainManager::GetInstance() {

    if (instance == nullptr) {
        instance = new SwapChainManager();
    }
    return instance;
}

VkSurfaceFormatKHR SwapChainManager::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChainManager::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChainManager::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
        glfwGetFramebufferSize( Window::GetWindowRef(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

SwapChainSupportDetails SwapChainManager::QuerySwapChainSupport(VkSurfaceKHR& surface, VkPhysicalDevice physDevice) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

void SwapChainManager::CreateSwapChain(VkSurfaceKHR surface) {

    auto devInstanceRef = VulkanDevices::GetInstance();

    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(surface,VulkanDevices::GetInstance()->GetPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VulkanQueueManager::GetInstance()->FindQueueFamilies(devInstanceRef->GetPhysicalDevice(),surface);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(devInstanceRef->GetDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(devInstanceRef->GetDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(devInstanceRef->GetDevice(), swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void SwapChainManager::CreateImageViews() {

    swapChainImageViews.resize(swapChainImages.size());


    for (uint32_t i = 0; i < GetSwapChainImages().size(); i++) {
        swapChainImageViews[i] = Image::CreateImageView(GetSwapChainImages()[i], GetSwapChainImageFormat(), VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
}

void SwapChainManager::CreateDepthRessources() {

    depthRessources.CreateDepthResources(swapChainExtent.width,swapChainExtent.height);
}


void SwapChainManager::CreateFrameBufferRessources(VkRenderPass& renderPass) {

    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            std::array<VkImageView, 2> attachments = {
            swapChainImageViews[i],
            depthRessources.GetImageView()
        };

        BufferManager::GetInstance()->CreateFramebuffers(renderPass,attachments,swapChainExtent.width,swapChainExtent.height,swapChainFramebuffers[i]);
    }
    
}

void SwapChainManager::CleanupSwapChain() {

    auto devInstanceRef = VulkanDevices::GetInstance();

    // vkDestroyImageView(devInstanceRef->GetDevice(), depthImageView, nullptr);
    // vkDestroyImage(devInstanceRef->GetDevice(), depthImage, nullptr);
    // vkFreeMemory(devInstanceRef->GetDevice(), depthImageMemory, nullptr);

    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(devInstanceRef->GetDevice(), swapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vkDestroyImageView(devInstanceRef->GetDevice(), swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(devInstanceRef->GetDevice(), swapChain, nullptr);

}

void SwapChainManager::RecreateSwapChain(Window& window,VkRenderPass& renderPass) {

    int width = 0, height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window.GetWindowRef(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(VulkanDevices::GetInstance()->GetDevice());

    CleanupSwapChain();

    CreateSwapChain(window.GetSurface());
    CreateImageViews();
    depthRessources.CreateDepthResources(swapChainExtent.width,swapChainExtent.height);
    CreateDepthRessources();
    
}

// Getter für swapChain_
VkSwapchainKHR SwapChainManager::GetSwapChain() {
    return swapChain;
}

// Setter für swapChain_
void SwapChainManager::SetSwapChain(VkSwapchainKHR swapChain) {
    swapChain = swapChain;
}

// Getter für swapChainImages_
const std::vector<VkImage>& SwapChainManager::GetSwapChainImages() {
    return swapChainImages;
}

// Setter für swapChainImages_
void SwapChainManager::SetSwapChainImages(const std::vector<VkImage>& images) {
    swapChainImages = images;
}

// Getter für swapChainImageFormat_
VkFormat SwapChainManager::GetSwapChainImageFormat() {
    return swapChainImageFormat;
}

// Setter für swapChainImageFormat_
void SwapChainManager::SetSwapChainImageFormat(VkFormat format) {
    swapChainImageFormat = format;
}

// Getter für swapChainExtent_
VkExtent2D SwapChainManager::GetSwapChainExtent() {
    return swapChainExtent;
}

// Setter für swapChainExtent_
void SwapChainManager::SetSwapChainExtent(VkExtent2D extent) {
    swapChainExtent = extent;
}

// Getter für swapChainImageViews_
const std::vector<VkImageView>& SwapChainManager::GetSwapChainImageViews() {
    return swapChainImageViews;
}

// Setter für swapChainImageViews_
void SwapChainManager::SetSwapChainImageViews(const std::vector<VkImageView>& views) {
    swapChainImageViews = views;
}

void SwapChainManager::SetDepthRessources(DepthRessources& in_depthRessources) {
    depthRessources = in_depthRessources;

}

// Getter für swapChainFramebuffers_
std::vector<VkFramebuffer>& SwapChainManager::GetSwapChainFramebuffers() {
    return swapChainFramebuffers;
}

void SwapChainManager::SetSwapChainFramebuffers(std::vector<VkFramebuffer>& framebuffers) {
    swapChainFramebuffers = framebuffers;
}
