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

GameMgr::GameMgr(Engine *engine): Mgr(engine){

}

GameMgr::~GameMgr(){

}

void GameMgr::init()
{
}


void GameMgr::loadLevel()
{
	createEnts();
	createSky();
	createPlane();
	createCity("map0.txt");
	createParticles();
	createMissile();
}

void GameMgr::stop(){

}

void GameMgr::tick(float dt)
{

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
  Vector3 startPos = Ogre::Vector3(-7500.0f, 200.0f, -7500.0f);
  Vector3 buildScale = Ogre::Vector3(5.0f, 5.0f, 5.0f);

  Vector3 spawnPos = startPos;
  Ogre::Real space = 200.0f;
  Ogre::Real columnSpace = space * buildScale.x;
  Ogre::Real rowSpace = space * buildScale.z;
  Ogre::Real heightMultiplier = 100.0f;
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

      ent = engine->entityMgr->CreateEntity(EntityType::BUILDING, spawnPos);
      ent->ogreSceneNode->scale(buildScale);
    }
  }
}

void GameMgr::createParticles()
{
	 ParticleSystem* fireballParticle1 = engine->gfxMgr->ogreSceneManager->createParticleSystem("Fireball1", "Fire/Fireball");
	 SceneNode* fireballNode1 = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("fireballParticle1");
	 fireballNode1->attachObject(fireballParticle1);
	 fireballNode1->setPosition(0, 4000, 0);

/*	 ParticleSystem* fireballParticle2 = engine->gfxMgr->ogreSceneManager->createParticleSystem("Fireball2", "Fire/Fireball");
	 SceneNode* fireballNode2 = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("fireballParticle2");
	 fireballNode2->attachObject(fireballParticle2);
	 fireballNode2->setPosition(200, 4000, 0); */

	 ParticleSystem* fireParticle = engine->gfxMgr->ogreSceneManager->createParticleSystem("Fire", "Fire/Fire");
	 SceneNode* fireNode = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("fireParticle");
	 fireNode->attachObject(fireParticle);
	 fireNode->setPosition(0, 200, 0);

}

void GameMgr::createMissile()
{
	Entity381* ent;

	ent = engine->entityMgr->CreateEntity(EntityType::MISSILE, Ogre::Vector3(-500, 4500, 500), 0, 0);
	ent->leader = engine->entityMgr->selectedEntity;
}



