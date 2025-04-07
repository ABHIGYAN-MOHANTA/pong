#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;

class Ball
{
private:
    Vector2f m_Position;
    CircleShape m_Shape;
    float m_BaseRadius = 10.0f;
    float m_PulseFactor = 0.0f;
    float m_PulseSpeed = 3.0f;
    std::vector<CircleShape> m_Trail;
    std::vector<Vector2f> m_TrailPositions;
    float m_Speed = 3000.0f;
    float m_DirectionX = 0.2f;
    float m_DirectionY = 0.2f;

public:
    Ball(float startX, float startY);
    FloatRect getPosition();
    CircleShape getShape();
    const std::vector<CircleShape>& getTrail() const;
    float getXVelocity();
    void reboundSides();
    void reboundTop();
    void reboundBat(const FloatRect &batBounds);
    void reboundBottom();
    void update(Time dt);
};
