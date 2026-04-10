#pragma warning(disable : 4996)

#include <cmath>

#include "GameEngine.h"
#include "Scene_Menu.h"
#include "Scene_Shop.h"

Scene_Shop::Scene_Shop(GameEngine* gameEngine) : Scene(gameEngine) { init(); }

sf::Text Scene_Shop::textConfig(const std::string& stringForText,
                                sf::Vector2f position, int size,
                                sf::Font& font) {
  sf::Text txt;  // Declares and defines the text variable and then loads the
                 // corresponding data

  txt.setString(stringForText);
  txt.setPosition(position);
  txt.setCharacterSize(size);
  txt.setFont(font);
  txt.setFillColor(sf::Color::White);
  sf::FloatRect textRect = txt.getLocalBounds();
  txt.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
  txt.setOutlineThickness(2.0f);
  txt.setOutlineColor(sf::Color::Black);

  return txt;
}
sf::Color Scene_Shop::getDynamicColor(float time) {
  // create color components that change over time using sin functions
  sf::Uint8 red =
      static_cast<sf::Uint8>((std::sin(time * 5.0f) + 1.0f) * 127.5f);
  sf::Uint8 green =
      static_cast<sf::Uint8>((std::sin(time * 3.0f) + 1.0f) * 127.5f);
  sf::Uint8 blue =
      static_cast<sf::Uint8>((std::sin(time * 1.0f) + 1.0f) * 127.5f);
  return sf::Color(red, green, blue);
}

void Scene_Shop::init() {
  auto& w = m_game->window();
  auto& f = m_game->getAssets().getFont("RETROGAMING");
  sf::Vector2u windowSize = w.getSize();

  registerAction(sf::Keyboard::Escape, "QUIT");
  registerAction(sf::Keyboard::Enter, "ENTER");

  // View block
  backgroundView.setCenter(sf::Vector2f(900.0f, 540.0f));
  backgroundView.setSize(sf::Vector2f(1280, 720));

  textView.setCenter(sf::Vector2f(640.0f, 360.0f));
  textView.setSize(sf::Vector2f(1280, 720));

  // Texture block
  backgroundTexture.loadFromFile("Assets/Shop/shopBackground.jpg");
  backgroundTexture.setSmooth(true);

  idleTexture.loadFromFile("Assets/Player/Idle.png");
  sf::Vector2u idleTextureSize = idleTexture.getSize();
  idleTextureSize.x /= 11;

  venom.loadFromFile("Assets/Player/venoso.png");

  // Sprite block
  background.setTexture(backgroundTexture);

  // Player  block
  for (int i = 0; i <= 4; i++) {
    playerIdle[i].setSize(sf::Vector2f(75.f, 110.f));
    playerIdle[i].setOrigin(37.5f, 55.f);
    playerIdle[i].setTexture(&idleTexture);
    playerIdle[i].setTextureRect(
        sf::IntRect(idleTextureSize.x * 10, idleTextureSize.y * 0,
                    idleTextureSize.x, idleTextureSize.y));
  }
  playerIdle[5].setSize(sf::Vector2f(75.f, 100.f));
  playerIdle[5].setOrigin(37.5f, 50.f);
  playerIdle[5].setTexture(&venom);

  playerIdle[1].setFillColor(sf::Color(10, 10, 255, 255));
  playerIdle[2].setFillColor(sf::Color(255, 255, 0, 255));
  playerIdle[3].setFillColor(sf::Color(255, 0, 155, 255));
  playerIdle[4].setFillColor(sf::Color(0, 255, 255, 255));

  playerIdle[0].setPosition(float(windowSize.x / 6), 200.f);
  playerIdle[1].setPosition(float(windowSize.x / 2), 200.f);
  playerIdle[2].setPosition(float(windowSize.x * (5.f / 6.f)), 200.f);
  playerIdle[3].setPosition(float(windowSize.x / 6), 490.f);
  playerIdle[4].setPosition(float(windowSize.x / 2), 490.f);
  playerIdle[5].setPosition(float(windowSize.x * (5.f / 6.f)), 490.f);

  // Font & Text block
  // STORES THE POSITION FOR THE SELECTION HOVER AND THE BUY/EQUIP/EQUIPPED TEXT
  selectPosition.push_back(sf::Vector2f(float(windowSize.x / 6), 300.f));
  selectPosition.push_back(sf::Vector2f(float(windowSize.x / 2), 300.f));
  selectPosition.push_back(
      sf::Vector2f(float(windowSize.x * (5.f / 6.f)), 300.f));
  selectPosition.push_back(sf::Vector2f(float(windowSize.x / 6), 590.f));
  selectPosition.push_back(sf::Vector2f(float(windowSize.x / 2), 590.f));
  selectPosition.push_back(
      sf::Vector2f(float(windowSize.x * (5.f / 6.f)), 590.f));
  // STORES FIXED TEXTS
  texts.push_back(
      textConfig("SPACE SHOP", sf::Vector2f(windowSize.x / 2, 50.f), 32, f));
  texts.push_back(textConfig(
      "DEFAULT", sf::Vector2f(float(windowSize.x / 6), 130.f), 24, f));
  texts.push_back(
      textConfig("GHOST", sf::Vector2f(float(windowSize.x / 2), 130.f), 24, f));
  texts.push_back(textConfig(
      "GOLDEN", sf::Vector2f(float(windowSize.x * (5.f / 6.f)), 130.f), 24, f));
  texts.push_back(textConfig(
      "TWILIGHT", sf::Vector2f(float(windowSize.x / 6), 420.f), 24, f));
  texts.push_back(textConfig(
      "EMERALD", sf::Vector2f(float((windowSize.x / 2)), 420.f), 24, f));
  texts.push_back(textConfig(
      "VENOSO", sf::Vector2f(float(windowSize.x * (5.f / 6.f)), 420.f), 24, f));
  // STORES MALEABLE TEXTS
  texts.push_back(textConfig(" ", selectPosition[0], 24, f));
  texts.push_back(textConfig(" ", selectPosition[1], 24, f));
  texts.push_back(textConfig(" ", selectPosition[2], 24, f));
  texts.push_back(textConfig(" ", selectPosition[3], 24, f));
  texts.push_back(textConfig(" ", selectPosition[4], 24, f));
  texts.push_back(textConfig(" ", selectPosition[5], 24, f));
  texts.push_back(
      textConfig("CURRENCY: A LOT",
                 sf::Vector2f(windowSize.x / 2, windowSize.y * 0.9f), 24, f));

  // Selection block
  select.setSize(sf::Vector2f(150.f, 40.f));
  select.setOrigin(75.f, 20.f);
  select.setPosition(selectPosition[0]);
}

