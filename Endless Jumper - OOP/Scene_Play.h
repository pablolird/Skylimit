#pragma once
#include "Scene.h"
#include <vector>
#include <SFML/Graphics.hpp>


class Scene_Play : public Scene {
protected:

	sf::Music* m_backgroundMusic = nullptr; ///background music for the play scene
	sf::Text m_scoreText;		/// text for the score
	size_t m_score;				/// number for the score
	sf::View m_view;			/// the view of the scene
	size_t m_currentBackground = 0;	/// index of the background
	size_t m_pastBackground;		/// previous index background
	size_t m_currentFrame = 0;		/// index of the current frame
	size_t m_highestScore;			/// highest score number
	int m_alpha = 255;				/// variables to store the opacity of the background (useful for transition)
	bool m_drawTextures = true;		/// defines if the textures are drawn to the screen or not (PRESS T)
	bool m_drawCollision = false;	/// defines if the hitboxes are drawn to the screen or not (PRESS C)
	bool m_move = false;			/// defines if the camera is moving or not
	bool m_transition = false;		/// bool variable to know if the background are transitioning or not
	bool m_end = false;				/// to know if a scene has ended
	Vec2 m_gridSize = { 32,32 };	/// how many pixels x pixels has the grid
	Vec2 m_targetViewPosition;		/// it determines where the camera should go
	size_t m_platformSpacing = 100;	/// how many pixels have to be between the platforms


	std::shared_ptr<Entity> m_player;	/// shared pointer to the player
	sf::Clock m_deltaClock;				/// clock to measure time between frames
	float m_dt60 = 1.f;				/// delta time normalized to 60fps (1.0 at 60fps, 0.5 at 120fps)
public:
	Scene_Play(GameEngine* gameEngine);

	// Check which is public or private smh
	void init();
	void update();

	void onEnd();
	void replay();

	// Systems
	void sRemoveDeadPlatforms();
	void sPlatformGeneration();
	void sAnimation();
	void sMovement();
	void sCollision();
	void sRender();
	void sDoAction(const Action& a);
	void spawnPlayer();
	void loadLevel();
};