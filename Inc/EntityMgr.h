/*
 * EntityMgr.h
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#ifndef ENTITYMGR_H_
#define ENTITYMGR_H_


#include <list>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "../Inc/Entity381.h"
#include "../Inc/mgr.h"

class Engine;
class Entity381;

class EntityMgr: public Mgr {

public:

	std::list<Entity381 *> entities;
	Entity381* selectedEntity;

	EntityMgr(Engine *eng);
	~EntityMgr();

	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();

	Entity381 *CreateEntity(EntityType entityType, Ogre::Vector3 pos, float yaw = 0, Ogre::SceneNode* parent = NULL);
	void CreateOgreEntityAndNode(Entity381 *ent, Ogre::SceneNode *parent);
	//void Tick(float dt);
	
	void SelectNextEntity();

	Entity381* GetNearest(Ogre::Vector3 point);
};



#endif /* ENTITYMGR_H_ */
