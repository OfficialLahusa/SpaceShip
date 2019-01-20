#pragma once
#include <SFML/Graphics.hpp>

class Shot {

public:

	sf::RectangleShape shape;
	float speed = 1000.f;
	float lifetime = 0.f;

	Shot() {
		
	}
	~Shot() {

	}

};