#pragma warning(disable : 4996)

#include "Scene_Play.h"
#include "Scene_Menu.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <random>

Scene_Play::Scene_Play(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	srand(time(NULL));
	init();
}


void Scene_Play::init() {
	m_highestScore = m_game->getHighestScore();
	
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	//registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE"); // Toggle drawing textures
	//registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing collision boxes
	registerAction(sf::Keyboard::R, "REPLAY");
	if (m_game->getWASD()) {
		//registerAction(sf::Keyboard::W, "UP"); //POSSIBLE DOUBLE JUMP LATER
		registerAction(sf::Keyboard::A, "LEFT");
		registerAction(sf::Keyboard::D, "RIGHT");
	}
	else {
		//registerAction(sf::Keyboard::Up, "UP"); //POSSIBLE DOUBLE JUMP LATER
		registerAction(sf::Keyboard::Left, "LEFT");
		registerAction(sf::Keyboard::Right, "RIGHT");
	}


	loadLevel();

	auto& window = m_game->window();
	auto player_pos = m_player->getComponent<CTransform>().pos;  

	m_view = sf::View(sf::Vector2f(window.getSize().x / 2, player_pos.y), sf::Vector2f(1024, 720.f));
	m_view.setViewport(sf::FloatRect(0.1f, 0, 0.8, 1));
	window.setView(m_view);

	auto& f = m_game->getAssets().getFont("RETROGAMING");
	m_scoreText = sf::Text("0", f, 50);
	m_scoreText.setColor(sf::Color::White);
	m_score = 0;

}

void Scene_Play::sDoAction(const Action& action) {
	if (action.type() == "START") {
		if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
		else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
		else if (action.name() == "PAUSE") { setPaused(!m_paused); }
		else if (action.name() == "QUIT") { onEnd(); }
		else if (action.name() == "REPLAY") { if (!m_player->isActive() || m_paused) { replay(); }
		}//CHANGE SCENE }
		if (action.name() == "RIGHT") {
			m_player->getComponent<CInput>().right = true;	
		}
		if (action.name() == "LEFT") {
			m_player->getComponent<CInput>().left = true;
		}
		if (action.name() == "UP") {
			m_player->getComponent<CInput>().up = true;
		}
	}
	else if (action.type() == "END") {
		if (action.name() == "RIGHT") {
			m_player->getComponent<CInput>().right = false;
		}
		if (action.name() == "UP") {
			m_player->getComponent<CInput>().up = false;
		}
		if (action.name() == "LEFT") {
			m_player->getComponent<CInput>().left = false;
		}
	}
}

void Scene_Play::update() {
	m_dt60 = std::min(m_deltaClock.restart().asSeconds() * 60.f, 3.f);
	sRemoveDeadPlatforms();
	m_entityManager.update();
	if (!m_paused) {
		sCollision();
		sMovement();
		sPlatformGeneration();
		sAnimation();
	}
	sRender();
	m_currentFrame++;
}


void Scene_Play::spawnPlayer() {
	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("playerFall"), false);
	m_player->getComponent<CAnimation>().animation.getSprite().setScale(2, 2);
	auto size = m_player->getComponent<CAnimation>().animation.getSprite().getGlobalBounds();
	m_player->addComponent<CBoundingBox>(Vec2(size.getSize().x, size.getSize().y));
	m_player->addComponent<CTransform>(Vec2(m_game->window().getSize().x / 2, -20));
	m_player->addComponent<CGravity>();
	m_player->addComponent<CState>();
}