void Scene_Shop::update() {
  m_entityManager.update();
  sRender();
}

void Scene_Shop::onEnd() {
  m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
}

void Scene_Shop::sDoAction(const Action& action) {
  if (action.type() == "START") {
    if (action.name() == "PAUSE") {
      setPaused(!m_paused);
    } else if (action.name() == "QUIT") {
      onEnd();
    }
  }
}

void Scene_Shop::sRender() {  // VISUAL

  // Store sf::window in a variable for more convenience
  auto& w = m_game->window();
  auto& f = m_game->getAssets().getFont("RETROGAMING");
  // Creates a clock variable for defining the change of colors in texts
  sf::Event e;
  static sf::Clock keyCooldownClock;
  const float keyCooldown = 0.1f;  // 200 milliseconds

  // Handling input block
  auto& moveSound = m_game->getAssets().getSound("clickSound");
  auto& enterSound = m_game->getAssets().getSound("enterSound");

  if (keyCooldownClock.getElapsedTime().asSeconds() >= keyCooldown) {
    switch (selectPositionIndex) {
      case 0:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();

          select.setPosition(selectPosition[1]);
          selectPositionIndex = 1;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();

          select.setPosition(selectPosition[3]);
          selectPositionIndex = 3;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (!isEquipped[0]) {
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[0] = true;
            m_game->m_equippedSkin = 0;
          }
          enterSound.setVolume(m_game->getVolume());
          enterSound.play();
        }
        break;
      case 1:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[2]);
          selectPositionIndex = 2;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          select.setPosition(selectPosition[4]);
          selectPositionIndex = 4;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[0]);
          selectPositionIndex = 0;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (!skinPurchased[0]) {
            skinPurchased[0] = true;
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[1] = true;
            m_game->m_equippedSkin = 1;
          } else if (skinPurchased[0]) {
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[1] = true;
            m_game->m_equippedSkin = 1;
          }
          enterSound.setVolume(m_game->getVolume());
          enterSound.play();
        }
        break;
      case 2:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[1]);
          selectPositionIndex = 1;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[5]);
          selectPositionIndex = 5;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (!skinPurchased[1]) {
            skinPurchased[1] = true;
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[2] = true;
            m_game->m_equippedSkin = 2;
          } else if (skinPurchased[1]) {
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[2] = true;
            m_game->m_equippedSkin = 2;
          }
          enterSound.setVolume(m_game->getVolume());
          enterSound.play();
        }
        break;
      case 3:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[4]);
          selectPositionIndex = 4;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[0]);
          selectPositionIndex = 0;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (!skinPurchased[2]) {
            skinPurchased[2] = true;
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[3] = true;
            m_game->m_equippedSkin = 3;
          } else if (skinPurchased[2]) {
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[3] = true;
            m_game->m_equippedSkin = 3;
          }
          enterSound.setVolume(m_game->getVolume());
          enterSound.play();
        }
        break;
      case 4:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[5]);
          selectPositionIndex = 5;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[1]);
          selectPositionIndex = 1;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[3]);
          selectPositionIndex = 3;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (!skinPurchased[3]) {
            skinPurchased[3] = true;
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[4] = true;
            m_game->m_equippedSkin = 4;
          } else if (skinPurchased[3]) {
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[4] = true;
            m_game->m_equippedSkin = 4;
          }
          enterSound.setVolume(m_game->getVolume());
          enterSound.play();
        }
        break;
      case 5:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[4]);
          selectPositionIndex = 4;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          moveSound.setVolume(m_game->getVolume());
          moveSound.play();
          select.setPosition(selectPosition[2]);
          selectPositionIndex = 2;
          keyCooldownClock.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (!skinPurchased[4]) {
            skinPurchased[4] = true;
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[5] = true;
            m_game->m_equippedSkin = 5;
          } else if (skinPurchased[4]) {
            for (int i = 0; i <= 5; i++) {
              isEquipped[i] = false;
            }
            isEquipped[5] = true;
            m_game->m_equippedSkin = 5;
          }
          enterSound.setVolume(m_game->getVolume());
          enterSound.play();
        }
        break;
    }
  }

  // Hovering design block
  if (!isEquipped[0])
    texts[7] = textConfig("OFF", selectPosition[0], 24, f);
  else
    texts[7] = textConfig("ON", selectPosition[0], 24, f);

  if (!skinPurchased[0])
    texts[8] = textConfig("$10", selectPosition[1], 24, f);
  else if (skinPurchased[0] && isEquipped[1])
    texts[8] = textConfig("ON", selectPosition[1], 24, f);
  else
    texts[8] = textConfig("OFF", selectPosition[1], 24, f);

  if (!skinPurchased[1])
    texts[9] = textConfig("$20", selectPosition[2], 24, f);
  else if (skinPurchased[1] && isEquipped[2])
    texts[9] = textConfig("ON", selectPosition[2], 24, f);
  else
    texts[9] = textConfig("OFF", selectPosition[2], 24, f);

  if (!skinPurchased[2])
    texts[10] = textConfig("$30", selectPosition[3], 24, f);
  else if (skinPurchased[2] && isEquipped[3])
    texts[10] = textConfig("ON", selectPosition[3], 24, f);
  else
    texts[10] = textConfig("OFF", selectPosition[3], 24, f);

  if (!skinPurchased[3])
    texts[11] = textConfig("$40", selectPosition[4], 24, f);
  else if (skinPurchased[3] && isEquipped[4])
    texts[11] = textConfig("ON", selectPosition[4], 24, f);
  else
    texts[11] = textConfig("OFF", selectPosition[4], 24, f);

  if (!skinPurchased[4])
    texts[12] = textConfig("$69", selectPosition[5], 24, f);
  else if (skinPurchased[4] && isEquipped[5])
    texts[12] = textConfig("ON", selectPosition[5], 24, f);
  else
    texts[12] = textConfig("OFF", selectPosition[5], 24, f);

  float time = clock.getElapsedTime().asSeconds();

  texts[0].setFillColor(getDynamicColor(time));
  select.setFillColor(getDynamicColor(time));

  w.clear();

  w.setView(backgroundView);  // sets the background view and draws it
  w.draw(background);

  w.setView(textView);  // sets the text view and draws it
  w.draw(select);
  for (int i = 0; i <= 13; i++) w.draw(texts[i]);
  for (int i = 0; i <= 5; i++) w.draw(playerIdle[i]);

  w.display();
}
