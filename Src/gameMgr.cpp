/*
 * gameMgr.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <OgreMeshManager.h>

#include "../Inc/gameMgr.h"
#include "../Inc/engine.h"

using namespace Ogre;
using std::fstream;
using std::getline;
using std::string;

GameMgr::GameMgr(Engine *engine): Mgr(engine)
{
  headCounter = 0;
  xSpawn = 1000;
  zSpawn = 1000;
}

GameMgr::~GameMgr(){

}

void GameMgr::init()
{
  fireballActive = false;
  fireballPos = Ogre::Vector3::ZERO;
}


void GameMgr::loadLevel()
{
	createEnts();
	createSky();
	createPlane();
	createCity("map0.txt");
}

void GameMgr::stop(){

}

void GameMgr::tick(float dt)
{
  if(headCounter < 1000)
  {
    createMissile();

    headCounter++;

    xSpawn += 100;

    zSpawn += 100;
  }

  if(fireballActive)
  {
    shootFireball(fireballNode, dt);
  }
}

void GameMgr::createEnts()
{
	Entity381 *ent, *parent;

//	engine->entityMgr->CreateEntity(EntityType::BUILDING, Ogre::Vector3(0, 5000, 0), 0);
//	parent = engine->entityMgr->CreateEntity(EntityType::BLANK, Ogre::Vector3(0, 5000, 0), 0);
	parent->ogreSceneNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode();
	parent->ogreSceneNode->setPosition(0, 5000, 0);
	ent = engine->entityMgr->CreateEntity(EntityType::DRAKE, Ogre::Vector3(-120, 0, -210), 0, parent->ogreSceneNode);
	std::cout << "Created: " << ent->meshfile << std::endl;
	parent->isSelected = true;
	engine->entityMgr->selectedEntity = parent;
}

void GameMgr::createPlane()
{
	plane = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
	
	Ogre::MeshManager::getSingleton().createPlane(
	    "ground",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    plane,
	    100000, 100000, 20, 20,
	    true,
	    1, 5, 5,
	    Ogre::Vector3::UNIT_Z);

	  Ogre::Entity* groundEntity = engine->gfxMgr->ogreSceneManager->createEntity("ground");
	  engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

	  groundEntity->setCastShadows(false);
	  groundEntity->setMaterialName("Ocean2_HLSL_GLSL");
	  //groundEntity->setMaterialName("OceanHLSL_GLSL");
	  //groundEntity->setMaterialName("Ocean2_Cg");
	  //groundEntity->setMaterialName("NavyCg");
}


void GameMgr::createSky()
{
  engine->gfxMgr->ogreSceneManager->setSkyBox(true, "Examples/SpaceSkyBox");
}

void GameMgr::createCity(string filename)
{
  auto heightMap = getHeightMatrix(filename);
  genCity(heightMap);
}

std::vector<std::vector<int> > GameMgr::getHeightMatrix(const string& filename)
{
  std::vector<std::vector<int> > heightMatrix;
  fstream fin(filename);
  string heightRow;
  int height;

  for (int row = 0; getline(fin, heightRow); ++row)
  {
    std::stringstream heightStream;
    heightStream.str(heightRow);
    heightMatrix.push_back(std::vector<int>());

    while (heightStream >> height)
    {
      heightMatrix[row].push_back(height);
    }
  }

  return heightMatrix;
}

void GameMgr::genCity(const std::vector<std::vector<int> >& map)
{
  Vector3 startPos = Ogre::Vector3(-7500.0f, -2500.0f, -7500.0f);
  Vector3 buildScale = Ogre::Vector3(250.0f, 250.0f, 250.0f);

  Vector3 spawnPos = startPos;
  Ogre::Real space = 2.0f;
  Ogre::Real columnSpace = space * buildScale.x;
  Ogre::Real rowSpace = space * buildScale.z;
  Ogre::Real heightMultiplier = 200.0f;
  int height;
  Entity381* ent = NULL;

  srand(time(NULL));

  for (size_t row = 0; row < map.size(); ++row)
  {
    for (size_t col = 0; col < map[row].size(); ++col)
    {
      spawnPos.x = startPos.x + (row * rowSpace);
      spawnPos.z = startPos.z + (col * columnSpace);

      height = (map[row][col] != 0) ? rand() % 9 + 1 : 0;
      spawnPos.y = startPos.y + height * heightMultiplier;

      if (map[row][col] != 0)
      {
        ent = engine->entityMgr->CreateEntity(EntityType::BUILDING, spawnPos);
        ent->ogreSceneNode->setScale(buildScale);
      }
      else
      {
        spawnPos.y = 200.0f;
        ent = engine->entityMgr->CreateEntity(EntityType::BUILDING, spawnPos);
        ent->ogreSceneNode->setScale(250.0f, 100.0f, 250.0f);
        ent->ogreSceneNode->roll(Degree(90));
        ent->ogreEntity->setMaterialName("Examples/Rockwall");
      }

    }
  }
}

void GameMgr::createFireball()
{

	 ParticleSystem* fireballParticle1 = engine->gfxMgr->ogreSceneManager->createParticleSystem("Fireball1", "Fire/Fireball");
	 fireballNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("fireballParticle1");
	 fireballNode->attachObject(fireballParticle1);
	 fireballNode->setPosition(engine->entityMgr->selectedEntity->ogreSceneNode->getPosition());
	 fireballNode->translate(Ogre::Vector3(0,0,-100), Ogre::Node::TS_LOCAL);

}

void GameMgr::shootFireball(Ogre::SceneNode* fireballNode, float dt)
{
 	float target;

    Ogre::Vector3 diff = engine->inputMgr->clickPoint - fireballNode->getPosition();

    //target = atan2(diff.z, diff.x)

	fireballNode->translate(diff * dt, Ogre::Node::TS_LOCAL);

	fireballPos = fireballNode->getPosition();

	if(fireballPos == Ogre::Vector3(0, 4500,-100))
	{
		std::cout << "HERE'S JOHNNY" << std::endl;

	    fireballActive = false;

/*	    // create a particle system named explosions using the explosionTemplate
		ParticleSystem* EXUPROSION = engine->gfxMgr->ogreSceneManager->createParticleSystem("explosions", "explosionTemplate");

		// fast forward 1 second  to the point where the particle has been emitted
		EXUPROSION->fastForward(1.0);

		// attach the particle system to a scene node
		Ogre::SceneNode* explodeNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("EXUPROSION");
		explodeNode->attachObject(EXUPROSION); */
	}
}

void GameMgr::createFire(Ogre::Vector3 position)
{
	 ParticleSystem* fireParticle = engine->gfxMgr->ogreSceneManager->createParticleSystem("Fire1", "Fire/Fire");
	 SceneNode* fireNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("fireParticle");
	 fireNode->attachObject(fireParticle);
	 fireNode->setPosition(0, 200, 0);

}

void GameMgr::createMissile()
{
	Entity381* ent;

	ent = engine->entityMgr->CreateEntity(EntityType::MISSILE, Ogre::Vector3(xSpawn, 4500, zSpawn), 0, 0);
	ent->leader = engine->entityMgr->selectedEntity;
}


