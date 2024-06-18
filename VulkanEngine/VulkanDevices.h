#pragma once

#include "Defs.h"
#include "VulkanQueues.h"
#include "SwapChain.h"


class VulkanDevices
{
public:
	VkPhysicalDevice& PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
	void CreateLogicalDevice(VkSurfaceKHR surface);
	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	void SetPhysicalDevice(VkPhysicalDevice in_device);
	VkPhysicalDevice& GetPhysicalDevice();

	void SetDevice(VkDevice in_device);
	VkDevice& GetDevice();

	VulkanDevices();
	~VulkanDevices();

	static VulkanDevices* GetInstance();

private:
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	static VulkanDevices* instance;


};