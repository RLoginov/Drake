/*
 * Entity.h
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#ifndef ENTITY381_H_
#define ENTITY381_H_

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <list>

#include "../Inc/Types.h"

class Aspect;
class Command;

class Entity381 {

private:

	static unsigned int nextId;

public:

	Entity381(EntityType entityType, Ogre::Vector3 pos, float heading);
	~Entity381();
	void Tick(float dt);
	void DefaultInit();

	//static stuff
	float maxSpeed, minSpeed;
	float acceleration, turnRate;
	EntityType entityType;
	std::string meshfile;

	//command stuff
	Entity381* leader;
	Ogre::Vector3 point;
	bool shouldAdd;
	bool isNewComm;
	bool isFollowComm;

	//Ogre stuff
	Ogre::Vector3 pos;
	Ogre::Vector3 vel;
	Ogre::SceneNode *ogreSceneNode;
	Ogre::Entity* ogreEntity;

	//Engine stuff dynamic
	float speed, heading;
	float desiredSpeed, desiredHeading;

	unsigned int entityId;

	//selection
	bool isSelected;

	//sound
	bool didSelectSoundPlay;
	std::string soundFile;
	bool playSound;
	unsigned int audioId;

	std::list<Aspect*> aspects;

	bool hit;
};

class Dragon: public Entity381
{

public:
	Dragon(Ogre::Vector3 pos, float heading);
	~Dragon();
};

class Building: public Entity381
{

public:
	Building(Ogre::Vector3 pos, float heading);
	~Building();
};

class Missile: public Entity381
{

public:
	Missile(Ogre::Vector3 pos, float heading);
	~Missile();
};

class Blank: public Entity381
{
public:
	Blank(Ogre::Vector3 pos, float heading);
	~Blank();
};

#endif /* ENTITY381_H_ */
