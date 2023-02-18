//
// Created by HORIA on 17.02.2023.
//
#include "Window.h"

void Window::init_variables() {
    this->m_window = nullptr;
}

void Window::init_window(int width, int height) {
    this->m_screen.x = width;
    this->m_screen.y = height;
    this->scaleFactor = 2;
    this->m_zoomFactor = 5.0;
    this->m_window = new sf::RenderWindow(sf::VideoMode(m_screen.x, m_screen.y), "Mandelbrot",
                                          sf::Style::Titlebar | sf::Style::Close);
    this->m_window->setFramerateLimit(60);
}

// constructor
Window::Window(int width, int height) {
    this->init_variables();
    this->load_font();
    this->set_text();
    this->init_window(width, height);
    this->init_image();
    this->init_texture();
}

// default constructor has default size of 1920 * 1080
Window::Window() {
    this->init_variables();
    this->load_font();
    this->set_text();
    this->init_window(1920, 1080);
    this->init_image();
    this->init_texture();
}

// destructor
// needed to deallocate memory for window
Window::~Window() {
    delete this->m_window;
}

bool Window::is_running() const {
    return this->m_window->isOpen();
}

void Window::handle_mouse_event(const sf::Event::MouseButtonEvent& mouseEvent, Mandelbrot& mandelbrot) const
{
    using PrecisionType = long double;

    auto zoomAtMousePosition = [&](long double zoomFactor, double mouseX, double mouseY) {
        PrecisionType minReal = mandelbrot.get_min_re();
        PrecisionType maxReal = mandelbrot.get_max_re();
        PrecisionType minImaginary = mandelbrot.get_min_im();
        PrecisionType maxImaginary = mandelbrot.get_max_im();

        PrecisionType centerX = minReal + (maxReal - minReal) * mouseX / m_screen.x;
        PrecisionType centerY = minImaginary + (maxImaginary - minImaginary) * mouseY / m_screen.y;

        PrecisionType newWidth = (maxReal - minReal) / zoomFactor;
        PrecisionType newHeight = (maxImaginary - minImaginary) / zoomFactor;
        mandelbrot.set_max_re(centerX + newWidth / this->scaleFactor);
        mandelbrot.set_min_re(centerX - newWidth / this->scaleFactor);
        mandelbrot.set_max_im(centerY + newHeight / this->scaleFactor);
        mandelbrot.set_min_im(centerY - newHeight / this->scaleFactor);
    };

    if (mouseEvent.button == sf::Mouse::Left) {
        zoomAtMousePosition(this->m_zoomFactor, mouseEvent.x, mouseEvent.y);
        mandelbrot.set_zoom(mandelbrot.get_zoom() * this->m_zoomFactor);
    }
    else if (mouseEvent.button == sf::Mouse::Right) {
        zoomAtMousePosition(1.0 / this->m_zoomFactor, mouseEvent.x, mouseEvent.y);
        mandelbrot.set_zoom(mandelbrot.get_zoom() / this->m_zoomFactor);
    }
}

void Window::adjust_max_iterations(Mandelbrot& mandelbrot, int delta, double scaleFactor) {
    if (delta > 0) {
        mandelbrot.set_max_iterations(mandelbrot.get_max_iterations() * scaleFactor);
    } else {
        mandelbrot.set_max_iterations(mandelbrot.get_max_iterations() / scaleFactor);
    }

    const int minIterations {1};
    if (mandelbrot.get_max_iterations() < minIterations) {
        mandelbrot.set_max_iterations(minIterations);
    }
}

void Window::handle_key_press_event(const sf::Event& event, Mandelbrot& mandelbrot)
{
    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    using PrecisionType = long double;

    const PrecisionType maxRe = mandelbrot.get_max_re();
    const PrecisionType minRe = mandelbrot.get_min_re();
    const PrecisionType maxIm = mandelbrot.get_max_im();
    const PrecisionType minIm = mandelbrot.get_min_im();

    const PrecisionType width = (maxRe - minRe) * 0.3;
    const PrecisionType height = (maxIm - minIm) * 0.3;

    if (event.key.code == sf::Keyboard::Left) {
        mandelbrot.set_min_re(minRe - width);
        mandelbrot.set_max_re(maxRe - width);
    } else if (event.key.code == sf::Keyboard::Right) {
        mandelbrot.set_min_re(minRe + width);
        mandelbrot.set_max_re(maxRe + width);
    } else if (event.key.code == sf::Keyboard::Up) {
        mandelbrot.set_min_im(minIm - height);
        mandelbrot.set_max_im(maxIm - height);
    } else if (event.key.code == sf::Keyboard::Down) {
        mandelbrot.set_min_im(minIm + height);
        mandelbrot.set_max_im(maxIm + height);
    }
}


void Window::poll_events(Mandelbrot& mandelbrot) {
    while (this->m_window->pollEvent(this->m_event))
        switch (this->m_event.type) {
            case sf::Event::Closed:
                this->m_window->close();
                break;
            case sf::Event::KeyPressed:
                if (this->m_event.key.code == sf::Keyboard::Escape)
                    this->m_window->close();

                if (this->m_event.type == sf::Event::KeyPressed) {
                    handle_key_press_event(this->m_event, mandelbrot);
                }
                break;

            case sf::Event::MouseWheelScrolled:
                if (m_event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    adjust_max_iterations(mandelbrot, m_event.mouseWheelScroll.delta, this->scaleFactor);
                }
                break;

        case sf::Event::MouseButtonPressed:
            handle_mouse_event(this->m_event.mouseButton, mandelbrot);
            break;
        }
}

void Window::update(Mandelbrot& mandelbrot) {
    this->poll_events(mandelbrot);
}

void Window::render(Mandelbrot &mandelbrot) {
    this->m_window->clear();

    // calculate mandelbrot
    mandelbrot.mandy(this->m_screen);

    // update texture
    this->update_texture(mandelbrot);
    this->update_sprite();
    this->m_window->draw(this->m_sprite);

    // update text
    this->update_text(mandelbrot);

    this->m_window->draw(this->m_text);

    this->m_window->display();
}

void Window::load_font() {
    if (!this->m_font.loadFromFile("resources/ArialTh.ttf"))
        assert("Failed to load font");
}

void Window::set_text() {
    this->m_text.setFont(this->m_font);
    this->m_text.setCharacterSize(60);
    this->m_text.setFillColor(sf::Color::White);
    this->m_text.setPosition(10, 10);
}

void Window::update_text(Mandelbrot& mandelbrot) {
    std::ostringstream oss;
    oss << "Iterations: " << mandelbrot.get_max_iterations() << "\n";
    oss << "Zoom Factor: " << mandelbrot.get_zoom() << "\n";
    this->m_text.setString(oss.str());
}

void Window::init_image() {
    this->m_image.create(this->m_screen.x, this->m_screen.y);
}

void Window::init_texture() {
    if (!this->m_texture.create(this->m_screen.x, this->m_screen.y))
        assert("Failed to create texture");
}

void Window::update_sprite() {
    this->m_sprite.setTexture(this->m_texture);
}

void Window::update_texture(Mandelbrot& mandelbrot) {
    this->m_texture.loadFromImage(mandelbrot.get_image());
}
