#include <filesystem>
#include "Window.h"

static void modifyCurrentWorkingDirectory();

int main () {

    modifyCurrentWorkingDirectory();

    // create mandelbrot
    Mandelbrot mandelbrot {};

    // create window
    Window window{1920, 1080};

    while (window.is_running()) {
        window.update(mandelbrot);

        window.render(mandelbrot);
    }

    return 0;
}

static void modifyCurrentWorkingDirectory()
{
    while (!std::filesystem::exists("resources"))
    {
        std::filesystem::current_path(std::filesystem::current_path().parent_path());
    }
    auto cwd = std::filesystem::current_path();
}
