#include "SyncObjects.h"
#include "VulkanDevices.h"

void SyncObjects::CreateSyncObjects() {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();

    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

}

std::vector<VkSemaphore>& SyncObjects::GetImageAvailableSemaphores(){
    return imageAvailableSemaphores;
}

void SyncObjects::SetImageAvailableSemaphores(std::vector<VkSemaphore>& in_imageAvaibleSemaphoren){
    imageAvailableSemaphores = in_imageAvaibleSemaphoren;
}

std::vector<VkSemaphore>& SyncObjects::GetRenderFinishedSemaphores() {
    return renderFinishedSemaphores;
}

void SyncObjects::SetRenderFinishedSemaphores(std::vector<VkSemaphore>& in_renderFinishedSemaphores) {
    renderFinishedSemaphores = in_renderFinishedSemaphores;
}


std::vector<VkFence>& SyncObjects::GetInFlightFences() {
    return inFlightFences;
}

void SyncObjects::SetInFlightFences(std::vector<VkFence>& in_inFlightFences) {
    inFlightFences = in_inFlightFences;
}

