/*
 * inputMgr.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil/Tyler Goffinet/Robert Loginov
 */

#include "../Inc/inputMgr.h"
#include "../Inc/engine.h"

using namespace std;

InputMgr::InputMgr(Engine *engine) : Mgr(engine){
	keyboardTimer = keyTime;
	selectionTimer = selectionTime;

	keyboard = 0;
	mouse = 0;
	oisInputManager = 0;

	OIS::ParamList pl;
	size_t windowHandle = 0;
	std::ostringstream windowHandleStr;
	engine->gfxMgr->ogreRenderWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleStr << windowHandle;

	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

	// insert the following lines right before calling mInputSystem = OIS::InputManager::createInputSystem( paramList );
#if defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("true")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

	oisInputManager = OIS::InputManager::createInputSystem(pl);
	keyboard = static_cast<OIS::Keyboard*>(oisInputManager->createInputObject(OIS::OISKeyboard, true));
	mouse    = static_cast<OIS::Mouse*>(oisInputManager->createInputObject(OIS::OISMouse, true));

	keyboard->setEventCallback(this);
	mouse->setEventCallback(this);

	windowResized(engine->gfxMgr->ogreRenderWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);


	lShiftDown = false;

}

InputMgr::~InputMgr(){ // before gfxMgr destructor

}

void InputMgr::init(){

}

void InputMgr::loadLevel(){

}

void InputMgr::tick(float dt){
	keyboard->capture();
	mouse->capture();
	if(keyboard->isKeyDown(OIS::KC_ESCAPE))
		engine->stop();

	UpdatePosition(dt);
	UpdateCamera(dt);




	//UpdateDesiredSpeedHeading(dt);
}

void InputMgr::stop(){
	if (oisInputManager){
		oisInputManager->destroyInputObject(mouse);
		oisInputManager->destroyInputObject(keyboard);
		OIS::InputManager::destroyInputSystem(oisInputManager);
		oisInputManager = 0;
	}
}

void InputMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void InputMgr::windowClosed(Ogre::RenderWindow* rw){

	if(rw == engine->gfxMgr->ogreRenderWindow){
		engine->stop();
	}
}

bool InputMgr::keyPressed(const OIS::KeyEvent &arg)
{
	float fly = 500.0f;
	float rush = 500.0f;
	float rotate = 0.1f;
	Ogre::Vector3 temp;
	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	switch (arg.key)
	{
	  case OIS::KC_LSHIFT:
		lShiftDown = true;
		break;
	}
	return true;
}

bool InputMgr::keyReleased(const OIS::KeyEvent &arg){

	switch (arg.key)
	{
	case OIS::KC_LSHIFT:
		lShiftDown = false;
		break;
	case OIS::KC_TAB:
		engine->entityMgr->SelectNextEntity();
		break;
	}

	return true;
}

bool InputMgr::mouseMoved(const OIS::MouseEvent &arg)
{
	if (engine->uiMgr->mTrayMgr->injectMouseMove(arg)) return true;
		return true;
}

bool InputMgr::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    std::cout << "mouse clicked" << std::endl;
    Ogre::Ray mouseRay;
    Ogre::Real screenWidth;
    Ogre::Real screenHeight;
    Ogre::Real offsetX;
    Ogre::Real offsetY;
    std::pair<bool, float> result;
    Entity381* nearest;
    if(engine->uiMgr->fireballsReady > 0)
       engine->uiMgr->fireballsReady--;

  	if (engine->uiMgr->mTrayMgr->injectMouseDown(arg, id)) return true;


    screenWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    screenHeight = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();

    offsetX = arg.state.X.abs / screenWidth;
    offsetY = arg.state.Y.abs / screenHeight;

    mouseRay = engine->gfxMgr->ogreCamera->getCameraToViewportRay(offsetX, offsetY);
    result = mouseRay.intersects(engine->gameMgr->plane);

    if (result.first)
    {
  	  clickPoint = mouseRay.getPoint(result.second);

	  engine->gameMgr->createFireball();

	  engine->gameMgr->fireballActive = true;
    }

  	return true;
}

bool InputMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (engine->uiMgr->mTrayMgr->injectMouseUp(arg, id)) return true;
	  	return true;
}


