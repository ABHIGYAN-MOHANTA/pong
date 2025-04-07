#include "bat.hpp"
#include "ball.hpp"
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace sf;

int main()
{
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Pong");

    int score = 0;
    int lives = 3;

    Bat bat(1920 / 2, 1080 - 20);
    Ball ball(1920 / 2, 0);

    Text hud;
    Font font;
    font.loadFromFile("fonts/DS-DIGI.TTF");
    hud.setFont(font);
    hud.setCharacterSize(75);
    hud.setFillColor(Color::White);
    hud.setPosition(20, 20);

    Clock clock;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        // Bat movement
        if (Keyboard::isKeyPressed(Keyboard::Left))
            bat.moveLeft();
        else
            bat.stopLeft();

        if (Keyboard::isKeyPressed(Keyboard::Right))
            bat.moveRight();
        else
            bat.stopRight();

        Time dt = clock.restart();
        bat.update(dt);
        ball.update(dt);

        // Update HUD
        std::stringstream ss;
        ss << "Score:" << score << " Lives:" << lives;
        hud.setString(ss.str());

        // Collision handling
        // Top collision
        if (ball.getPosition().top < 0)
        {
            ball.reboundTop();
            score++;
        }
        // Left/right collisions
        if (ball.getPosition().left < 0 ||
            ball.getPosition().left + ball.getPosition().width > window.getSize().x)
        {
            ball.reboundSides();
        }
        // Bottom collision (missed ball)
        if (ball.getPosition().top > window.getSize().y)
        {
            ball.reboundBottom();
            lives--;
            if (lives < 1)
            {
                score = 0;
                lives = 3;
            }
        }
        // Collision with bat
        if (ball.getPosition().intersects(bat.getPosition()))
        {
            ball.reboundBat(bat.getPosition());
        }

        // Draw everything
        window.clear();
        window.draw(hud);
        window.draw(bat.getShape());
        const auto& trail = ball.getTrail();
        for (int i = trail.size()-1; i >= 0; i--)
        {
            window.draw(trail[i]);
        }
        window.draw(ball.getShape());
        window.display();
    }
    return 0;
}
