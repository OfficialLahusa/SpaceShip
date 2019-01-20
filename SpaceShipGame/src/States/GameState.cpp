#include "GameState.hpp"

/* Returns the animated player sprite, if given the skin id, the direction the player is facing, and the frame */
/* Directions: 0: Down, 1: Right, 2: Top, 3: Left */
/* Frames: 0: Standing, 1: Walk1, 2: Walk2 */
sf::IntRect getPlayerSprite(unsigned int speed, unsigned int shotstate) {
	return sf::IntRect(sf::Vector2i(128 * 3 * speed + shotstate * 128, 0), sf::Vector2i(128, 128));
}

/* Returns the skin name by ID */
std::string getSkinName(unsigned int skin) {
	switch (skin) {
	default: return "Unnamed Character";
	}
}

/* Returns the rotated shot origin position */
sf::Vector2f getShotOrigin(unsigned int shotState, float angle) {
	sf::Vector2f offsetVec;
	shotState %= 2;
	if (shotState == 0) {
		offsetVec = sf::Vector2f(-13*TILE_SCALING, 28*TILE_SCALING);
	}
	else {
		offsetVec = sf::Vector2f(13*TILE_SCALING, 28*TILE_SCALING);
	}
	float vecLength = sqrtf(offsetVec.x*offsetVec.x + offsetVec.y*offsetVec.y);
	float offsetVectorRotation = atan2f(offsetVec.x / vecLength, offsetVec.y / vecLength);

	offsetVec = sf::Vector2f(cos(offsetVectorRotation + angle / 180 * sse::math::PI)*vecLength, sin(offsetVectorRotation + angle / 180 * sse::math::PI)*vecLength);
	//std::cout << "X:" << offsetVec.x << "; Y:" << offsetVec.y << "; Angle: " << offsetVectorRotation << "; Length: " << vecLength << "\n";
	return offsetVec;
}

/* Returns the meteorite sprite by ID */
sf::IntRect getMeteoriteSprite(unsigned int ID) {
	return sf::IntRect(sf::Vector2i(0, 64 * ID), sf::Vector2i(64, 64));
}

/* Returns the health bar sprite by health */
sf::IntRect getHealthBarSprite(int current, int max, bool isProtected) {

	if (isProtected) {
		return sf::IntRect(sf::Vector2i(0, HEALTH_BAR_PROTECTED * HEALTH_BAR_HEIGHT), sf::Vector2i(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT));
	}

	int verticalOffset = -(int)((((float)current / (float)max)*(float)(HEALTH_BAR_VARIATIONS - 1)) - (float)(HEALTH_BAR_VARIATIONS - 1));
	return sf::IntRect(sf::Vector2i(0, verticalOffset * HEALTH_BAR_HEIGHT), sf::Vector2i(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT));
}

namespace sse
{
	GameState::GameState(GameDataRef data)
		: m_data(data)
	{

	}

	GameState::~GameState()
	{

	}

