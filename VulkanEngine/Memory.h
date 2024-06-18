#pragma once

#include "Includes.h"

namespace Memory
{
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
};