void Scene_Play::loadLevel() {
	// reset the entity manager every time we load a level
	m_entityManager = EntityManager();

	size_t initialPlatformCount = 70;
	
	spawnPlayer();

	auto& window = m_game->window();

	auto x_spawn_min = 0.1 * window.getSize().x;
	int win_width = window.getSize().x*0.8;

	float y_pos = m_player->getComponent<CTransform>().pos.y - 50.0f;
	for (int i = 0; i < initialPlatformCount; ++i) {
			auto tile = m_entityManager.addEntity("tile");
			y_pos -= 100.0f;  // Adjust spacing between tiles

			// Crear un generador de n�meros aleatorios basado en el tiempo del sistema
			std::random_device rd;
			std::mt19937 gen(rd());

			std::bernoulli_distribution d(0.5);

			int type = d(gen) ? 1 : 2;

			if (type == 1) {
				tile->addComponent<CAnimation>(m_game->getAssets().getAnimation("grass01"), false);
			}
			else if (type == 2) {
				tile->addComponent<CAnimation>(m_game->getAssets().getAnimation("log01"), false);
			}
			tile->getComponent<CAnimation>().animation.getSprite().setScale(0.15, 0.15);
			auto size = tile->getComponent<CAnimation>().animation.getSprite().getGlobalBounds();
			tile->addComponent<CBoundingBox>(Vec2(size.getSize().x, size.getSize().y));

			if (i == 0) {
				tile->addComponent<CTransform>(m_player->getComponent<CTransform>().pos+Vec2(0,60));
			}
			else {
				if (type == 2) {
					// Crear una distribuci�n de Bernoulli con probabilidad de 0.8 para obtener true (lo que representar� 1)
					std::bernoulli_distribution d(0.7);
					// Generar el n�mero basado en la distribuci�n
					int side = d(gen) ? 0 : 1;
					auto pos_x = x_spawn_min + 100 + rand() % 300 + side * win_width / 2.f;
					tile->addComponent<CTransform>(Vec2(pos_x, y_pos));
					tile->addComponent<CMove>();
					tile->getComponent<CTransform>().velocity.x = rand() % 3 + 1;
				}
				else {
					int size_x = tile->getComponent<CBoundingBox>().size.x;
					auto x_pos = (rand() % (win_width - size_x) + x_spawn_min + (size_x / 2.f));
					tile->addComponent<CTransform>(Vec2(x_pos, y_pos));
				}
			}
			auto pos = tile->getComponent<CTransform>().pos;
			tile->getComponent<CAnimation>().animation.getSprite().setPosition(pos.x, pos.y);
	}
}

