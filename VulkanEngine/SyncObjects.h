#pragma once

#include "Defs.h"

class SyncObjects
{
public:

    void CreateSyncObjects();

    std::vector<VkSemaphore>& GetImageAvailableSemaphores();
    void  SetImageAvailableSemaphores(std::vector<VkSemaphore>& in_imageAvaibleSemaphoren);

    std::vector<VkSemaphore>& GetRenderFinishedSemaphores();
    void SetRenderFinishedSemaphores(std::vector<VkSemaphore>& in_renderFinishedSemaphores);

    std::vector<VkFence>& GetInFlightFences();
    void SetInFlightFences(std::vector<VkFence>& in_inFlightFences);

private:

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

};

