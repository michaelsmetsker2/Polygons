/** -------------------------------------
*
* @file		game.cpp
* @author	Michael Metsker
* @version	1.0
*
* main game function for an asteroids clone
*
* --------------------------------------------*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include "constants.h"
#include "spaceObject.h"
#include <cmath>

sf::Vector2f getRandomLocation();
sf::Vector2f getRandomVelocity();
bool objectsIntersect(SpaceObject*, SpaceObject*);
void splitAsteroid(SpaceObject* (&asteroids)[MAX_ASTEROIDS], int);

int main()
{
	//create SpaceObjects
	SpaceObject* asteroids[MAX_ASTEROIDS] = { nullptr };
	SpaceObject* photons[MAX_PHOTONS] = { nullptr };
	SpaceObject* ship = new SpaceObject(SHIP, 26, { WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2 }, { -1,2 }, sf::degrees(90));

	srand(static_cast<unsigned int>(time(NULL)));

	//initialize asteroids
	for (int i = 0; i < 5; i++) {
		asteroids[i] = new SpaceObject(ASTEROID, ASTEROID_SIZE, getRandomLocation(), getRandomVelocity(), sf::degrees(90));
	}

	//create graphics window
	sf::RenderWindow window(
		sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
		"Polygons!",
		sf::Style::Titlebar | sf::Style::Close
	);
	window.setFramerateLimit(60);

	//game loop
	while (window.isOpen())
	{
		//handle user input (events and keyboard keys being pressed) ----

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			ship->changeAngle(sf::degrees(-TURN_SPEED));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			ship->changeAngle(sf::degrees(TURN_SPEED));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
			ship->applyThrust(0.05f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
			int i = 0;
			while (photons[i] && i < MAX_PHOTONS) {
				i++;
			}
			if (i < MAX_PHOTONS) {
				photons[i] = new SpaceObject(PHOTON_TORPEDO, 2,
					ship->getLocation(), ship->getVelocity(), ship->getAngle()
				);
				photons[i]->applyThrust(5);
			}
		}

		// update game objects ------------------------------------------

		for (SpaceObject* ast : asteroids) {
			if (ast) {
				ast->updatePosition();
			}
		}
		for (int i = 0; i < MAX_PHOTONS; i++) {
			if (photons[i]) {
				photons[i]->updatePosition();
				if (photons[i]->getTimesDrawn() > PHOTON_LIFESPAN) {
					delete photons[i];
					photons[i] = nullptr;
				}
			}
		}
		ship->updatePosition();

		// checks for collision ------------------------------------------

		//see if torpedo hits
		for (int i = 0; i < MAX_PHOTONS; i++) {
			if (photons[i]) {
				for (int j = 0; j < MAX_ASTEROIDS; j++) {
					if (asteroids[j] && photons[i]) {

						if (objectsIntersect(photons[i], asteroids[j])) {
							delete photons[i];
							photons[i] = nullptr;

							splitAsteroid(asteroids, j);
						}
					}
				}
			}
		}

		//see if ship is hit
		for (SpaceObject* ast : asteroids) {
			if (ast) {
				if (objectsIntersect(ship, ast)) {
					ship->explode();
				}
			}
		}

		// draw new frame ------------------------------------------------
		window.clear();
		for (SpaceObject* ast : asteroids) {
			if (ast) {
				ast->draw(window);
			}
		}
		for (SpaceObject* torp : photons) {
			if (torp) {
				torp->draw(window);
			}
		}
		ship->draw(window);
		window.display();
	}

	return 0;
}

/*
*
* generates a random point from (0,0) to (WINDOW_WIDTH, WINDOW_HEIGHT)
*
* @return a vector2f corrolating to a location on the window
*
*/
sf::Vector2f getRandomLocation() {

	float randomX = rand() % (WINDOW_WIDTH + 1);
	float randomY = rand() % (WINDOW_HEIGHT + 1);
	return { randomX, randomY };
}

/*
*
* generates a random point from (-1,-1) to (1,1)
*
* @return a vector2f with x and y values between -1 and 1
*
*/
sf::Vector2f getRandomVelocity() {

	float randomX = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
	float randomY = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
	return { randomX, randomY };
}

/*
*
* checks to see if two SpaceObjects are currently colliding
*
* @param obj1 - first SpaceObject
* @param obj2 - second SpaceObject
*
* @return true if SpaceObjects are colliding
*
*/
bool objectsIntersect(SpaceObject* obj1, SpaceObject* obj2) {

	sf::Vector2f pos1 = obj1->getLocation();
	sf::Vector2f pos2 = obj2->getLocation();

	float distance = static_cast<float>(sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2)));

	if (distance <= obj1->getRadius() + obj2->getRadius()) {
		return true;
	}
	return false;
}

/*
* 
* splits an asteroid into two smaller ones and deletes the original
* 
* @param asteroids - array of asteroids
* @param i - index of original asteroid in asteroids array
* 
*/
void splitAsteroid(SpaceObject* (&asteroids)[MAX_ASTEROIDS], int i) {

	if (asteroids[i]->getRadius() >= ASTEROID_SIZE * .25) {

		int j = 0;
		while (asteroids[j] && j < MAX_ASTEROIDS) {
			j++;
		}
		if (j < MAX_ASTEROIDS) {
			asteroids[j] = new SpaceObject(ASTEROID,
				asteroids[i]->getRadius() / 2, asteroids[i]->getLocation(),
				getRandomVelocity(), sf::degrees(90)
			);
		}
		j = 0;
		while (asteroids[j] && j < MAX_ASTEROIDS) {
			j++;
		}
		if (j < MAX_ASTEROIDS) {
			asteroids[j] = new SpaceObject(ASTEROID,
				asteroids[i]->getRadius() / 2, asteroids[i]->getLocation(),
				getRandomVelocity(), sf::degrees(90)
			);
		}
	}
	delete asteroids[i];
	asteroids[i] = nullptr;
}