void Scene_Play::sRender() {

	auto& player_pos = m_player->getComponent<CTransform>().pos;
	auto& window = m_game->window();
	auto& animation = m_player->getComponent<CAnimation>().animation;
	auto& equippedSkin = m_game->m_equippedSkin;
	//Verifies the player skin
	switch (equippedSkin)
	{
	case 1:
		animation.getSprite().setColor(sf::Color(10, 10, 255, 255));
		break;
	case 2:
		animation.getSprite().setColor(sf::Color(255, 255, 0, 255));
		break;
	case 3:
		animation.getSprite().setColor(sf::Color(255, 0, 155, 255));
		break;
	case 4:
		animation.getSprite().setColor(sf::Color(0, 255, 255, 255));
		break;
	default:
		break;
	}
	window.clear(sf::Color::Transparent);

	window.setView(m_view);

	auto& view_center = m_view.getCenter();

	// Define parallax speeds for each background layer
	float parallaxSpeed = 0.9;

	// Adjust background positions with parallax effect

	if ((m_score / 5000) % 8 != m_currentBackground) {
		m_transition = true;
		m_pastBackground = m_currentBackground;
		m_currentBackground = (m_score / 5000) % 8;
		m_platformSpacing += 5;
	}

	for (auto& bg : m_game->getAssets().getBackground(m_currentBackground).getLayers()) {
		float offset = (view_center.y + (m_score / 5000) * 5000) * parallaxSpeed;
		bg.setPosition(bg.getPosition().x, offset - 300 - (m_score / 5000) * 5000);
		parallaxSpeed -= 0.1;
		if (m_transition) {
			bg.setColor(sf::Color(255, 255, 255, (255 - m_alpha)));
		}
		else {
			bg.setColor(sf::Color(255, 255, 255, 255));
		}
		window.draw(bg);
	}

	float parallaxSpeed2 = 0.9;

	if (m_transition) {
		if (m_alpha > 0) {
			for (auto& bg2 : m_game->getAssets().getBackground(m_pastBackground).getLayers()) {
				float offset2 = (view_center.y + (m_pastBackground * 5000 / 5000) * 5000) * parallaxSpeed2;
				bg2.setPosition(bg2.getPosition().x, offset2 - 300 - (m_pastBackground * 5000 / 5000) * 5000);
				bg2.setColor(sf::Color(255, 255, 255, m_alpha));
				parallaxSpeed2 -= 0.1;
				window.draw(bg2);
			}
			m_alpha -= 5;
		}
		else {
			m_alpha = 255;
			m_transition = false;
		}

	}


	if ((m_score - m_highestScore) < 1000 || (m_score - m_highestScore) > -1000) {

		sf::RectangleShape HighLine(sf::Vector2f(window.getSize().x, 4.f));
		HighLine.setOrigin(HighLine.getSize().x / 2.f, HighLine.getSize().y / 2.f);
		HighLine.setPosition(window.getSize().x/2.f, -(int)m_highestScore);
		HighLine.setFillColor(sf::Color::White);
		std::string text = "Highest Score: " + std::to_string(m_highestScore);
		auto HighText = sf::Text(text, m_game->getAssets().getFont("RETROGAMING"), 20);
		HighText.setFillColor(sf::Color::White);
		auto x_spawn_min = window.getView().getViewport().getPosition().x * window.getSize().x;
		HighText.setPosition(x_spawn_min+50, -(int)m_highestScore - 30);
		HighText.setOutlineThickness(1.f);
		HighText.setOutlineColor(sf::Color::Black);
		window.draw(HighLine);
		window.draw(HighText);
	}

	
	if (m_drawTextures) {
		for (auto e : m_entityManager.getEntities()) {
			window.draw(e->getComponent<CAnimation>().animation.getSprite());
		}
	}

	if (m_drawCollision) {
		for (auto e : m_entityManager.getEntities()) {
			auto size_x = e->getComponent<CBoundingBox>().size.x;
			auto size_y = e->getComponent<CBoundingBox>().size.y;
			sf::RectangleShape BoundingBox(sf::Vector2f(size_x, size_y));
			BoundingBox.setOrigin(e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CBoundingBox>().halfSize.y);
			BoundingBox.setPosition(e->getComponent<CTransform>().pos.x, e->getComponent<CTransform>().pos.y);
			BoundingBox.setOutlineColor(sf::Color::Blue);
			BoundingBox.setFillColor(sf::Color::Transparent);
			BoundingBox.setOutlineThickness(1);
			window.draw(BoundingBox);
		}
	}
	if (!m_player->isActive() || m_paused) {
		auto replay = sf::Text("Replay: R", m_game->getAssets().getFont("RETROGAMING"), 50);
		auto quit = sf::Text("Quit: ESC", m_game->getAssets().getFont("RETROGAMING"), 50);

		auto rect = sf::RectangleShape(sf::Vector2f(400, 250));
		rect.setFillColor(sf::Color::Yellow);
		rect.setOrigin(rect.getSize().x / 2.f, rect.getSize().y / 2.f);
		rect.setPosition(m_view.getCenter().x, m_view.getCenter().y-40);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(6.f);

		replay.setOrigin(replay.getGlobalBounds().width / 2.f, replay.getGlobalBounds().height / 2.f);
		quit.setOrigin(quit.getGlobalBounds().width / 2.f, quit.getGlobalBounds().height / 2.f);

		replay.setPosition(m_view.getCenter().x, m_view.getCenter().y-100);
		quit.setPosition(m_view.getCenter().x, m_view.getCenter().y);

		replay.setColor(sf::Color::Black);
		quit.setColor(sf::Color::Black);

		replay.setOutlineThickness(3.f);
		quit.setOutlineThickness(3.f);
		replay.setOutlineColor(sf::Color::White);
		quit.setOutlineColor(sf::Color::White);

		window.draw(rect);
		window.draw(replay);
		window.draw(quit);
	}

	window.draw(m_scoreText);

	window.display();
}

void Scene_Play::sAnimation() {
	if (m_player->isActive()) {
		auto& player = m_player->getComponent<CTransform>();
		auto player_state = m_player->getComponent<CState>().state;
		auto& animation = m_player->getComponent<CAnimation>().animation;
		auto& equippedSkin = m_game->m_equippedSkin;

		if(equippedSkin != 5)
		{
			if (player_state == "jump") {
				if (animation.getName() != "playerJump") {
					animation = m_game->getAssets().getAnimation("playerJump");
				}
			}
			else if (player_state == "fall") {
				if (animation.getName() != "playerFall") {
					animation = m_game->getAssets().getAnimation("playerFall");
				}
			}
		}
		else if (equippedSkin == 5) {
			if (player_state == "jump") {
				if (animation.getName() != "venom_Jump") {
					animation = m_game->getAssets().getAnimation("venom_Jump");
				}
			}
			else if (player_state == "fall") {
				if (animation.getName() != "venom_Fall") {
					animation = m_game->getAssets().getAnimation("venom_Fall");
				}
			}
		}
		

		animation.getSprite().setScale(2, 2);

		if (player.pos.x < player.prevPos.x) {
			if (animation.getSprite().getScale().x != -2) {
				animation.getSprite().scale(-1, 1);
			}
		}
		else if (player.pos.x > player.prevPos.x) {
			if (animation.getSprite().getScale().x != 2) {
				animation.getSprite().scale(-1, 1);
			}
		}

		m_player->getComponent<CAnimation>().animation.getSprite().setPosition(player.pos.x, player.pos.y);
		m_player->getComponent<CAnimation>().animation.update();
	}
}

