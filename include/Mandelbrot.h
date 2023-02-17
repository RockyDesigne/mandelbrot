//
// Created by HORIA on 16.02.2023.
//

#ifndef SFML_PROJECT_MADNELBROT_H
#define SFML_PROJECT_MADNELBROT_H

#include <SFML/Graphics.hpp>
#include <cassert>

class Mandelbrot {
private:
    using PrecisionType = long double;

    sf::Image m_image {};
    sf::Texture m_texture {};
    sf::Sprite m_sprite {};

    int m_width {};
    int m_height {};
    long double m_zoom {};

    PrecisionType m_minRe {};
    PrecisionType m_maxRe {};

    PrecisionType m_minIm {};
    PrecisionType m_maxIm {};

    int m_maxIterations {};


    // private functions
    void init_variables();

public:
    Mandelbrot();

    // public functions
    void mandy(sf::Vector2i screen);

    //accessor functions

    // setters
    void set_color(int iters, int x, int y);

    void set_zoom(long double zoom);

    void set_min_re(long double minRe);

    void set_max_re(long double maxRe);

    void set_min_im(long double minIm);

    void set_max_im(long double maxIm);

    void set_max_iterations(int maxIterations);

    // getters
    long double get_zoom() const;

    long double get_min_re() const;

    long double get_max_re() const;

    long double get_min_im() const;

    long double get_max_im() const;

    int get_max_iterations() const;

    sf::Image get_image();

    // interpolation functions
    static sf::Color linear_interp(const sf::Color& color1, const sf::Color& color2, double ratio);

    sf::Color interpolate_color(double colorIndex, const std::vector<sf::Color> &colors);
};

#endif //SFML_PROJECT_MADNELBROT_H
