/*
 * GfxMgr.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: sushil
 */


#include "../Inc/GfxMgr.h"
#include "../Inc/engine.h"

#include <unistd.h>

GfxMgr::GfxMgr(Engine *eng): Mgr(eng)
{

	resources = "resources.cfg";
	plugins   = "plugins.cfg";

	ogreRoot = new Ogre::Root(plugins);

	cf.load(resources);
	loadConfig(cf);
	configureRenderSystem();
	ogreRenderWindow = ogreRoot->initialise(true, "Drake Overtake");

	createSceneManager();
	createCamera();
	createViewport();
}

void GfxMgr::loadConfig(Ogre::ConfigFile cf){
	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
	while(secIt.hasMoreElements()){
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;
		for(it = settings->begin(); it != settings->end(); ++it){
			locType = it->first;
			name = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}
}

void GfxMgr::configureRenderSystem(){
	if(!(ogreRoot->restoreConfig() || ogreRoot->showConfigDialog()))
		engine->stop();
}

void GfxMgr::initResources(){
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void GfxMgr::createSceneManager(){
	ogreSceneManager = ogreRoot->createSceneManager(Ogre::ST_GENERIC);
}

void GfxMgr::createCamera(){
	ogreCamera = ogreSceneManager->createCamera("MainCam");
	ogreCamera->setPosition(0, 0, 0);
	ogreCamera->setNearClipDistance(5);
	cameraNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode();
	pitchNode = cameraNode->createChildSceneNode();
	pitchNode->attachObject(ogreCamera);

	cameraNode->setPosition(0, 6000, 0);
	ogreCamera->lookAt(0, 500, -200);
}

void GfxMgr::createViewport(){
	ogreViewport = ogreRenderWindow->addViewport(ogreCamera);
	ogreViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	ogreCamera->setAspectRatio(Ogre::Real(ogreViewport->getActualWidth()) /
		    Ogre::Real(ogreViewport->getActualHeight()));
}


GfxMgr::~GfxMgr() {
	delete ogreRoot; //after inputMgr destructor
}

void GfxMgr::init()
{
	bool infiniteClip =
	    engine->gfxMgr->ogreRoot->getRenderSystem()->getCapabilities()->hasCapability(
	    Ogre::RSC_INFINITE_FAR_PLANE);

	std::cout << "Initialize gfx" << std::endl;
	initResources();

	if (infiniteClip)
	  engine->gfxMgr->ogreCamera->setFarClipDistance(0);
	else
	  engine->gfxMgr->ogreCamera->setFarClipDistance(50000);

	engine->gfxMgr->ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	ogreLight = engine->gfxMgr->ogreSceneManager->createLight("MainLight");
	ogreLight->setDiffuseColour(Ogre::ColourValue::Red);
	ogreLight->setSpecularColour(Ogre::ColourValue(.1, .1, .1));
	ogreLight->setPosition(20.0, 80.0, 50.0);
}

void GfxMgr::loadLevel(){

}


void GfxMgr::tick(float dt){
	Ogre::WindowEventUtilities::messagePump();
	if(ogreRenderWindow->isClosed()) engine->stop();
	if(!ogreRoot->renderOneFrame()) engine->stop();
	return;
}

void GfxMgr::stop(){
	std::cout << "stopping engine and ogre" << std::endl;
	ogreRoot->shutdown();
	return;
}
