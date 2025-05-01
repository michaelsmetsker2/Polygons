/** -------------------------------------
*
* @file		spaceObject.h
* @author	Michael Metsker
* @version	1.1
*
* SpaceObject class implementation
*
* --------------------------------------------*/

#include "spaceObject.h"
#include "constants.h"
#include <iostream>

SpaceObject::SpaceObject() {

	type = ASTEROID;
	setLocation(WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2);
	setVelocity(0, 0);
	setAngle(sf::degrees(0));
	setRadius(20);
	timesDrawn = 0;
}

SpaceObject::SpaceObject(SpaceObjType type, float radius,
	sf::Vector2<float> location, sf::Vector2<float> velocity, sf::Angle angle) {

	timesDrawn = 0;

	if (type == SpaceObjType::PHOTON_TORPEDO || type == SpaceObjType::SHIP)
		this->type = type;
	else
		this->type = ASTEROID;

	if (radius > 0 || radius <= WINDOW_HEIGHT / 2.0)
		setRadius(radius);
	else
		setRadius(5);

	if (location.x >= 0 && location.x <= WINDOW_HEIGHT &&
		location.y >= 0 && location.y <= WINDOW_WIDTH)
		setLocation(location.x, location.y);
	else
		setLocation(0, 0);

	if (velocity.x >= -100 && velocity.x <= 100 &&
		velocity.y >= -100 && velocity.y <= 100)
		setVelocity(velocity.x, velocity.y);
	else
		setVelocity(0, 0);

	if (angle >= sf::degrees(0) && angle <= sf::degrees(360))
		setAngle(angle);
	else
		setAngle(sf::degrees(0));
}

bool SpaceObject::setRadius(float radius) {

	if (radius < 0 || radius > WINDOW_HEIGHT / 2) {
		return false;
	}
	else {
		this->radius = radius;
		return 1;
	}
}

bool SpaceObject::setLocation(float x, float y) {

	if (x >= 0 && x <= WINDOW_HEIGHT) {
		location.x = x;
	}
	else if (x < 0) {
		//wraps to bottom of screen
		int counter = 0;
		while (x < 0) {
			x += WINDOW_HEIGHT;
			if (++counter == 50) {
				return false;
			}
		}
		location.x = x;
	}
	else if (x > WINDOW_HEIGHT) {
		//wraps to top of screen
		int counter = 0;
		while (x > WINDOW_HEIGHT) {
			x -= WINDOW_HEIGHT;
			if (++counter == 50) {
				return false;
			}
		}
		location.x = x;
	}
	else {
		return false;
	}

	if (y >= 0 && y <= WINDOW_WIDTH) {
		location.y = y;
	}
	else if (y < 0) {
		//wraps to right of screen
		int counter = 0;
		while (y < 0) {
			y += WINDOW_WIDTH;
			if (++counter == 50) {
				return false;
			}
		}
		location.y = y;
	}
	else if (y > WINDOW_WIDTH) {
		//wraps to left of screen
		int counter = 0;
		while (y > WINDOW_WIDTH) {
			y -= WINDOW_WIDTH;
			if (++counter == 50) {
				return false;
			}
		}
		location.y = y;
	}
	else {
		return false;
	}
	return true;
}

bool SpaceObject::setVelocity(float velocityX, float velocityY) {

	if (velocityX > -100 && velocityX < 100 && velocityY > -100 && velocityY < 100) {
		velocity.x = velocityX;
		velocity.y = velocityY;
	}
	else {
		return false;
	}
	return true;
}

bool SpaceObject::setAngle(sf::Angle angle) {

	if (angle >= sf::degrees(0) && angle <= sf::degrees(360)) {
		this->angle = angle;
	}
	else {
		return false;
	}
	return true;
}

void SpaceObject::changeAngle(sf::Angle deltaAngle) {

	if (type == SHIP) {
		angle += deltaAngle;
	}
	while (angle.asDegrees() > 360) {
		angle -= sf::degrees(360);
	}
	while (angle.asDegrees() < 0) {
		angle += sf::degrees(360);
	}
}

double SpaceObject::getRadius() const
{
	return radius;
}

sf::Vector2<float> SpaceObject::getLocation() const
{
	return location;
}

sf::Vector2<float> SpaceObject::getVelocity() const
{
	return velocity;
}

sf::Angle SpaceObject::getAngle() const
{
	return angle;
}

int SpaceObject::getTimesDrawn() const
{
	return timesDrawn;
}

void SpaceObject::updatePosition() {
	setLocation(location.x + velocity.x, location.y + velocity.y);
}

void SpaceObject::applyThrust(const float thrust) {

	float forcex = cos((angle.asRadians() - PI / 2)) * thrust;
	float forcey = sin((angle.asRadians() - PI / 2)) * thrust;

	setVelocity(forcex + velocity.x, forcey + velocity.y);
}

void SpaceObject::dumpData() const {
	std::cout << type << std::endl;
	std::cout << location.x << " " << location.y << std::endl;
	std::cout << velocity.x << " " << velocity.y << std::endl;
	std::cout << angle.asDegrees() << std::endl;
	std::cout << radius << std::endl;
}

void SpaceObject::explode()
{
	if (type == SHIP) {
		type = SHIP_EXPLODING;
		setVelocity(0, 0);
		setRadius(0);
	}
}

void SpaceObject::draw(sf::RenderWindow& win) {

	timesDrawn++;

	switch (type)
	{
		case SHIP:
			drawShip(win);
			break;
		case ASTEROID:
			drawAsteroid(win);
			break;
		case PHOTON_TORPEDO:
			drawAsteroid(win);
			break;
		case SHIP_EXPLODING:
			drawExplodingShip(win);
			break;
		defualt:
			break;
	}
}

void SpaceObject::drawAsteroid(sf::RenderWindow& win) const {
	int points = 5;
	sf::CircleShape shape(radius, points); //radius from our SpaceObject

	sf::Vector2f midpoint(radius, radius);
	shape.setOrigin(midpoint);

	shape.setFillColor(sf::Color(0, 0, 0));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(255, 255, 255));

	//apply our object position to the graphics object and draw it
	shape.setPosition(location);
	shape.setRotation(angle);

	win.draw(shape);
}

void SpaceObject::drawShip(sf::RenderWindow& win) const {
	// draw ship
	sf::ConvexShape shipShape;
	shipShape.setPointCount(3);
	shipShape.setPoint(0, sf::Vector2f(10, 0));
	shipShape.setPoint(1, sf::Vector2f(0, 25));
	shipShape.setPoint(2, sf::Vector2f(20, 25));

	sf::Vector2f midpoint(10, 15);
	shipShape.setOrigin(midpoint);

	shipShape.setFillColor(sf::Color(0, 0, 0));
	shipShape.setOutlineThickness(1);
	shipShape.setOutlineColor(sf::Color(255, 255, 255));

	shipShape.setPosition(location);
	shipShape.setRotation(angle);
	win.draw(shipShape);
}

void SpaceObject::drawExplodingShip(sf::RenderWindow& win) {

	if (radius < 30) {

		sf::CircleShape explosion(radius);

		sf::Vector2f midpoint(radius, radius);
		explosion.setOrigin(midpoint);

		explosion.setPosition(location);

		explosion.setFillColor(sf::Color(0, 0, 0));
		explosion.setOutlineThickness(1);
		explosion.setOutlineColor(sf::Color(255, 255, 255));

		win.draw(explosion);

		radius++;
	}
	else {
		type = SHIP_GONE;
	}
}