// Game specific input handling
void InputMgr::UpdatePosition(float dt)
{
	float fly = 1000.0f;
	float rush = 2000.0f;
	float turn = 0.5f;
	float quickturn = 0.7f;

	Ogre::Vector3 temp;


	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	  if (keyboard->isKeyDown(OIS::KC_W))
	  {
		  if (lShiftDown)
			  dirVec.z -= rush;
		  else
			  dirVec.z -= fly;
	  }


/*	  if (keyboard->isKeyDown(OIS::KC_R))
	  {
		  if (lShiftDown)
		  {
			  dirVec.y += rush;
		  }
		  else
		  {
			  dirVec.y += fly;
		  }

	  }

	  if (keyboard->isKeyDown(OIS::KC_F))
	  {
		  temp = engine->gfxMgr->cameraNode->getPosition();

		  if (temp.y <= 4000.0f)
		  {
		    return;
		  }

		  if (keyboard->isKeyDown(OIS::KC_LSHIFT))
		  {
			  dirVec.y -= rush;
		  }

		  else
		  {
			  dirVec.y -= fly;
		  }
	  } */

	  if (keyboard->isKeyDown(OIS::KC_Q))
	  {
		  if (lShiftDown)
		  {
			  engine->entityMgr->selectedEntity->ogreSceneNode->yaw(Ogre::Degree(-quickturn));
			  engine->gfxMgr->cameraNode->yaw(Ogre::Degree(-quickturn));
		  }

		  else
		  {
	          //engine->entityMgr->selectedEntity->ogreSceneNode->yaw(Ogre::Degree(-turn));
			  engine->gfxMgr->cameraNode->yaw(Ogre::Degree(-turn));
			  engine->entityMgr->selectedEntity->ogreSceneNode->yaw(Ogre::Degree(-turn));
		  }

	  }

	  if (keyboard->isKeyDown(OIS::KC_E))
	  {
		  if (lShiftDown)
		  {
			  engine->gfxMgr->cameraNode->yaw(Ogre::Degree(quickturn));
			  engine->entityMgr->selectedEntity->ogreSceneNode->yaw(Ogre::Degree(quickturn));
		  }

		  else
		  {
			  engine->gfxMgr->cameraNode->yaw(Ogre::Degree(turn));
			  engine->entityMgr->selectedEntity->ogreSceneNode->yaw(Ogre::Degree(turn));
		  }

	  }

/*	  if (keyboard->isKeyDown(OIS::KC_Z))
	  {
		  if (keyboard->isKeyDown(OIS::KC_LSHIFT))
			  engine->gfxMgr->cameraNode->pitch(Ogre::Degree(quickturn));
		  else
			  engine->gfxMgr->cameraNode->pitch(Ogre::Degree(turn));
	  }

	  if (keyboard->isKeyDown(OIS::KC_X))
	  {
		  if (keyboard->isKeyDown(OIS::KC_LSHIFT))
			  engine->gfxMgr->cameraNode->pitch(Ogre::Degree(-quickturn));
		  else
			  engine->gfxMgr->cameraNode->pitch(Ogre::Degree(-turn));
	  } */

    engine->entityMgr->selectedEntity->ogreSceneNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
	  engine->gfxMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
}

void InputMgr::UpdateSelection(float dt){
	selectionTimer -= dt;
	if(selectionTimer < 0 && keyboard->isKeyDown(OIS::KC_TAB)){
		selectionTimer = this->selectionTime;
		engine->entityMgr->SelectNextEntity();
	}
}

// Game specific input handling
void InputMgr::UpdateCamera(float dt){
	float move = 100.0f;
	float rotate = 0.1f;

	 Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	  if (keyboard->isKeyDown(OIS::KC_NUMPAD8))
	    dirVec.z -= move;

	  if (keyboard->isKeyDown(OIS::KC_NUMPAD2))
	    dirVec.z += move;

	  if (keyboard->isKeyDown(OIS::KC_NUMPAD9))
	    dirVec.y += move;

	  if (keyboard->isKeyDown(OIS::KC_NUMPAD3))
	    dirVec.y -= move;

	  if (keyboard->isKeyDown(OIS::KC_NUMPAD4))
	  {
	    if (keyboard->isKeyDown(OIS::KC_LSHIFT))
		      engine->gfxMgr->cameraNode->yaw(Ogre::Degree(5 * rotate));
	    else
	      dirVec.x -= move;
	  }

	  if (keyboard->isKeyDown(OIS::KC_NUMPAD6))
	  {
	    if (keyboard->isKeyDown(OIS::KC_LSHIFT))
	      engine->gfxMgr->cameraNode->yaw(Ogre::Degree(-5 * rotate));
	    else
	      dirVec.x += move;
	  }

	  engine->gfxMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
}




void InputMgr::UpdateDesiredSpeedHeading(float dt){
	keyboardTimer -= dt;

	//cout << endl << engine->entityMgr->selectedEntity->desiredSpeed << endl << endl;

	if(engine->entityMgr->selectedEntity){

		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_W)){
			keyboardTimer = keyTime;
			//cout << engine->entityMgr->selectedEntity->desiredSpeed << endl;
			//cout << engine->entityMgr->selectedEntity->speed << endl << endl;
			engine->entityMgr->selectedEntity->desiredSpeed += 10;
			//engine->entityMgr->selectedEntity->ogreSceneNode->getParent()->desiredSpeed += 10;
		}
		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_S)){
			keyboardTimer = keyTime;
			engine->entityMgr->selectedEntity->desiredSpeed -= 10;
		}
		engine->entityMgr->selectedEntity->desiredSpeed =
				std::max(engine->entityMgr->selectedEntity->minSpeed,
						std::min(engine->entityMgr->selectedEntity->maxSpeed,
								engine->entityMgr->selectedEntity->desiredSpeed));


		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_A)){
			keyboardTimer = keyTime;
			engine->entityMgr->selectedEntity->desiredHeading -= 0.3f;
		}
		if((keyboardTimer < 0) && keyboard->isKeyDown(OIS::KC_D)){
			keyboardTimer = keyTime;
			engine->entityMgr->selectedEntity->desiredHeading += 0.3f;
		}
		//entityMgr->selectedEntity->desiredHeading = FixAngle(entityMgr->selectedEntity->desiredHeading);
	}
}

