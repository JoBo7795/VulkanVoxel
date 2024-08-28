#include "VulkanInstance.h"

int main() {

    VulkanInstance app;

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        system("Pause");
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}