void Scene_Play::sCollision() {
	if (m_player->isActive()) {
		auto& player = m_player->getComponent<CTransform>();

		if (player.pos.y > m_view.getCenter().y + 350) {
			m_player->destroy();
			m_end = true;
		}

		for (auto& e : m_entityManager.getEntities("tile")) {
			if (player.pos.y > player.prevPos.y) {
				auto& tile = e->getComponent<CTransform>();
				Vec2 overlap = GetOverlap(m_player, e);
				Vec2 prevOverlap = GetPreviousOverlap(m_player, e);

				float dy = tile.pos.y - player.pos.y;

				if (0 < overlap.x && -m_gridSize.y < overlap.x && dy > 0) {
					if (0 <= overlap.y && prevOverlap.y <= 0) {
						// stand on tile
						if (e->hasComponent<CKill>()) {
							m_player->destroy();
							m_end = true;
						}
						else {
							m_player->getComponent<CInput>().canJump = true;
							m_player->getComponent<CGravity>().gravity = 0;
							player.velocity.y = -20;
							// collision resolution
							auto& jumpSound = m_game->getAssets().getSound("jumpSound");
							jumpSound.setVolume(m_game->getVolume());
							jumpSound.play();

							player.pos.y -= overlap.y;
							m_move = true;
							m_targetViewPosition = Vec2(m_view.getCenter().x, tile.pos.y - 500);
						}
					}
				}
			}
		}
	}
}


sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t) {
	return start + t * (end - start);
}

void Scene_Play::sMovement() {
	auto& input = m_player->getComponent<CInput>();
	auto& player_pos = m_player->getComponent<CTransform>().pos;
	auto& player = m_player->getComponent<CTransform>();
	auto& player_velocity = m_player->getComponent<CTransform>().velocity;
	player_velocity.x = 0;

	auto& window = m_game->window();

	if (m_move) {
		// Calculate the new position using interpolation
		float t = std::min(0.05f * m_dt60, 0.15f);  // Adjust t for the desired speed (0 < t < 1), scaled to fps
		sf::Vector2f currentViewPosition = m_view.getCenter();
		sf::Vector2f newViewPosition = lerp(currentViewPosition, sf::Vector2f(m_targetViewPosition.x, m_targetViewPosition.y), t);

		// Update the view's position
		m_view.setCenter(newViewPosition);

		// Check if the camera is close enough to the target position
		if (std::abs(newViewPosition.y - m_targetViewPosition.y) < 1.0f) {
			m_view.setCenter(sf::Vector2f(m_targetViewPosition.x, m_targetViewPosition.y));
			m_move = false;
		}
	}

	if (m_player->isActive()) {
		if (input.left) {
			player_velocity.x = -5;
			m_player->getComponent<CState>().state = "run";
		}
		if (input.right) {
			player_velocity.x = 5;
			m_player->getComponent<CState>().state = "run";
		}
		if (input.up) {
			player_velocity.y = -50;
			m_player->getComponent<CState>().state = "jump";
		}
		if (player_pos.y > player.prevPos.y) {
			m_player->getComponent<CState>().state = "fall";
		}
		if (player_pos.y < player.prevPos.y) {
			m_player->getComponent<CState>().state = "jump";
		}
		m_player->getComponent<CTransform>().prevPos = player_pos;

		player_velocity.y += m_player->getComponent<CGravity>().gravity * m_dt60;
		player_pos += player_velocity * m_dt60;

		if (player_velocity.y > 20) {
			player_velocity.y = 20;
		}
		m_player->getComponent<CGravity>().gravity = 0.4;

		if (player.pos == player.prevPos) {
			m_player->getComponent<CState>().state = "idle";
		}

		if (m_score < abs(player.pos.y) && player.pos.y < 0) {
			m_score = abs((int)player_pos.y);
			m_scoreText.setString(std::to_string(m_score));
		}
	}

	m_scoreText.setPosition(m_view.getCenter().x-m_scoreText.getGlobalBounds().width/2.f, m_view.getCenter().y - 350);


	auto x_spawn_min = window.getView().getViewport().getPosition().x * window.getSize().x;
	int win_width = window.getView().getSize().x;

	for (auto& e : m_entityManager.getEntities("tile")) {
		if (e->hasComponent<CMove>()) {
			auto & movingTile = e->getComponent<CTransform>();
			movingTile.pos += movingTile.velocity * m_dt60;
			e->getComponent<CAnimation>().animation.getSprite().setPosition(movingTile.pos.x, movingTile.pos.y);
			if (movingTile.pos.x < x_spawn_min + win_width / 2) {
				if (movingTile.pos.x < x_spawn_min + 100 ||
					movingTile.pos.x > x_spawn_min + 400)
				{
					movingTile.velocity.x *= -1;
				}
			}
			else {
				if (movingTile.pos.x > x_spawn_min + win_width - 100 ||
					movingTile.pos.x < x_spawn_min + win_width/2.f + 100)
				{
					movingTile.velocity.x *= -1;
				}
			}
		}
	}
}

