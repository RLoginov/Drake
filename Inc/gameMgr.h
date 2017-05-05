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

    void createParticles();
    void createMissile();


public:
	GameMgr(Engine *engine);
	~GameMgr();
	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();
	

	//---------------------------------

	Ogre::Plane plane;
	Entity381* cube;
};


#endif /* GAMEMGR_H_ */
