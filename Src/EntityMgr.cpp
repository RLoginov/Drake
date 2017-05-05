/*
 * EntityMgr.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */


#include "../Inc/EntityMgr.h"
#include "../Inc/engine.h"


EntityMgr::EntityMgr(Engine *eng): Mgr(eng){
	entities.clear();
	selectedEntity = 0;
}

EntityMgr::~EntityMgr(){
	entities.clear();
}

void EntityMgr::tick(float dt){
	for (std::list<Entity381 *>::const_iterator it = entities.begin(); it != entities.end(); ++it){
		(*it)->Tick(dt);
		if ((*it)->isSelected)
			this->selectedEntity = (*it);
	}
}

void EntityMgr::init(){
	return;
}

void EntityMgr::loadLevel(){

}

void EntityMgr::stop(){
	return;
}

void EntityMgr::CreateOgreEntityAndNode(Entity381 *ent, Ogre::SceneNode* parent){

	if(ent) {
		ent->ogreEntity = engine->gfxMgr->ogreSceneManager->createEntity(ent->meshfile);
		ent->ogreSceneNode = parent->createChildSceneNode(ent->pos);
		ent->ogreSceneNode->attachObject(ent->ogreEntity);
		ent->ogreSceneNode->yaw(Ogre::Radian(ent->heading));
	}
}

Entity381* EntityMgr::CreateEntity(EntityType entityType, Ogre::Vector3 position, float heading, Ogre::SceneNode* parent){

	Entity381 *ent = NULL;

	if (parent == NULL) parent = engine->gfxMgr->ogreSceneManager->getRootSceneNode();

	switch(entityType){
	case EntityType::DRAKE:
		ent = new Dragon(position, heading);
		break;
	case EntityType::BUILDING:
		ent = new Building(position, heading);
		break;
	case EntityType::MISSILE:
		ent = new Missile(position, heading);
		break;
	default:
		break;
	}

	CreateOgreEntityAndNode(ent, parent);
	entities.push_front(ent);
	return ent;
}

void EntityMgr::SelectNextEntity()
{
	int n = 0;

	for (std::list<Entity381 *>::const_iterator it = entities.begin(); it != entities.end(); ++it)
	{
		n++;
		if((*it)->isSelected)
		{
			(*it)->isSelected = false;
			it++;
			if(it == entities.end())
			{
			  std::cout << "End of ents" << std::endl;
			  selectedEntity = *(entities.begin());
			} 
			
			else 
			{
			  selectedEntity = *it;
			}
			
			selectedEntity->isSelected = true;
			break;
		}
	}

}


Entity381* EntityMgr::GetNearest(Ogre::Vector3 point){
	// Thresholds
	Ogre::Real radius = 150;

	Ogre::Real dist = Ogre::Math::POS_INFINITY;
	Entity381* closestEnt = NULL;

	for (std::list<Entity381 *>::const_iterator it = entities.begin(); it != entities.end(); ++it){
		if ((*it)->pos.distance(point) < dist){
			dist = (*it)->pos.distance(point);
			if (dist <= radius){
				closestEnt = *it;
			}
		}
	}

   return closestEnt;
}
