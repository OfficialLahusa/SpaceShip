#pragma once
#include "../../SimpleSfmlEngine/engine.hpp"

#include "../def.h"
#include "../player.h"
#include "../shot.h"
#include "../meteorite.h"

#include <iostream>

/* Returns the animated player sprite, if given the skin id, the direction the player is facing, and the frame */
/* Directions: 0: Down, 1: Right, 2: Top, 3: Left */
/* Frames: 0: Standing, 1: Walk1, 2: Walk2 */
sf::IntRect getPlayerSprite(unsigned int speed, unsigned int shotstate);

/* Returns the rotated shot origin position */
sf::Vector2f getShotOrigin(unsigned int shotState, float angle);

/* Returns the skin name by ID */
std::string getSkinName(unsigned int skin);

/* Returns the meteorite sprite by ID */
sf::IntRect getMeteoriteSprite(unsigned int ID);

/* Returns the health bar sprite by health */
sf::IntRect getHealthBarSprite(int current, int max, bool isProtected);

namespace sse
{
	class GameState : public State
	{
	public:
		explicit GameState(GameDataRef data);
		~GameState();

		bool Init() override;

		bool HandleInput(float dt) override;
		bool Update(float dt) override;
		bool Render(float dt) override;

	private:

		GameDataRef m_data;


		/* Player Setup */
		sf::Texture playerSprites;
		Player player;

		/* Combat Setup */
		sf::Texture particleTexture;
		std::vector<Shot> shots;
		Shot basicShot;
		sf::Texture meteoriteTexture;
		std::vector<Meteorite> meteorites;
		Meteorite basicMeteorite;
		sf::Texture healthBarTexture;
		sf::RectangleShape healthBar;

		/* Collision detection and resolution variables */
		sf::RectangleShape collisionShape;
		sf::FloatRect collisionBounds, playerBounds;

		/* UI Elements */
		sf::Font font;
		sf::Text xText;

		/* Sound Setup */
		sf::SoundBuffer shotBuffer, engineBuffer;
		sf::Sound shotSound, engineSound;
	};
}