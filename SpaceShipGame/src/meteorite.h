#pragma once
#include <SFML/Graphics.hpp>


class Meteorite {

public:

	sf::RectangleShape shape;

	bool isProtected = false;
	int maxHP = METEORITE_HP_MIN;
	int currentHP = METEORITE_HP_MIN;
	float speed = METEORITE_SPEED_MIN;
	float impactDamage = METEORITE_IMPACT_DAMAGE;
	float movementAngle = 0.f;
	float rotationSpeed = METEORITE_ROTATIONSPEED_MIN;
	unsigned int SpriteID = 0;
	float lifetime = 0.f;
	
	Meteorite () {

	}
	~Meteorite() {

	}

};