/*
 *      Author: sushil, Tyler Goffinet, Robert Loginov
 *      tylergoffinet@cox.net, rloginov@nevada.unr.edu
 */

#ifndef ASPECT_H_
#define ASPECT_H_

#include "../Inc/Entity381.h"
#include "../Inc/Types.h"
#include <engine.h>

class Aspect {

public:
	Aspect(Entity381* ent);
	virtual ~Aspect();

	virtual void Tick(float dt) = 0;
	Entity381 *entity;
	ASPECT_TYPE aspectType;
};

class Renderable: public Aspect {
public:
	Renderable(Entity381* ent);
	~Renderable();
	virtual void Tick(float dt);
};


class Physics: public Aspect {
public:
	Physics(Entity381* ent);
	~Physics();
	virtual void Tick(float dt);
};

class AI: public Aspect {
public:
	AI(Entity381* ent);
	~AI();
	virtual void Tick(float dt);

	void checkForCollision();

	//---------------------------

	float hitTimer;
};

#endif /* ASPECT_H_ */
