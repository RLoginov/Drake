/*
 * Command.h
 *
 *  Created on: Mar 28, 2017
 *      Author: Tyler Goffinet, Robert Loginov
 *      tylergoffinet@cox.net, rloginov@nevada.unr.edu
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "../Inc/EntityMgr.h"
#include <OgreVector3.h>

class Entity381;

class Command {

public:
	Command(Entity381* ent);
	virtual ~Command();

	virtual void print();
	virtual void init();
	virtual void tick(float dt);
	virtual bool done();

	bool isDone;
	Entity381* entity;
	float MIN_DISTANCE_THRESHOLD;
	float HEADING_TOLERANCE;
};

class MoveTo: public Command {

public:
	MoveTo(Entity381* ent, Ogre::Vector3 location);
	~MoveTo();

	void print();
	void init();
	void tick(float dt);
	bool done();

	Ogre::Vector3 targetLocation;
};

class Follow: public Command {

public:
	Follow(Entity381* ent, Entity381* leader);
	~Follow();

	void print();
	void init();
	void tick(float dt);
	bool done();

	Entity381* leader;
};




#endif /* COMMAND_H_ */
