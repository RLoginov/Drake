/*
 * inputMgr.h
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#ifndef INPUTMGR_H_
#define INPUTMGR_H_

#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "../Inc/mgr.h"
#include "../Inc/EntityMgr.h"
#include "../Inc/Entity381.h"



class InputMgr : public Mgr, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener
{
private:
	void UpdatePosition(float dt);
	void UpdateSelection(float dt);
	float keyboardTimer;
	float selectionTimer;
	const float keyTime = 0.2f;
	const float selectionTime = 0.2f;

	void UpdateCamera(float dt);

protected:
	virtual void windowResized(Ogre::RenderWindow *rw);
	virtual void windowClosed(Ogre::RenderWindow *rw);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

public:

	InputMgr(Engine *engine);
	~InputMgr();
	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();

	//OIS Input devices
	OIS::InputManager* oisInputManager;
    OIS::Mouse*        mouse;
    OIS::Keyboard*     keyboard;
    Ogre::Vector3      clickPoint;


    bool lShiftDown;

    void UpdateDesiredSpeedHeading(float dt);
};



#endif /* INPUTMGR_H_ */
