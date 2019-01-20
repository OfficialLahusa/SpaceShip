#pragma once
#include <SFML/Graphics.hpp>
#include "def.h"

class Player {
private:
	unsigned int m_skin;
	unsigned int m_x, m_y;
	unsigned int m_id;

public:
	unsigned int playerFrame = 0;
	float playerWalkCooldown = 0.f;
	float shotCooldown = 0.f;
	float hitCooldown = 0.f;
	unsigned int lives = HP_DEFAULT;
	unsigned int shotState = 0;
	unsigned int speed = 0;
	float NormalSpeed = 100.f;
	float EngineBonus = 3.f;
	float fireSpeed = 3.f;

	sf::RectangleShape shape;

	Player(unsigned int newID = 0, unsigned int newSkin = 0) {

		m_id = newID;
		m_skin = newSkin;
		shape.setSize(sf::Vector2f(128 * TILE_SCALING, 128 * TILE_SCALING));
		shape.setPosition(128 * TILE_SCALING, 128 * TILE_SCALING);
		
	}

	void update() {
		m_x = ((shape.getPosition().x - 16 * INFO_WIDTH * TILE_SCALING) + 6 * TILE_SCALING) / (16 * TILE_SCALING);
		m_y = (shape.getPosition().y + 2 * TILE_SCALING) / (16 * TILE_SCALING);
	}

	void move(sf::Vector2f movement) {
		shape.move(movement);
	}

	bool isDead() const {
		if (lives <= 0) {
			return true;
		}
		else {
			return false;
		}
	}

	sf::Vector2i getTilePosition() const {
		return sf::Vector2i(m_x, m_y);
	}

	unsigned int getSkin() const {
		return m_skin;
	}

	unsigned int getID() const {
		return m_id;
	}

	void setSkin(unsigned int newSkin = 0) {
		m_skin = newSkin;
	}

	unsigned int setTilePosition(unsigned int tx, unsigned int ty) {
		m_x = tx;
		m_y = ty;
		shape.setPosition((tx + INFO_WIDTH) * 16 * TILE_SCALING, ty * 16 * TILE_SCALING);
	}

};