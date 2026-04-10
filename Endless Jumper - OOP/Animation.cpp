#include <cmath>
#include <iostream>

#include "Animation.h"

Animation::Animation() = default;

Animation::Animation(const std::string& name, const sf::Texture& t)
    : Animation(name, t, 1, 0) {}

Animation::Animation(const std::string& name, const sf::Texture& t,
                     size_t frameCount, size_t speed)
    : m_name(name),
      m_sprite(t),
      m_frameCount(frameCount),
      m_currentFrame(0),
      m_speed(speed) {
  m_size = Vec2((float)(t.getSize().x) / frameCount, (float)t.getSize().y);
  m_sprite.setOrigin(m_size.x / 2.f, m_size.y / 2.f);
  m_sprite.setTextureRect(
      sf::IntRect(floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
  if (name == "background1" || name == "background2" || name == "background3" ||
      name == "background4" || name == "background5" || name == "background6") {
    m_sprite.setScale(6, 6);
  }
}

// updates the animation to show the next frame, depending on it's speed
// animation loops when it reaches the end
void Animation::update() {
  // add the speed variable to the current frame
  m_currentFrame++;
  if (m_speed > 0) {
    m_sprite.setTextureRect(
        sf::IntRect(floor(m_currentFrame / m_speed % m_frameCount) * m_size.x,
                    0, m_size.x, m_size.y));
    // std::cout << m_currentFrame / m_speed % m_frameCount << std::endl;
  }
  // TODO: 1) calculate the correct frame of animation to play based on
  // currentFrame and speed
  //		 2) set the texture rectangle properly (see constructor for
  // sample)
}

std::string& Animation::getName() { return m_name; }

Vec2& Animation::getSize() { return m_size; }

sf::Sprite& Animation::getSprite() { return m_sprite; }
