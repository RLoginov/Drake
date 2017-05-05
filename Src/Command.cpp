/*
 * Command.cpp
 *
 *  Created on: April 2, 2017
 *      Author: Tyler Goffinet, Robert Loginov
 *      tylergoffinet@cox.net, rloginov@nevada.unr.edu
 */

#include <unistd.h>


#include "../Inc/Command.h"

Command::Command(Entity381* ent)
{
  isDone = false;
  entity = ent;
  MIN_DISTANCE_THRESHOLD = 0.0f;
  HEADING_TOLERANCE = 0.5f;
}

Command::~Command()
{
}

void Command::init()

{

}

void Command::tick(float dt)
{

}

void Command::print()
{

}

bool Command::done()
{
	return false;
}

MoveTo::MoveTo(Entity381* ent, Ogre::Vector3 location) : Command(ent)
{
	targetLocation = location;
	MIN_DISTANCE_THRESHOLD = 100.0f;
}

MoveTo::~MoveTo()
{

}

void MoveTo::init()
{

}

void MoveTo::print(){
	std::cout << "Move(" << targetLocation.x << "," << targetLocation.z << ") ";
}

void MoveTo::tick(float dt)
{
  // Stop boat if in stopping distance
  if (entity->pos.distance(targetLocation) <= MIN_DISTANCE_THRESHOLD){
	  entity->desiredSpeed = 0.0f;
	  isDone = true;
	  return;
  }

  Ogre::Radian newHeading = Ogre::Math::ATan2(
											  (targetLocation.z - entity->pos.z),
											  (targetLocation.x - entity->pos.x)
											  );

  entity->desiredHeading = newHeading.valueRadians();

  if (entity->desiredHeading - HEADING_TOLERANCE < entity->heading &&
	  entity->desiredHeading + HEADING_TOLERANCE > entity->heading)
  {
	  entity->desiredSpeed = entity->maxSpeed;
  }
}

bool MoveTo::done()
{
  return isDone;
}

Follow::Follow(Entity381* ent, Entity381* leader) : Command(ent)
{
  this->leader = leader;
  MIN_DISTANCE_THRESHOLD = 100.0f;
}

Follow::~Follow()
{

}

void Follow::init()
{

}

void Follow::print()
{
	std::cout << "Follow(" << leader->entityId << ")" ;
}

void Follow::tick(float dt)
{
  Ogre::Radian newHeading;

  // Stop boat if in stopping distance
  if (entity->pos.distance(leader->pos) <= MIN_DISTANCE_THRESHOLD){
	  entity->speed = 0.0f;
	  return;
  }


  newHeading = Ogre::Math::ATan2(
								  (leader->pos.z - entity->pos.z),
								  (leader->pos.x - entity->pos.x)
								 );


  entity->desiredHeading = newHeading.valueRadians();

  if (entity->desiredHeading - HEADING_TOLERANCE < entity->heading &&
	  entity->desiredHeading + HEADING_TOLERANCE > entity->heading)
  {
	  entity->desiredSpeed = entity->maxSpeed;
  }
}

bool Follow::done()
{
  return false;
}

