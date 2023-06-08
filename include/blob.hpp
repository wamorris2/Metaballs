#ifndef _BLOB_H_
#define _BLOB_H_
#include <SFML/System/Vector2.hpp>

class blob {
	public:
		float radius;
		sf::Vector2f position;
		sf::Vector2f velocity;

		blob(float radius, sf::Vector2f position, sf::Vector2f velocity) : radius(radius), position(position), velocity(velocity) {}
		blob() : radius(50.0f), position(sf::Vector2f()), velocity(sf::Vector2f()) {};
};

#endif