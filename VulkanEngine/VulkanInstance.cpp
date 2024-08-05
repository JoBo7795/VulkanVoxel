#include "VulkanInstance.h"
#include "VulkanDevices.h"
#include "CommandPool.h"

VulkanInstance* VulkanInstance::objInstance = nullptr;

VulkanInstance::VulkanInstance() {

    window.InitWindow();    
    CreateInstance();
    debugMessenger = ValidationLayers(instance);     
    window.CreateSurface(instance);
    window.SetCallback(KEY_INPUT);
    window.SetCallback(MOUSE_INPUT);
    window.SetCallback(SCROLL_INPUT);
    
    VulkanDevices::GetInstance()->PickPhysicalDevice(instance,window.GetSurface());
    VulkanDevices::GetInstance()->CreateLogicalDevice(window.GetSurface());
    swapChain = SwapChainManager::GetInstance();
    swapChain->CreateSwapChain(window.GetSurface());
    swapChain->CreateImageViews();
    CommandPoolManager::GetInstance()->CreateCommandPool(window.GetSurface());

    Scene::LoadRessources();
    Scene::SceneDescription();

    for (int i = 0; i < GameObjectManager::GetInstance()->GetGameObjectQueueSize(); i++) {
        BufferManager::GetInstance()->CreateUniformBuffers();
    }


    InitVulkan();    
    
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

    MainLoop();

}

void VulkanInstance::InitVulkan() {

    Renderer* rendererInstance = Renderer::GetInstance();

    Camera camera = Camera(glm::vec3(1.0f, 0.0f, 0.0f));
    
    rendererInstance->SetCamera(camera);
    rendererInstance->InitRenderer(window);
}


void VulkanInstance::MainLoop() {

    while (!glfwWindowShouldClose(window.GetWindowRef())) {
        glfwPollEvents();

        Renderer::GetInstance()->Render();
        
    }

    vkDeviceWaitIdle(VulkanDevices::GetInstance()->GetDevice());
}

void VulkanInstance::Cleanup() {

    debugMessenger.DestroyDebugUtilsMessengerEXT(instance,nullptr);

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

