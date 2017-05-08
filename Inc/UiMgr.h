/*
 * UiMgr.h
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */

#ifndef INC_UIMGR_H_
#define INC_UIMGR_H_

#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <mgr.h>
#include <string>
#include <cstring>
#include <iostream>

#include <SdkTrays.h>

using namespace std;

class UiMgr : public Mgr, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener, public OgreBites::SdkTrayListener
{
private:

protected:
	virtual void windowResized(Ogre::RenderWindow *rw);
	virtual void windowClosed(Ogre::RenderWindow *rw);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    void buttonHit(OgreBites::Button *b);
    void itemSelected(OgreBites::SelectMenu *m);

public:

	UiMgr(Engine *engine);
	~UiMgr();
	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();

	OgreBites::InputContext mInputContext;
	OgreBites::SdkTrayManager* mTrayMgr;
	Ogre::OverlaySystem* mOverlaySystem;

	OgreBites::SelectMenu* testMenu;
	OgreBites::Button* testButton;
	float splashScreenTimer;
	int lives;
	Ogre::String livesText;
	Ogre::String livesRemainingString;
	OgreBites::Label* testLabel;
	OgreBites::Label* fireballLabel;
	float instructionScreenTimer;

	Ogre::String timeText;
	Ogre::String timeString;
	int tensSecondsRemaining;
	int onesSecondsRemaining;
	int minutesRemaining;
	float downTimer;
	OgreBites::Label* timeLabel;

	void timeRemainingToString();
	void livesRemainingToString();
	void fireballsToString();

	bool gameStart;

	Ogre::String fireballText;
	Ogre::String fireballString;
	int fireballTimer;
	int fireballsReady;

	Ogre::OverlayContainer* plus;
	Ogre::OverlayElement* plusImage;
};

#endif /* INC_UIMGR_H_ */
