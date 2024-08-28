#pragma once

#include "Defs.h"
#include "ValidationLayers.h"
#include "Window.h"
#include "SwapChain.h"
#include "Image.h"
#include "GraphicsPipeline.h"
#include "Descriptors.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "ModelManager.h"
#include "Renderer.h"
#include "SceneDescription.h"


class VulkanInstance
{
public:
	
	static VulkanInstance* GetInstance();

	VkInstance& GetVulkanInstance();

	void Run();

	VulkanInstance();
	~VulkanInstance();

private:

	void InitVulkan();
	void MainLoop();
	void Cleanup();
	void CreateInstance();
	void InitImGui(GLFWwindow* window, VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);
	Scene scene;

	std::vector<const char*> GetRequiredExtensions();

	VkInstance instance;
	static VulkanInstance* objInstance;

	Window window;
	ValidationLayers debugMessenger;
	SwapChainManager* swapChain;

};

