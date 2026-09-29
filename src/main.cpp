#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const int FPA = 90;
static int frame_counter = 0;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
            switch (key->code) {

                //global tween
                case sf::Keyboard::Key::Num1:
                    tween = [](float a, float b, float t) {return (1 - t) * a + t * b;};
                    break;

                    //easeInQuadratic
                case sf::Keyboard::Key::Num2:
                    tween = [](float a, float b, float t) {return (1 - t * t) * a + t * t * b;};
                    break;

                    //easeInSin
                case sf::Keyboard::Key::Num3:
                    tween = [](float a, float b, float t) {return (1 - ((sin((t - 0.5) * std::numbers::pi) + 1) /2)) * a + ((sin((t - 0.5) * std::numbers::pi) + 1) /2) * b;};
                    break;

                    //easeInCos
                case sf::Keyboard::Key::Num4:
                    tween = [](float a, float b, float t) {return (1 - (-1 * (cos(t * std::numbers::pi) - 1) /2)) * a + (-1 * (cos(t * std::numbers::pi) - 1) /2) * b;};
                    break;

                    //EaseOutQuart
                case sf::Keyboard::Key::Num5:
                    tween = [](float a, float b, float t) {
                        float x = 1 - std::pow(1 - t, 4);
                        return (1 - x) * a + x * b;
                    };
                    break;

                    //EaseOutQuad
                case sf::Keyboard::Key::Num6:
                    tween = [](float a, float b, float t) {
                        float x = 1 - (1 - t) * (1 - t);
                        return (1 - x) * a + x * b;
                    };
                    break;

                    //easeInOutCubic
                case sf::Keyboard::Key::Num7:
                    tween = [](float a, float b, float t) {
                        float x = t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
                        return (1 - x) * a + x * b;
                    };
                    break;

                    //easeInOutBack
                case sf::Keyboard::Key::Num8:
                    tween = [](float a, float b, float t) {
                        const float c1 = 1.70158;
                        const float c2 = c1 * 1.525;
                        float x = t < 0.5 ? (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
                                    : (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
                        return (1 - x) * a + x * b;
                    };
                    break;

                    //easeInQuint
                case sf::Keyboard::Key::Num9:
                    tween = [](float a, float b, float t) {
                        float x = t * t * t * t * t;
                        return (1 - x) * a + x * b;
                    };
                    break;
                default:
                    break;
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    float animation_time = (frame_counter % FPA) / static_cast<float>(FPA);
    float pos_y = WINDOW_HEIGHT / 3.0f;
    float pos_x = tween(0, WINDOW_WIDTH, animation_time);
    sf::CircleShape ball(25.f);
    ball.setFillColor(sf::Color::Green);
    ball.setOrigin({50, 50});
    sf::Vector2f ball_pos = sf::Vector2f(pos_x, pos_y);
    ball.setPosition(ball_pos);

    window.draw(ball);
    frame_counter++;
    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    float graph_size_y = WINDOW_HEIGHT / 3.f;
    float graph_size_x = WINDOW_WIDTH -100;
    float margin_side = 80;
    float margin_bot = WINDOW_HEIGHT - 80;

    sf::RectangleShape x_axis({graph_size_x, 5});
    sf::RectangleShape y_axis({5, graph_size_y});
    x_axis.setFillColor(sf::Color::Cyan);
    y_axis.setFillColor(sf::Color::Cyan);
    x_axis.setPosition({margin_side, margin_bot});
    y_axis.setPosition({margin_side, margin_bot - graph_size_y});
    window.draw(x_axis);
    window.draw(y_axis);

    for (int i = 0; i <= WINDOW_WIDTH; i++) {
        float t = i / WINDOW_WIDTH;
        float y = tween(0, 1, t);
        sf::CircleShape point(3);
        point.setPosition({margin_side + t * graph_size_x, margin_bot - y * graph_size_y});
        window.draw(point);
    }

    float z = tween(0, 1, animation_time);
    sf::CircleShape point(15);
    point.setOrigin({15, 15});
    point.setPosition({margin_side + animation_time * graph_size_x, margin_bot - z * graph_size_y});
    point.setFillColor(sf::Color::Magenta);
    window.draw(point);
    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
