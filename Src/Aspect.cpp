/*
 * Aspect.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#include <Aspect.h>
#include <Utils.h>
#include <math.h>

using namespace std;

Aspect::Aspect (Entity381 * ent){
	this->entity = ent;
	this->aspectType = ASPECT_TYPE::NONE;
}

Aspect::~Aspect(){

}


Renderable::Renderable(Entity381 * ent): Aspect(ent){
	return;
}

Renderable::~Renderable(){

}

void Renderable::Tick(float dt)
{
  if(this->entity->meshfile == "ogrehead.mesh")
  {
    entity->ogreSceneNode->setPosition(entity->pos);
    entity->ogreSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
    entity->ogreSceneNode->yaw(Ogre::Radian(-entity->heading));
  }

  entity->ogreSceneNode->showBoundingBox(false);




	entity->ogreSceneNode->setPosition(entity->pos);
	entity->ogreSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
	entity->ogreSceneNode->yaw(Ogre::Radian(-entity->heading));
	if (entity->isSelected)
		entity->ogreSceneNode->showBoundingBox(true);
	else
		entity->ogreSceneNode->showBoundingBox(false);



}

Physics::Physics(Entity381 * ent): Aspect(ent) {

}

Physics::~Physics(){

}


void Physics::Tick(float dt)
{
  if(this->entity->meshfile == "ogrehead.mesh")
  {

    entity->speed += entity->acceleration*dt;
    entity->pos += entity->vel * dt;
    entity->heading = entity->desiredHeading;
  }

  entity->vel = Ogre::Vector3(cos(entity->heading) * entity->speed, 0, sin(entity->heading) * entity->speed);





	if(entity->speed < entity->desiredSpeed){
		//cout << endl << "gsdhgjd" << endl << endl;
		entity->speed += entity->acceleration *dt;
	}
	if(entity->speed > entity->desiredSpeed)
		entity->speed -= entity->acceleration *dt;

	//entity->speed = std::max(entity->minSpeed, std::min(entity->speed, entity->maxSpeed)); //clamp

	if(entity->heading < entity->desiredHeading)
		entity->heading += entity->turnRate * dt;
	if(entity->heading > entity->desiredHeading)
		entity->heading -= entity->turnRate * dt;

	//entity->heading = FixAngle(entity->heading); // between -pi and pi

	entity->vel = Ogre::Vector3(cos(entity->heading) * entity->speed, 0, sin(entity->heading) * entity->speed);
	entity->pos += entity->vel * dt;

}

AI::AI(Entity381 * ent): Aspect(ent) {

}

AI::~AI()
{

}


void AI::Tick(float dt)
{
 Ogre::Vector3 diff;

  if(this->entity->meshfile == "ogrehead.mesh")
  {
	//std::cout << entity->leader->pos << std::endl;
    diff = entity->leader->ogreSceneNode->getPosition() - entity->pos;
    entity->desiredHeading = atan2(diff.z, diff.x);
    entity->desiredSpeed = entity->maxSpeed;

    checkForCollision();
  }

}

void AI::checkForCollision()
{
  if(entity->pos.x == entity->leader->ogreSceneNode->getPosition().x ||
	 entity->pos.z == entity->leader->ogreSceneNode->getPosition().z)
  {
    std::cout << "AAAAAAAAAAAA" << std::endl;
  }
}
