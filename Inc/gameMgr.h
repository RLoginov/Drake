/*
 * gameMgr.h
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <vector>
#include <string>

#include <OgrePlane.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreRoot.h>

#include "../Inc/mgr.h"
#include "../Inc/Entity381.h"

class GameMgr : public Mgr {

private:
	void createEnts();
	void createSky();
	void createPlane();

	void createCity(std::string filename);
	std::vector<std::vector<int> > getHeightMatrix(const std::string& filename);
    void genCity(const std::vector<std::vector<int> >& map);

    void createFire(Ogre::Vector3 position);
    void createMissile();

public:
	GameMgr(Engine *engine);
	~GameMgr();
	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();
	
    void createFireball();
    void shootFireball(Ogre::SceneNode* fireballNode, float dt);


	//---------------------------------

	Ogre::Plane plane;
	int headCounter, fireballCounter;
	int xSpawn, zSpawn;
	Ogre::SceneNode* fireballNode;
	bool fireballActive;
	Ogre::Vector3 fireballPos;
	Ogre::Vector3 fireballTarget;
};


#endif /* GAMEMGR_H_ */
