#pragma once

#include "Defs.h"


class Window
{
public:

	void InitWindow();
	void CreateSurface(VkInstance& instance);
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	bool GetFramebufferResized();
	VkSurfaceKHR& GetSurface();
	static GLFWwindow* GetWindowRef();

	bool GetFrameBufferResized();
	void SetFrameBufferResized(bool in_framebufferResized);
	void CleanUp();

	Window();
	~Window();

private:

	Window* instance;
	static GLFWwindow* windowRef;
	static VkInstance vulkanInstanceRef;
	VkSurfaceKHR surface;

	static bool framebufferResized;
};

