#include "Window.h"

GLFWwindow* Window::windowRef;
bool Window::framebufferResized = false;
VkInstance Window::vulkanInstanceRef;

Window::Window() {
    InitWindow();
}

Window::~Window() {
    

}

void Window::CleanUp(){

    vkDestroySurfaceKHR(vulkanInstanceRef, surface, nullptr);
    vkDestroyInstance(vulkanInstanceRef, nullptr);

    glfwDestroyWindow(windowRef);
    glfwTerminate();
}

void Window::CreateSurface(VkInstance& instance) {

    if (glfwCreateWindowSurface(instance,
        windowRef,
        nullptr,
        &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    vulkanInstanceRef = instance;
}

void Window::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    windowRef = glfwCreateWindow(WIDTH, HEIGHT, "VulkanVoxelEngine", nullptr, nullptr);
    glfwSetWindowUserPointer(windowRef, this);
    glfwSetFramebufferSizeCallback(windowRef, FramebufferResizeCallback);
}

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

bool Window::GetFramebufferResized() {

    return framebufferResized;
}

VkSurfaceKHR& Window::GetSurface() {

    return surface;
}

GLFWwindow* Window::GetWindowRef() {

    return windowRef;
}

bool Window::GetFrameBufferResized() {
    return framebufferResized;
}

void Window::SetFrameBufferResized(bool in_framebufferResized) {
    framebufferResized = in_framebufferResized;
}