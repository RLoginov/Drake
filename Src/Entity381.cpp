/*
 * Entity381.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#include "../Inc/Entity381.h"
#include "../Inc/Types.h"
#include "../Inc/Aspect.h"


unsigned int Entity381::nextId = 0;

Entity381::Entity381(EntityType entType, Ogre::Vector3 pos, float heading)
{
	entityType = entType;
	leader = NULL;
	point = Ogre::Vector3::ZERO;
	shouldAdd = false;
	isNewComm = false;
	isFollowComm = false;

	this->pos = pos;
	this->heading = heading;
	this->vel = Ogre::Vector3::ZERO;
	this->speed = 0;
	this->isSelected = false;
	this->entityId = Entity381::nextId++;
	this->playSound = false;
	this->audioId = 0;

	Renderable *r = new Renderable(this);
	Physics *p = new Physics(this);
	AI *ai = new AI(this);


	this->aspects.clear();
	this->aspects.push_front(r);
	this->aspects.push_front(p);
	this->aspects.push_front(ai);

	DefaultInit();
}

Entity381::~Entity381(){

}

void Entity381::DefaultInit(){
	this->acceleration = 0.5f;
	this->turnRate = 0.5f;

	this->maxSpeed = 100;
	this->minSpeed = 0;

	this->desiredHeading = 0;
	this->desiredSpeed = 0;

	this->meshfile = "cube.mesh";
	this->soundFile = "Dragon Roar.wav";
}

void Entity381::Tick(float dt){
	for(std::list<Aspect*>::const_iterator ai = aspects.begin(); ai != aspects.end(); ++ai){
		(*ai)->Tick(dt);
	}

	this->pos = this->ogreSceneNode->getPosition();
}


Dragon::Dragon(Ogre::Vector3 pos, float heading) : Entity381(EntityType::DRAKE, pos, heading){
	this->meshfile = "dragon.mesh";
	this->acceleration = 1.0f;
	this->turnRate = 0.1f;
}

Dragon::~Dragon()
{

}


Building::Building(Ogre::Vector3 pos, float heading) : Entity381(EntityType::DRAKE, pos, heading)
{
	this->meshfile = "cube.mesh";
	this->acceleration = 0.0f;
	this->turnRate = 0.0f;
}

Building::~Building()
{

}


Missile::Missile(Ogre::Vector3 pos, float heading) : Entity381(EntityType::DRAKE, pos, heading)
{
	this->meshfile = "ogrehead.mesh";
	this->acceleration = 50.0f;
	this->turnRate = 0.9f;

}

Missile::~Missile()
{

}

Blank::Blank(Ogre::Vector3 pos, float heading) : Entity381(EntityType::BLANK, pos, heading)
{
	this->meshfile = "";
}

Blank::~Blank()
{

}
