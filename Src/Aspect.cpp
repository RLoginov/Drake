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
}

Physics::Physics(Entity381 * ent): Aspect(ent) {

}

Physics::~Physics(){

}


void Physics::Tick(float dt)
{
  if(this->entity->meshfile == "ogrehead.mesh")
  {

    entity->speed = 750;
    entity->pos += entity->vel * dt;
    entity->heading = entity->desiredHeading;
  }

  entity->vel = Ogre::Vector3(cos(entity->heading) * entity->speed, 0, sin(entity->heading) * entity->speed);

}

AI::AI(Entity381 * ent): Aspect(ent) {
	hitTimer = 1;
}

AI::~AI()
{

}


void AI::Tick(float dt)
{
 Ogre::Vector3 diff;
 hitTimer -= dt;

  if(this->entity->meshfile == "ogrehead.mesh")
  {
	//std::cout << entity->leader->pos << std::endl;
    diff = entity->leader->ogreSceneNode->getPosition() - entity->pos;
    entity->desiredHeading = atan2(diff.z, diff.x);
    entity->desiredSpeed = entity->maxSpeed;

    //if(hitTimer < 0)
	checkForCollision(hitTimer);
  }
}

void AI::checkForCollision(float dt)
{
     Ogre::Real xDist = entity->pos.x - entity->leader->ogreSceneNode->getPosition().x;
     Ogre::Real zDist = entity->pos.z - entity->leader->ogreSceneNode->getPosition().z;


     if(xDist <= 10.0 && xDist >= -10.0 && zDist >= -10.0 && zDist <= 10.0)
     {
       std::cout << "AAAAAAAAAAAA" << std::endl;
       entity->hit = true;
     }

}
