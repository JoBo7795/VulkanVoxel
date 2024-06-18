#include "VulkanInstance.h"
#include "VulkanDevices.h"
#include "CommandPool.h"

VulkanInstance* VulkanInstance::objInstance = nullptr;

VulkanInstance::VulkanInstance() {

    CreateInstance();
    debugMessenger = ValidationLayers(instance);    
    window.CreateSurface(instance);
    
    VulkanDevices::GetInstance()->PickPhysicalDevice(instance,window.GetSurface());
    VulkanDevices::GetInstance()->CreateLogicalDevice(window.GetSurface());
    swapChain = SwapChainManager::GetInstance();
    swapChain->CreateSwapChain(window.GetSurface());
    swapChain->CreateImageViews();
    CommandPoolManager::GetInstance()->CreateCommandPool(window.GetSurface());
    texture.CreateTextureRessources();
    BufferManager::GetInstance()->CreateUniformBuffers();
    swapChain->CreateGraphicsPipeline(texture);
    //graphicsPipeline = GraphicsPipeline(texture);
    //graphicsPipeline.CreateRenderPass();
    //graphicsPipeline.CreateGraphicsPipeline();
    
    //depthRessources.CreateDepthResources(swapChain);
    //swapChain.SetDepthRessources(depthRessources);
    //BufferManager::GetInstance()->CreateFramebuffers(graphicsPipeline.GetRenderPass(), swapChain, depthRessources.GetImageView());
    model.LoadModelFromObjFile();
    BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    BufferManager::GetInstance()->CreateIndexBuffer(model.indices);

    //CommandPoolManager::GetInstance()->CreateCommandPool(window.GetSurface());
    //BufferManager::GetInstance()->CreateCommandBuffers();
    
}

VulkanInstance::~VulkanInstance() {
    Cleanup();
}


VulkanInstance* VulkanInstance::GetInstance() {

    if (objInstance == nullptr) {
        objInstance = new VulkanInstance();
    }
    return objInstance;
}

void VulkanInstance::Run() {
    

    InitVulkan();
    MainLoop();
    Cleanup();
}

void VulkanInstance::InitVulkan() {

    //CreateInstance();
    //SetupDebugMessenger();
    //CreateSurface();
    //PickPhysicalDevice();
    //CreateLogicalDevice();
    //CreateSwapChain();
    //CreateImageViews();
    //CreateRenderPass();
    //CreateDescriptorSetLayout();
    //CreateGraphicsPipeline();
    //CreateCommandPool();
    //CreateDepthResources();
    //CreateFramebuffers();
    //CreateTextureImage();
    //CreateTextureImageView();
    //CreateTextureSampler();
    //LoadModel();
    //CreateVertexBuffer();
    //CreateIndexBuffer();
    //CreateUniformBuffers();
    //CreateDescriptorPool();
    //CreateDescriptorSets();
    //CreateCommandBuffers();
    //CreateSyncObjects();
}


void VulkanInstance::MainLoop() {
    while (!glfwWindowShouldClose(window.GetWindowRef())) {
        glfwPollEvents();

        swapChain->GetGraphicsPipeLine().DrawFrame(window, model);
    }

    vkDeviceWaitIdle(VulkanDevices::GetInstance()->GetDevice());
}

void VulkanInstance::Cleanup() {

    //CleanupSwapChain();

    auto device = VulkanDevices::GetInstance()->GetDevice();

    vkDestroySampler(device, texture.textureSampler, nullptr);
    vkDestroyImageView(device, texture.textureImageView, nullptr);

    vkDestroyImage(device, texture.textureImage, nullptr);
    vkFreeMemory(device, texture.textureImageMemory, nullptr);

    auto bufferManager = BufferManager::GetInstance();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device, bufferManager->GetUniformBuffers()[i], nullptr);
        vkFreeMemory(device, bufferManager->GetUniformBuffersMemory()[i], nullptr);
    }
    vkDestroyDescriptorPool(device, descriptors.GetDescriptorPool(), nullptr);
    vkDestroyDescriptorSetLayout(device, descriptors.GetDescriptorSetLayout(), nullptr);

    // vkDestroyBuffer(device, bufferManager->indexBuffer, nullptr);
    // vkFreeMemory(device, bufferManager->indexBufferMemory, nullptr);
    // 
    // vkDestroyBuffer(device, bufferManager->vertexBuffer, nullptr);
    // vkFreeMemory(device, bufferManager->vertexBufferMemory, nullptr);

    // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //     vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
    //     vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    //     vkDestroyFence(device, inFlightFences[i], nullptr);
    // }

    //vkDestroyCommandPool(device, commandPool, nullptr);

    //vkDestroyPipeline(device, graphicsPipeline., nullptr);
    //vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    //vkDestroyRenderPass(device, renderPass, nullptr);


    //vkDestroyDevice(device, nullptr);

    //if (enableValidationLayers) {
    //    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    //}

    //vkDestroySurfaceKHR(instance, window.GetSurface(), nullptr);
    //vkDestroyInstance(instance, nullptr);

    //glfwDestroyWindow(window.GetWindowRef());
    //
    //glfwTerminate();
}

void VulkanInstance::CreateInstance() {
    if (enableValidationLayers && !debugMessenger.CheckValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        debugMessenger.PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

std::vector<const char*> VulkanInstance::GetRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

VkInstance& VulkanInstance::GetVulkanInstance() {

    return instance;
}

