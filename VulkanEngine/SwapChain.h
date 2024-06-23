#pragma once
#include "Defs.h"
#include "VulkanQueues.h"
#include "Window.h"
#include "DepthRessources.h"
#include "Texture.h"
#include "GraphicsPipeline.h"


class SwapChainManager
{

public:

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR& surface, VkPhysicalDevice physDevice);

	void CreateSwapChain(VkSurfaceKHR surface);
	void CleanupSwapChain();
	void RecreateSwapChain(Window& window, VkRenderPass& renderPass);
	void CreateImageViews();
	void CreateDepthRessources();
	void CreateFrameBufferRessources(VkRenderPass& renderPass);
	void CreateGraphicsPipeline();

	VkSwapchainKHR GetSwapChain();
	void SetSwapChain(VkSwapchainKHR swapChain);
	const std::vector<VkImage>& GetSwapChainImages();
	void SetSwapChainImages(const std::vector<VkImage>& images);
	VkFormat GetSwapChainImageFormat();
	void SetSwapChainImageFormat(VkFormat format);
	VkExtent2D GetSwapChainExtent();
	void SetSwapChainExtent(VkExtent2D extent);
	const std::vector<VkImageView>& GetSwapChainImageViews();
	void SetSwapChainImageViews(const std::vector<VkImageView>& views);
	void SetDepthRessources(DepthRessources& in_depthRessources);


	std::vector<VkFramebuffer>& GetSwapChainFramebuffers();
	void SetSwapChainFramebuffers(std::vector<VkFramebuffer>& framebuffers);


	static SwapChainManager* GetInstance();

private:

	SwapChainManager();
	~SwapChainManager();
	
	static SwapChainManager* instance;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	DepthRessources depthRessources;
	std::vector<VkFramebuffer> swapChainFramebuffers;


};

