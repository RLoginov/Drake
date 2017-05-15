/*
 * gameMgr.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#include <fstream>
#include <iostream>
#include <sstream>


#include "../Inc/gameMgr.h"
#include "../Inc/engine.h"

using namespace Ogre;
using std::fstream;
using std::getline;
using std::string;

GameMgr::GameMgr(Engine *engine): Mgr(engine)
{
  headCounter = 0;
  xSpawn = -435;
  zSpawn = -4907;
}

GameMgr::~GameMgr(){

}

void GameMgr::init()
{
  fireballs.resize(engine->uiMgr->fireballsReady + 10);
  fireballNodes.resize(engine->uiMgr->fireballsReady + 10);
  fires.resize(engine->uiMgr->fireballsReady + 10);
  fireNodes.resize(engine->uiMgr->fireballsReady + 10);
  smoke.resize(engine->uiMgr->fireballsReady + 10);
  smokeNodes.resize(engine->uiMgr->fireballsReady + 10);

  fireballActives.resize(engine->uiMgr->fireballsReady + 10);
  firePositions.resize(engine->uiMgr->fireballsReady + 10);
}


void GameMgr::loadLevel()
{
	createEnts();
	createSky();
	createPlane();
	createCity("map0.txt");
	createFireballs();
	createFires();
	createSmoke();
}

void GameMgr::stop(){

}


void GameMgr::tick(float dt)
{
  if(headCounter < 500)
  {
    createMissile();

    headCounter++;

    xSpawn += 100;

    zSpawn += 100;
  }

  for (int i = 0; i < engine->uiMgr->fireballsReady + 8; i++)
  {
     if(this->fireballActives[i])
     {
       shootFireball(dt, i);
     }
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

void GameMgr::createFireballs()
{
	for (int i = 0; i < engine->uiMgr->fireballsReady; i++)
	 {
	 	Ogre::String number = Ogre::StringConverter::toString(i + 1);

	 	fireballs[i] = engine->gfxMgr->ogreSceneManager->createParticleSystem("Fireball " + number, "Fire/Fireball");
	 	fireballNodes[i] = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("Fireball Node " + number);

	 	fireballNodes[i]->attachObject(fireballs[i]);

	 	fireballActives[i] = false;
	 }

}

void GameMgr::createFires()
{
	for (int i = 0; i < engine->uiMgr->fireballsReady; i++)
	 {
	 	Ogre::String number = Ogre::StringConverter::toString(i + 1);

	 	fires[i] = engine->gfxMgr->ogreSceneManager->createParticleSystem("Fire " + number, "Fire/Fire");;
	 	fireNodes[i] = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("Fire Node " + number);

	 	fireNodes[i]->attachObject(fires[i]);
	 }

}

void GameMgr::createSmoke()
{
	for (int i = 0; i < engine->uiMgr->fireballsReady; i++)
	 {
	 	Ogre::String number = Ogre::StringConverter::toString(i + 1);

	 	smoke[i] = engine->gfxMgr->ogreSceneManager->createParticleSystem("Smoke " + number, "Fire/Smoke");;
	 	smokeNodes[i] = engine->gfxMgr->ogreSceneManager->getRootSceneNode()->createChildSceneNode("Smoke Node " + number);

	 	smokeNodes[i]->attachObject(smoke[i]);
	 }

}

void GameMgr::shootFireball(float dt, int fireballNum)
{
   this->firePositions[fireballNum] = engine->inputMgr->clickPoint - fireballNodes[fireballNum]->getPosition();

   fireballNodes[fireballNum]->translate(this->firePositions[fireballNum] * dt, Ogre::Node::TS_LOCAL);

   if( fireballNodes[fireballNum]->getPosition().y <= 500.0f )
   {
	  if( fireballNodes[fireballNum]->getPosition().z >= -7700.0f &&
		  fireballNodes[fireballNum]->getPosition().z <= 750.0f &&
		  fireballNodes[fireballNum]->getPosition().x >= -8600.0f &&
		  fireballNodes[fireballNum]->getPosition().x <= 27000.0f )
	  {
        spawnFire(fireballNum, Ogre::Vector3(engine->inputMgr->clickPoint.x,500,engine->inputMgr->clickPoint.z));

        spawnSmoke(fireballNum, Ogre::Vector3(engine->inputMgr->clickPoint.x,1500,engine->inputMgr->clickPoint.z));

        engine->uiMgr->score += 100;
	  }

      this->fireballActives[fireballNum] = false;

      fireballs[fireballNum]->setVisible(false);
   }

}

void GameMgr::spawnFire(int fireballNum, Ogre::Vector3 position)
{
   if(this->fireballActives[fireballNum])
   {
      fireNodes[fireballNum]->setPosition(position);
   }
}

void GameMgr::spawnSmoke(int fireballNum, Ogre::Vector3 position)
{
   if(this->fireballActives[fireballNum])
   {
      smokeNodes[fireballNum]->setPosition(position);
   }
}


void GameMgr::createMissile()
{
	Entity381* ent;

	ent = engine->entityMgr->CreateEntity(EntityType::MISSILE, Ogre::Vector3(xSpawn, 4500, zSpawn), 0, 0);
	ent->leader = engine->entityMgr->selectedEntity;
}


