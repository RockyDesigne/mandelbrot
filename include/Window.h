//
// Created by HORIA on 17.02.2023.
//

#ifndef SFML_PROJECT_WINDOW_H
#define SFML_PROJECT_WINDOW_H

#include "Mandelbrot.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <sstream>

class Window {
private:
    // window
    sf::RenderWindow* m_window {};
    sf::Vector2i m_screen {};
    sf::Event m_event {};

    // mandelbrot
    sf::Image m_image {};
    sf::Texture m_texture {};
    sf::Sprite m_sprite {};

    // zoom
    int m_scaleFactor {};
    long double m_zoomFactor {};

    // text
    sf::Font m_font {};
    sf::Text m_text {};

    // private functions
    void init_variables();
    void init_window();
    void init_image();
    void init_texture();

public:
    // constructors
    explicit Window(int width = 1920, int height = 1080);

    // destructor
    ~Window();

    // public functions
    [[nodiscard]] bool is_running() const;

    void load_font();

    void set_text();

    void update_text(Mandelbrot& mandelbrot);

    void update_texture(Mandelbrot& mandelbrot);

    void update_sprite();

    void poll_events(Mandelbrot& mandelbrot);

    void update(Mandelbrot& mandelbrot);

    void render(Mandelbrot& mandelbrot);

    void handle_mouse_event(const sf::Event::MouseButtonEvent &mouseEvent, Mandelbrot &mandelbrot) const;

    static void adjust_max_iterations(Mandelbrot &mandelbrot, int delta, double scaleFactor);

    static void handle_key_press_event(const sf::Event &event, Mandelbrot &mandelbrot);
};

#endif //SFML_PROJECT_WINDOW_H
