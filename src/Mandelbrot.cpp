//
// Created by HORIA on 17.02.2023.
//
#include "Mandelbrot.h"

void Mandelbrot::init_variables() {
    m_image.create(m_width, m_height);
    if (!m_texture.create(m_width, m_height))
        assert("Failed to create texture");
}

sf::Color Mandelbrot::interpolate_color(double colorIndex, const std::vector<sf::Color>& colors) {

    // Determine the maximum color index based on the number of colors in the provided vector
    const auto maxColor {colors.size() - 1};

    // Determine the maximum color index based on the number of colors in the provided vector
    colorIndex *= maxColor;

    // Determine the lower index by casting down the color index to the nearest integer
    auto lowerIndex {static_cast<size_t>(colorIndex)};

    // Get the colors corresponding to the lower and upper color indices
    auto color1 {colors[lowerIndex]};
    auto color2 {colors[std::clamp(lowerIndex + 1, size_t{0}, colors.size() - 1)]};

    // Interpolate the color between the two colors based on the color index and the lower index
    return linear_interp(color1, color2, colorIndex - lowerIndex);
}

void Mandelbrot::set_color(int iters, int x, int y) {

    // Define a vector of colors to use for coloring the Mandelbrot set
    const std::vector<sf::Color> colors {
            {0,0,0},
            {255,0,0},
            {255,127,0},
            {255,255,0},
            {0,255,0},
            {0,0,255},
            {75,0,130},
            {148,0,211},
            {255,0,255},
            {255,255,255}
    };

    // If the number of iterations is equal to the maximum number of iterations, the point is considered to be in the set
    // and is assigned the default color (black)
    if (iters == m_maxIterations) {
        iters = 0;
    }

    // Calculate the value of mu, which is a value between 0 and 1 that is used to determine the color of the point
    double mu {1.0 * iters / m_maxIterations};

    // Calculate the value of mu, which is a value between 0 and 1 that is used to determine the color of the point
    auto color {interpolate_color(mu, colors)};

    // Set the color of the pixel at (x, y) in the Mandelbrot image
    m_image.setPixel(x, y, color);
}

/**
 * Generate the Mandelbrot set and store it in the object's color buffer.
 *
 * @param screen The size of the output screen.
 */
void Mandelbrot::mandy(sf::Vector2i screen) {

    using CoordType = long double;

    // OpenMP parallelize this loop to utilize multiple threads
#pragma omp parallel for default(none) shared(screen)

    // Iterate over the screen's pixels
    for (int y = 0; y < screen.y; ++y) {
        for (int x = 0; x < screen.x; ++x) {

            // Calculate the coordinates of the current pixel on the complex plane
            CoordType realCoord {m_minRe + (m_maxRe - m_minRe) * x / screen.x};
            CoordType imagCoord {m_minIm + (m_maxIm - m_minIm) * y / screen.y};

            // Initialize the real and imaginary parts of the complex number to 0
            CoordType realComponent {0.0}, imagComponent {0.0};
            int iters {};

            // Perform the iterative calculation to determine whether the current point
            // is in the Mandelbrot set or not
            for (iters = 0; iters < m_maxIterations; ++iters) {

                // Calculate the next point in the sequence
                CoordType tr {realComponent * realComponent - imagComponent * imagComponent + realCoord};
                imagComponent = 2 * realComponent * imagComponent + imagCoord;
                realComponent = tr;

                // If the point is outside the circle of radius 2, exit the loop early
                if (realComponent * realComponent + imagComponent * imagComponent > 2 * 2) {
                    break;
                }
            }
            // Set the color of the current pixel based on the number of iterations
            set_color(iters, x, y);
        }
    }
}

sf::Image Mandelbrot::get_image() {
    return m_image;
}

long double Mandelbrot::get_min_re() const {
    return m_minRe;
}

void Mandelbrot::set_min_re(long double minRe) {
    m_minRe = minRe;
}

// member initialization list
Mandelbrot::Mandelbrot() : m_width {1920}, m_height {1080}, m_maxIterations{128},
    m_minRe {-2.5}, m_maxRe {1.0}, m_minIm {-1.0}, m_maxIm {1.0}, m_zoom {1.0}
{
    init_variables();
}

void Mandelbrot::set_max_re(long double maxRe) {
    m_maxRe = maxRe;
}

void Mandelbrot::set_min_im(long double minIm) {
    m_minIm = minIm;
}

void Mandelbrot::set_max_im(long double maxIm) {
    m_maxIm = maxIm;
}

long double Mandelbrot::get_max_re() const {
    return m_maxRe;
}

long double Mandelbrot::get_min_im() const {
    return m_minIm;
}

long double Mandelbrot::get_max_im() const {
    return m_maxIm;
}

void Mandelbrot::set_max_iterations(int maxIterations) {
    m_maxIterations = maxIterations;
}

int Mandelbrot::get_max_iterations() const {
    return m_maxIterations;
}

/**
 * Interpolates between two colors using linear interpolation.
 *
 * @param color1 the first color
 * @param color2 the second color
 * @param ratio the interpolation factor (a value between 0 and 1)
 * @return the interpolated color
 */
sf::Color Mandelbrot::linear_interp(const sf::Color& color1, const sf::Color& color2, double ratio) {

    // calculate the complement of the interpolation factor
    double inverseRatio = 1.0 - ratio;

    // interpolate the RGB values of the colors using the given factor
    auto red = static_cast<sf::Uint8>(inverseRatio * color1.r + ratio * color2.r);
    auto green = static_cast<sf::Uint8>(inverseRatio * color1.g + ratio * color2.g);
    auto blue = static_cast<sf::Uint8>(inverseRatio * color1.b + ratio * color2.b);

    // return the interpolated color
    return {red, green, blue};
}


long double Mandelbrot::get_zoom() const {
    return m_zoom;
}

void Mandelbrot::set_zoom(long double zoom) {
    m_zoom = zoom;
}
