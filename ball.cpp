#include "ball.hpp"
#include <cmath>

Ball::Ball(float startX, float startY)
{
    m_Position.x = startX;
    m_Position.y = startY;
    
    m_Shape.setRadius(m_BaseRadius);
    m_Shape.setFillColor(sf::Color(255, 100, 100));
    m_Shape.setOutlineThickness(2);
    m_Shape.setOutlineColor(sf::Color(255, 200, 200));
    m_Shape.setOrigin(m_BaseRadius, m_BaseRadius);
    m_Shape.setPosition(m_Position);

    // Initialize trail pieces and their positions
    for (int i = 0; i < 10; i++) {
        sf::CircleShape trailPiece(m_BaseRadius);
        trailPiece.setFillColor(sf::Color(255, 0, 0, 255 - i * 20));
        trailPiece.setOutlineThickness(2);
        trailPiece.setOutlineColor(sf::Color(255, 100, 100, 200 - i * 15));
        m_Trail.push_back(trailPiece);
        m_TrailPositions.push_back(m_Position);
    }
}

FloatRect Ball::getPosition()
{
    return m_Shape.getGlobalBounds();
}

sf::CircleShape Ball::getShape()
{
    return m_Shape;
}

const std::vector<sf::CircleShape>& Ball::getTrail() const
{
    return m_Trail;
}

float Ball::getXVelocity()
{
    return m_DirectionX;
}

/// Adjust left/right collisions so that the ball is fully within bounds
void Ball::reboundSides()
{
    sf::FloatRect bounds = m_Shape.getGlobalBounds();
    if (bounds.left < 0)
    {
        m_Position.x = bounds.width / 2;
        m_Shape.setPosition(m_Position);
    }
    else if (bounds.left + bounds.width > 1920)
    {
        m_Position.x = 1920 - bounds.width / 2;
        m_Shape.setPosition(m_Position);
    }
    m_DirectionX = -m_DirectionX;
}

/// Adjust top collisions so that the ball is fully in view
void Ball::reboundTop()
{
    m_DirectionY = -m_DirectionY;
    sf::FloatRect bounds = m_Shape.getGlobalBounds();
    if (bounds.top < 0)
    {
        m_Position.y = bounds.height / 2;
        m_Shape.setPosition(m_Position);
    }
}

/// Adjust bat collision so that the ball's bottom touches the bat's top
void Ball::reboundBat(const sf::FloatRect &batBounds)
{
    // Force upward movement
    m_DirectionY = -std::abs(m_DirectionY);
    sf::FloatRect bounds = m_Shape.getGlobalBounds();
    m_Position.y = batBounds.top - (bounds.height / 2);
    m_Shape.setPosition(m_Position);
}

/// For bottom collisions, reset position and reverse vertical direction
void Ball::reboundBottom()
{
    m_Position.y = 0;
    m_Position.x = 500;
    m_DirectionY = -m_DirectionY;
    m_Shape.setPosition(m_Position);
}

void Ball::update(sf::Time dt)
{
    // Update pulse effect (visual only) using scaling
    m_PulseFactor += m_PulseSpeed * dt.asSeconds();
    float scale = 1.0f + std::sin(m_PulseFactor) * 0.15f;
    m_Shape.setScale(scale, scale);
    
    // Update ball position based on current velocity
    m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();
    m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
    m_Shape.setPosition(m_Position);
    
    // Update trail by storing previous positions
    m_TrailPositions.insert(m_TrailPositions.begin(), m_Position);
    if (m_TrailPositions.size() > m_Trail.size())
        m_TrailPositions.pop_back();
    for (size_t i = 0; i < m_Trail.size(); i++) {
        m_Trail[i].setPosition(m_TrailPositions[i]);
    }
}
