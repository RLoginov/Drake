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
#include <OgreMeshManager.h>

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

    void createFireballs();
    void createFires();
    void createSmoke();

    void createMissile();

public:
	GameMgr(Engine *engine);
	~GameMgr();
	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();
	
    void shootFireball(float dt, int fireballNum);
    void spawnFire(int firebalNum, Ogre::Vector3 position);
    void spawnSmoke(int fireballNum, Ogre::Vector3 position);


	//---------------------------------

	Ogre::Plane plane;
	int headCounter, fireballCounter;
	int xSpawn, zSpawn;
	std::vector<Ogre::ParticleSystem*> fireballs;
	std::vector<Ogre::SceneNode*> fireballNodes;
	std::vector<Ogre::ParticleSystem*> fires;
	std::vector<Ogre::SceneNode*> fireNodes;
	std::vector<Ogre::ParticleSystem*> smoke;
	std::vector<Ogre::SceneNode*> smokeNodes;

    std::vector<bool> fireballActives;
    std::vector<Ogre::Vector3> firePositions;

};


#endif /* GAMEMGR_H_ */