	bool GameState::Init() {

		/* Player Setup */
		playerSprites.loadFromFile("res/raumschiffe_spritesheet.png");
		player = Player(0, 0, sf::Vector2f(m_data->window.getSize().x / 2, m_data->window.getSize().y/2));
		player.shape.setOrigin(64 * TILE_SCALING, 64 * TILE_SCALING);
		player.shape.setTexture(&playerSprites);
		player.shape.setTextureRect(getPlayerSprite(1, 1));

		/* Combat Setup */
		particleTexture.loadFromFile("res/particles.png");
		basicShot.shape.setSize(sf::Vector2f(16 * TILE_SCALING, 16 * TILE_SCALING));
		basicShot.shape.setTexture(&particleTexture);
		basicShot.shape.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(16, 16)));
		basicShot.shape.setOrigin(8 * TILE_SCALING, 8 * TILE_SCALING);
		
		meteoriteTexture.loadFromFile("res/meteorites.png");
		basicMeteorite.shape.setSize(sf::Vector2f(64 * TILE_SCALING, 64 * TILE_SCALING));
		basicMeteorite.shape.setTexture(&meteoriteTexture);
		basicMeteorite.shape.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(64, 64)));
		basicMeteorite.shape.setOrigin(32 * TILE_SCALING, 32 * TILE_SCALING);

		healthBarTexture.loadFromFile("res/healthbar.png");
		healthBar.setSize(sf::Vector2f(HEALTH_BAR_WIDTH * TILE_SCALING, HEALTH_BAR_HEIGHT * TILE_SCALING));
		healthBar.setOrigin(HEALTH_BAR_WIDTH * TILE_SCALING / 2, HEALTH_BAR_HEIGHT * TILE_SCALING);
		healthBar.setTexture(&healthBarTexture);

		/* Collision detection and resolution variables */
		collisionShape.setSize(sf::Vector2f(16, 16));
		collisionShape.setScale(TILE_SCALING, TILE_SCALING);
		collisionShape.setFillColor(sf::Color::Red);

		/* UI Elements */
		font.loadFromFile("res/CarbonBlock.ttf");
		xText.setFillColor(sf::Color::White);
		xText.setPosition(20, 10);
		xText.setCharacterSize(35);
		xText.setFont(font);

		/* Sound Setup */
		shotBuffer.loadFromFile("res/laser_shot.wav");
		engineBuffer.loadFromFile("res/thruster.wav");
		shotSound.setBuffer(shotBuffer);
		engineSound.setBuffer(engineBuffer);

		return true;
	}

	bool GameState::HandleInput(float dt) {
		sf::Event evnt;

		while (m_data->window.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				m_data->window.close();
			}
		}

		/* Input Handling */
		/* Get Joystick axis input and map it correctly + remove calibration faults */
		float axisX, axisY, axisU, axisV, axisZ, axisR;
		axisX = std::max(std::min(sf::Joystick::getAxisPosition(0, sf::Joystick::X), 100.f), -100.f); if (axisX <= CALIBRATION_FAULT_THRESHOLD && axisX >= -CALIBRATION_FAULT_THRESHOLD) axisX = 0.f;
		axisY = std::max(std::min(sf::Joystick::getAxisPosition(0, sf::Joystick::Y), 100.f), -100.f); if (axisY <= CALIBRATION_FAULT_THRESHOLD && axisY >= -CALIBRATION_FAULT_THRESHOLD) axisY = 0.f;
		axisU = std::max(std::min(sf::Joystick::getAxisPosition(0, sf::Joystick::U), 100.f), -100.f); if (axisU <= CALIBRATION_FAULT_THRESHOLD && axisU >= -CALIBRATION_FAULT_THRESHOLD) axisU = 0.f;
		axisV = std::max(std::min(sf::Joystick::getAxisPosition(0, sf::Joystick::V), 100.f), -100.f); if (axisV <= CALIBRATION_FAULT_THRESHOLD && axisV >= -CALIBRATION_FAULT_THRESHOLD) axisV = 0.f;
		axisZ = std::max(std::min(sf::Joystick::getAxisPosition(0, sf::Joystick::Z), 100.f), -100.f); if (axisZ <= CALIBRATION_FAULT_THRESHOLD && axisZ >= -CALIBRATION_FAULT_THRESHOLD) axisZ = 0.f;

		sf::Vector2f direction(axisX / 100.f, axisY / 100.f);
		float vectorLength = std::max(sqrtf(direction.x*direction.x + direction.y*direction.y), 0.00000001f);
		direction.x = direction.x / vectorLength;
		direction.y = direction.y / vectorLength;
		float angle = -atan2f(direction.x, direction.y) * 180 / sse::math::PI + 90;
		if (direction.x == 0 && direction.y == 0) {
			// do nothing
		}
		else {
			player.shape.setRotation(angle);
		}
		//std::cout << direction.x << "; " << direction.y << "; Angle: " << angle << "deg\n";
		

		if (axisZ != 0) {
			if (player.shotState == 0 || (player.shotState > 1 && player.hitCooldown == 0.f)) {
				player.shotState = 1;
				player.hitCooldown += 0.1f * 1/player.fireSpeed;

				sf::Vector2f origin = getShotOrigin(player.shotState, player.shape.getRotation());
				shots.push_back(basicShot);
				shots[shots.size() - 1].shape.setPosition(player.shape.getPosition().x + origin.x, player.shape.getPosition().y + origin.y);
				shots[shots.size() - 1].shape.setRotation(player.shape.getRotation());
				shotSound.play();
			}
			else if(player.hitCooldown == 0.f) {
				player.shotState = 2;
				player.hitCooldown += 0.1f * 1/player.fireSpeed;
				
				sf::Vector2f origin = getShotOrigin(player.shotState, player.shape.getRotation());
				shots.push_back(basicShot);
				shots[shots.size() - 1].shape.setPosition(player.shape.getPosition().x + origin.x, player.shape.getPosition().y + origin.y);
				shots[shots.size() - 1].shape.setRotation(player.shape.getRotation());
				shotSound.play();
			}
		}
		else {
			player.shotState = 0;
		}

		if (sf::Joystick::isButtonPressed(0, 0)) {
			player.speed = 1;
			if (!engineSound.getLoop()) {
				engineSound.setLoop(true);
			}
			if (engineSound.getStatus() != sf::SoundSource::Status::Playing) {
				engineSound.play();
			}
		}
		else {
			player.speed = 0;
			engineSound.setLoop(false);
			engineSound.stop();
		}

		if (sf::Joystick::isButtonPressed(0, 4) && player.hitCooldown == 0) {
			/*player.fireSpeed -= 0.2f;
			player.hitCooldown += 0.5f;*/
		}
		if (sf::Joystick::isButtonPressed(0, 5) && player.hitCooldown == 0) {
			/*player.fireSpeed += 0.2f;*/
			player.hitCooldown += 0.1f;
			for (unsigned int i = 0; i < meteorites.size(); i++) {
				meteorites[i].currentHP = std::max(0, meteorites[i].currentHP - 1);
			}
		}

		//std::cout << "FireSpeed: " << player.fireSpeed << "\n";

		player.shape.move(direction.x * dt * player.NormalSpeed * (1 + (player.EngineBonus*player.speed)), direction.y * dt * player.NormalSpeed * (1 + (player.EngineBonus * player.speed)));

		/* Update Player */
		player.update();

		return true;
	}

	bool GameState::Update(float dt) {

		/* Spawn Meteorites */
		if (sse::random::randomReal<float>(0, 1) <= METEORITE_SPAWN_CHANCE) {
			meteorites.push_back(basicMeteorite);
			float spawnAngle = sse::random::randomReal<float>(0, 360);
			sf::Vector2f spawnVec(cos(spawnAngle / 180.f * sse::math::PI), sin(spawnAngle / 180.f * sse::math::PI));
			meteorites[meteorites.size() - 1].shape.setPosition(player.shape.getPosition().x + spawnVec.x * METEORITE_SPAWN_DISTANCE * TILE_SCALING, player.shape.getPosition().y + spawnVec.y * METEORITE_SPAWN_DISTANCE * TILE_SCALING);
			meteorites[meteorites.size() - 1].movementAngle = fmod(spawnAngle + 180.f, 360);
			meteorites[meteorites.size() - 1].rotationSpeed = sse::random::randomReal<float>(METEORITE_ROTATIONSPEED_MIN, METEORITE_ROTATIONSPEED_MAX);
			meteorites[meteorites.size() - 1].speed = sse::random::randomReal<float>(METEORITE_SPEED_MIN, METEORITE_SPEED_MAX);
			meteorites[meteorites.size() - 1].maxHP = sse::random::randomInteger<int>(METEORITE_HP_MIN, METEORITE_HP_MAX);
			meteorites[meteorites.size() - 1].currentHP = meteorites[meteorites.size() - 1].maxHP;
			meteorites[meteorites.size() - 1].SpriteID = sse::random::randomInteger<int>(0, METEORITE_SPRITE_COUNT - 1);
			meteorites[meteorites.size() - 1].shape.setTextureRect(getMeteoriteSprite(meteorites[meteorites.size() - 1].SpriteID));
		}

		/* Update Meteorites */
		for (unsigned int i = 0; i < meteorites.size(); i++) {
			meteorites[i].shape.rotate(meteorites[i].rotationSpeed * dt);
			sf::Vector2f transformation(cos(meteorites[i].movementAngle / 180.f * sse::math::PI)*meteorites[i].speed*dt, sin(meteorites[i].movementAngle / 180.f * sse::math::PI)*meteorites[i].speed*dt);
			meteorites[i].shape.move(transformation);
		}
		/* Update Shots */
		for (unsigned int i = 0; i < shots.size(); i++) {
			float rot = shots[i].shape.getRotation();
			shots[i].shape.move(shots[i].speed * dt * cos(rot / 180 * sse::math::PI), shots[i].speed * dt * sin(rot / 180 * sse::math::PI));
		}

		/* Update Cooldowns for all entities */
		player.shotCooldown -= dt;
		if (player.shotCooldown <= 0.f) {
			player.shotCooldown = 0.f;
		}
		player.playerWalkCooldown -= dt;
		if (player.playerWalkCooldown <= 0.f) {
			player.playerWalkCooldown = 0.f;
		}
		player.hitCooldown -= dt;
		if (player.hitCooldown <= 0.f) {
			player.hitCooldown = 0.f;
		}
		for (unsigned int i = 0; i < meteorites.size(); i++) {
			meteorites[i].lifetime += dt;
		}
		for (unsigned int i = 0; i < shots.size(); i++) {
			shots[i].lifetime += dt;
		}

		/* Collision detection */
		/*
		for (unsigned int x = 0; x < area.x(); x++) {
		for (unsigned int y = 0; y < area.y(); y++) {
		collisionShape.setPosition((x + INFO_WIDTH) * 16 * TILE_SCALING, y * 16 * TILE_SCALING + 8 * TILE_SCALING);
		if (area.at(x, y) != 0 && area.at(x, y) != 1) {
		collisionBounds = collisionShape.getGlobalBounds();

		// Detect for every player
		for (unsigned int i = 0; i < players.size(); i++) {
		playerBounds = players[i].shape.getGlobalBounds();
		if (collisionBounds.intersects(playerBounds)) {

		//https://www.youtube.com/watch?v=l2iCYCLi6MU for reference
		//Delta vector between the centers of both axis-aligned boxes
		sf::Vector2f translation(
		(collisionBounds.left + collisionBounds.width / 2) - (playerBounds.left + playerBounds.width / 2),
		(collisionBounds.top + collisionBounds.height / 2) - (playerBounds.top + playerBounds.height / 2)
		);

		sf::Vector2f colliderHalfSize(collisionBounds.width / 2, collisionBounds.height / 2);
		sf::Vector2f playerHalfSize(playerBounds.width / 2, playerBounds.height / 2);

		// Negative intersection values indicate a collision
		float intersectX = abs(translation.x) - (colliderHalfSize.x + playerHalfSize.x);
		float intersectY = abs(translation.y) - (colliderHalfSize.y + playerHalfSize.y);

		// Move the player along the axis of least intersection
		if (intersectX > intersectY) {
		if (translation.x > 0) {
		players[i].shape.move(intersectX, 0.f);
		}
		else {
		players[i].shape.move(-intersectX, 0.f);
		}

		}
		else {
		if (translation.y > 0) {
		players[i].shape.move(0, intersectY);
		}
		else {
		players[i].shape.move(0, -intersectY);
		}

		}
		}
		}
		}
		}
		*/

		/* Powerup pickup */

		/* Damage player */

		/* Clear Entities if their lifetime exceeds the maximum */
		meteorites.erase(std::remove_if(meteorites.begin(), meteorites.end(), [](Meteorite val) { return val.lifetime > METEORITE_MAXIMUM_LIFETIME; }), meteorites.end());
		shots.erase(std::remove_if(shots.begin(), shots.end(), [](Shot val) { return val.lifetime >= SHOT_MAXIMUM_LIFETIME; }), shots.end());
		
		return true;
	}

	bool GameState::Render(float dt) {

		m_data->window.clear(sf::Color(20, 20, 20, 255));	//sf::Color(40, 60, 90, 255)

		/* Render Player */
		player.shape.setTextureRect(getPlayerSprite(player.speed, player.shotState));
		m_data->window.draw(player.shape);

		/* Render Shots */
		for (unsigned int i = 0; i < shots.size(); i++) {
			m_data->window.draw(shots[i].shape);
		}

		/* Render Meteorites */
		for (unsigned int i = 0; i < meteorites.size(); i++) {
			m_data->window.draw(meteorites[i].shape);
			healthBar.setTextureRect(getHealthBarSprite(meteorites[i].currentHP, meteorites[i].maxHP, meteorites[i].isProtected));
			healthBar.setPosition(meteorites[i].shape.getPosition());
			healthBar.move(0, -15 * TILE_SCALING);
			m_data->window.draw(healthBar);
		}

		/* Draw Text */
		xText.setString("FPS: " + std::to_string((int)(1.f / dt)) + ", Shots: " + std::to_string(shots.size()) + ", Meteorites: " + std::to_string(meteorites.size()));
		m_data->window.draw(xText);

		m_data->window.display();
		return true;
	}
}