#include <filesystem>
#include "Mandelbrot.h"
#include "Window.h"

static void modifyCurrentWorkingDirectory();

int main () {

    modifyCurrentWorkingDirectory();

    // create mandelbrot
    Mandelbrot mandelbrot {};

    // create window
    Window window{};

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
