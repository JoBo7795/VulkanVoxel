#include "VulkanDevices.h"

VulkanDevices* VulkanDevices::instance = nullptr;

VulkanDevices::VulkanDevices() {


}

VulkanDevices::~VulkanDevices() {
    vkDestroyDevice(device, nullptr);
    delete instance;

}

VulkanDevices* VulkanDevices::GetInstance() {

    if (instance == nullptr) {
        instance = new VulkanDevices();
    }
    return instance;
}

VkPhysicalDevice& VulkanDevices::PickPhysicalDevice(VkInstance instance,VkSurfaceKHR surface) {

    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

    for (auto& physDevice : physicalDevices) {
        if (IsDeviceSuitable(physDevice,surface)) {
            //return physicalDevice;
            this->physicalDevice = physDevice;
            return physicalDevice;
        }
    }

    if (this->physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}



bool VulkanDevices::IsDeviceSuitable(VkPhysicalDevice physDevice, VkSurfaceKHR surface) {
    QueueFamilyIndices indices = VulkanQueueManager::GetInstance()->FindQueueFamilies(physDevice, surface);

    bool extensionsSupported = CheckDeviceExtensionSupport(physDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = SwapChainManager::GetInstance()->QuerySwapChainSupport(surface,physDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physDevice, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}



bool VulkanDevices::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void VulkanDevices::CreateLogicalDevice(VkSurfaceKHR surface) {
    VulkanQueueManager* vulkanQueueManagerRef = VulkanQueueManager::GetInstance();
    QueueFamilyIndices indices = vulkanQueueManagerRef->FindQueueFamilies(physicalDevice,surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &vulkanQueueManagerRef->GetGraphicsQueue());
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &vulkanQueueManagerRef->GetPresentQueue());
}

void VulkanDevices::SetPhysicalDevice(VkPhysicalDevice in_physicalDevice) {

    physicalDevice = in_physicalDevice;
}

VkPhysicalDevice& VulkanDevices::GetPhysicalDevice() {

    return physicalDevice;
}

void VulkanDevices::SetDevice(VkDevice in_device) {
    device = in_device;
}

VkDevice& VulkanDevices::GetDevice() {
    return device;
}