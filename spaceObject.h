/** -------------------------------------
*
* @file		spaceObject.h
* @author	Michael Metsker
* @version	1.1
*
* SpaceObject class definition
*
* --------------------------------------------*/

#ifndef SPACE_OBJECT_H
#define SPACE_OBJECT_H

#include<SFML/Graphics.hpp>

struct Point {
	double x;
	double y;
};

enum SpaceObjType { SHIP, ASTEROID, PHOTON_TORPEDO, SHIP_EXPLODING, SHIP_GONE };

class SpaceObject {

public:
	/**
	*
	* Initializes all member variables
	*
	*/
	SpaceObject();

	/**
	*
	* Initialize all member variables to the given values,
	* if values are invalid, use reasonable default values.
	*
	* @param type - type of object
	* @param radius - size of object's radius
	* @param location - x and y coordonits of the objects position
	* @param velocity - object's velocity in x and y positions
	* @param angle - objects angle
	*
	*/
	SpaceObject(SpaceObjType type, float radius,
		sf::Vector2<float> location, sf::Vector2<float> velocity, sf::Angle angle);

	//=============================================
	//mutators

	/**
	* sets the objects radius
	*
	* @param radius - new radius value
	*
	* @return true - if radius has been successfully update
	* @return false - if the new radius is in invalid
	*
	*/
	bool setRadius(float radius);

	/**
	* sets the objects position, if it is outside the screen it wraps it to the other side
	*
	* @param x - new vertical position
	* @param y - new horizontal position
	*
	* @return true - if position has been successfully update
	* @return false - if the new position is in invalid
	*
	*/
	bool setLocation(float x, float y);

	/**
	* sets the objects velocity, capped at -100, 100
	*
	* @param velocityX - new vertical velocity
	* @param velocityY - new horizontal velocity
	*
	* @return true - if position has been successfully update
	* @return false - if the new position is in invalid
	*
	*/
	bool setVelocity(float velocityX, float velocityY);

	/**
	*
	* sets the angle of the object
	*
	* @param angle - new angle in degrees
	*
	* @return true - if the angle has been updated successfully
	* @return false - if the given angle is invalid
	*
	*/
	bool setAngle(sf::Angle angle);

	/**
	*
	* adjusts the angle by a given ammount if the object is a ship
	*
	* @param deltaAngle - change in angle in degrees
	*
	*/
	void changeAngle(sf::Angle deltaAngle);

	//============================================
	//accessors

	/**
	*
	* @return radius
	*
	*/
	double getRadius() const;

	/**
	*
	* @return location
	*
	*/
	sf::Vector2<float> getLocation() const;

	/**
	*
	* @return velocity
	*
	*/
	sf::Vector2<float> getVelocity() const;

	/**
	*
	* @return angle in degrees
	*
	*/
	sf::Angle getAngle() const;

	/*
	* 
	* @return ammount of times drawn
	* 
	*/
	int getTimesDrawn() const;

	//============================================
	//others

	/**
	*
	* adds velocity to the current position of the object
	*
	*/
	void updatePosition();

	/**
	*
	* Adds thrust to the direction the object is facing
	*
	* @param thrust - ammount of thrust to be applied
	* 
	*/
	void applyThrust(const float thrust);

	/**
	*
	* draws the spaceObject on the given window
	*
	* @param win - window to draw the object on
	*
	*/
	void draw(sf::RenderWindow&);

	/**
	*
	* Writes out objects data to tha console for testing
	*
	*/
	void dumpData() const;

	/*
	*
	* changes type to exploding ship if the type is already ship and sets radius to 1
	* 
	*/
	void explode();

private:
	SpaceObjType type;	  //type of object
	sf::Vector2<float> location;	  //current location (x,y)
	sf::Vector2<float> velocity;	  //current velocity (in pixels/frame)
	sf::Angle angle;	  //angle object is facing
	float radius;        //gross radius of object (for collision detection)
	int timesDrawn;		//# of times drawn

	/**
	*
	* Draws the SpaceObject on the given window as an asteroid
	*
	* @param win - the window on which we’ll draw the asteroid
	*
	*/
	void drawAsteroid(sf::RenderWindow&) const;

	/**
	*
	* Draws the SpaceObject on the given window as ship
	*
	* @param win - the window on which we’ll draw the ship
	*
	*/
	void drawShip(sf::RenderWindow&) const;

	/**
	*
	* Draws the SpaceObject on the given window as an exploding ship
	* changes the type to ship_gone when radius is 30
	*
	* @param win - the window on which we’ll draw the exploding ship
	*
	*/
	void drawExplodingShip(sf::RenderWindow&);

};

#endif /* SPACEOBJECT_H */