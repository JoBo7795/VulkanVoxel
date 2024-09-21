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

    scene.LoadRessources();
    scene.SceneDescription();

    InitVulkan();   



}

VulkanInstance::~VulkanInstance() {

    Cleanup();
    delete objInstance;

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
    
    InitImGui(window.GetWindowRef(), instance,VulkanDevices::GetInstance()->GetDevice(), VulkanDevices::GetInstance()->GetPhysicalDevice(),VulkanQueueManager::GetInstance()->GetGraphicsQueue());
}

void VulkanInstance::InitImGui(GLFWwindow* window, VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue) {
    // ImGui Kontext erstellen 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    Renderer* rendererInstance = Renderer::GetInstance();

    std::vector<VkDescriptorPoolSize> pool_sizes =
    {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1;
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(&pool_sizes);
    pool_info.pPoolSizes = pool_sizes.data();


    auto descriptorInstance = Descriptors::GetInstance();
    size_t index = descriptorInstance->CreateDescriptorPool(pool_info, pool_sizes);

    if (vkCreateDescriptorPool(device, &pool_info, nullptr, &Descriptors::GetInstance()->GetDescriptorPool(index)) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance;
    init_info.PhysicalDevice = physicalDevice;
    init_info.Device = device;
    init_info.Queue = graphicsQueue;
    init_info.DescriptorPool = rendererInstance->GetGraphicsPipeline().GetDescriptors().GetDescriptorPool(index);
    init_info.MinImageCount = 2;
    init_info.ImageCount = 4;
    init_info.RenderPass = rendererInstance->GetGraphicsPipeline().GetRenderPass();
    ImGui_ImplVulkan_Init(&init_info);
}

void VulkanInstance::MainLoop() {

    glm::vec3 voxelIndex = glm::vec3(1,1,1);
    int voxelVal = 0;
    auto gOManagerRef = GameObjectManager::GetInstance();

    while (!glfwWindowShouldClose(window.GetWindowRef())) {
        glfwPollEvents();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        {
            ImGui::NewFrame();
            //ImGui::Begin("Voxel");
            //ImGui::Text("Select Voxel Index");
            //ImGui::InputFloat3("VoxelIndex", &voxelIndex[0]);
            //ImGui::InputInt("VoxelType", &voxelVal);
            //if (ImGui::Button("Set VoxelType")) {
            //    scene.ChangeVoxelAtIndex(voxelIndex, voxelVal);
            //}
            //
            //// Array von Strings, das den Enum-Werten entspricht
            //const char* cubeSides[] = { "FRONT", "BACK", "LEFT", "RIGHT", "TOP", "BOTTOM" };
            //static int currentSide = FRONT; // Index des aktuell ausgewählten Seiten
            //
            //if (ImGui::BeginCombo("Cube Sides", cubeSides[currentSide]))
            //{
            //    for (int n = 0; n < IM_ARRAYSIZE(cubeSides); n++)
            //    {
            //        bool is_selected = (currentSide == n);
            //        if (ImGui::Selectable(cubeSides[n], is_selected)) {
            //            currentSide = n;
            //        }
            //        if (is_selected)
            //            ImGui::SetItemDefaultFocus();
            //    }
            //    ImGui::EndCombo();
            //}
            //
            //if (ImGui::Button("Add Voxel at CubeSide")) {
            //    scene.voxelMesh.AddCubeToCubeSide(voxelIndex, currentSide);
            //}
            //
            //ImGui::End();

            ImGui::Begin("Light");
            ImGui::InputFloat3("LightPosition", &voxelIndex[0]);
            ImGui::End();
        }

        // Hole die aktuelle Zeichnungsliste für den Hintergrund
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

        // Bestimme die Mitte des Bildes
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);

        // Zeichne einen Punkt in der Mitte des Bildes
        draw_list->AddCircle(center, 1.0f, IM_COL32(255, 0, 0, 255), 12, 2.0f);

        ImGui::Render();
        
        gOManagerRef->UpdateGameObjectUBOs();

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
    appInfo.pEngineName = "Voxel Engine";
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