void Scene_Play::onEnd() {
	
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
}

void Scene_Play::sPlatformGeneration() {
	// Crear un generador de n�meros aleatorios basado en el tiempo del sistema
	std::random_device rd;
	std::mt19937 gen(rd());


	double percent = m_score / 9000;

	std::discrete_distribution<> dist({ 60-percent, 5 + percent, 30-percent, 5 + percent});


	auto & window = m_game->window();
	float highestPlatformY = m_entityManager.getEntities("tile").back()->getComponent<CTransform>().pos.y;

	auto& player_pos = m_player->getComponent<CTransform>().pos;

	auto x_spawn_min = window.getView().getViewport().getPosition().x*window.getSize().x;
	int win_width = window.getView().getSize().x;

	// Generate new platforms above the highest one if the player is nearing the top
	if (abs(player_pos.y) > abs(highestPlatformY)-300) {
		auto tile = m_entityManager.addEntity("tile");

		size_t type = dist(gen) + 1;

		if (type == 1) {
			tile->addComponent<CAnimation>(m_game->getAssets().getAnimation("grass01"), false);
		}
		else if (type == 2) {
			tile->addComponent<CAnimation>(m_game->getAssets().getAnimation("grass02"), false);
			tile->addComponent<CKill>();
		}
		else if (type == 3) {
			tile->addComponent<CAnimation>(m_game->getAssets().getAnimation("log01"), false);

		}
		else if (type == 4) {
			tile->addComponent<CAnimation>(m_game->getAssets().getAnimation("log02"), false);
			tile->addComponent<CKill>();
		}
		tile->getComponent<CAnimation>().animation.getSprite().setScale(0.15, 0.15);
		auto size = tile->getComponent<CAnimation>().animation.getSprite().getGlobalBounds();
		tile->addComponent<CBoundingBox>(Vec2(size.getSize().x, size.getSize().y));
		int size_x = tile->getComponent<CBoundingBox>().size.x;

		if (type == 3 || type == 4) {
			std::bernoulli_distribution d(0.5);
			int side = d(gen) ? 0 : 1;

			auto pos_x = x_spawn_min + 100 + rand()%300 + side * win_width / 2.f;
			tile->addComponent<CTransform>(Vec2(pos_x, highestPlatformY - m_platformSpacing));
			tile->addComponent<CMove>();
			tile->getComponent<CTransform>().velocity.x = rand() % 3 + 1;
		}
		else {
			auto pos_x = (rand() % (win_width - size_x * 2)) + x_spawn_min + size_x;
			tile->addComponent<CTransform>(Vec2(pos_x, highestPlatformY - m_platformSpacing));
		}

		auto pos = tile->getComponent<CTransform>().pos;
		tile->getComponent<CAnimation>().animation.getSprite().setPosition(pos.x, pos.y);
	}
}

void Scene_Play::sRemoveDeadPlatforms() {
	auto& view_center = m_view.getCenter();

	for (auto& tile : m_entityManager.getEntities("tile")) {
		auto& tile_pos = tile->getComponent<CTransform>().pos;
		if (abs(tile_pos.y)+400 < abs(view_center.y)) {
			tile->destroy();
		}
	}
}

void Scene_Play::replay() {
	if (m_score > m_highestScore) {
		m_game->setHighestScore(m_score);

	}
	m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game), true);
}