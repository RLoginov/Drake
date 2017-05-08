/*
 * UiMgr.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */

#include <UiMgr.h>
#include <engine.h>

using namespace std;

UiMgr::UiMgr(Engine* eng): Mgr(eng){
	// initialize class variables
	mTrayMgr = 0;
	testMenu = 0;
	testButton = 0;
	testLabel = 0;
	downTimer = 1;
	timeLabel = 0;
	splashScreenTimer = 5;
	instructionScreenTimer = 5;
	tensSecondsRemaining = 0;
	onesSecondsRemaining = 0;
	minutesRemaining = 1;
	lives = 3;
	gameStart = false;
	plus = 0;
	fireballLabel = 0;
	fireballTimer = 2;
	fireballsReady = 21;

	// Initialize the OverlaySystem (changed for Ogre 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();
	engine->gfxMgr->ogreSceneManager->addRenderQueueListener(mOverlaySystem);

	//Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);

	// display default lives and time remaining
	livesRemainingToString();
	timeRemainingToString();
	fireballsToString();
}

UiMgr::~UiMgr(){ // before gfxMgr destructor
	timeLabel->cleanup();
}

void UiMgr::init(){
	//init sdktrays
    mInputContext.mKeyboard = engine->inputMgr->keyboard;
    mInputContext.mMouse = engine->inputMgr->mouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", engine->gfxMgr->ogreRenderWindow, mInputContext, this);
}

void UiMgr::stop(){

}

void UiMgr::loadLevel(){
	// load in widgets (buttons, menus, labels, etc.)
	testButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton", "Start Game");
	mTrayMgr->showBackdrop("splash-screen");

	mTrayMgr->showCursor("plus");
	mTrayMgr->getCursorLayer();
	plus = mTrayMgr->getCursorContainer();
	plusImage = mTrayMgr->getCursorImage();
	plusImage->setPosition(-10, -10);
}

void UiMgr::tick(float dt){
	mTrayMgr->refreshCursor();
	splashScreenTimer -= dt;
	instructionScreenTimer -= dt;
	downTimer -= dt;

	// display time and lives remaining
	if(gameStart)
	{
		// one second has passed
		if(downTimer < 0)
		{
			// check if going to change minutes and tens place seconds
			if(onesSecondsRemaining == 0)
			{
				// reset ones place seconds
				onesSecondsRemaining = 9;

				// check if going to a new minute
				if(tensSecondsRemaining == 0)
				{
					tensSecondsRemaining = 5;
					minutesRemaining--;
				}

				// count down tens place seconds
				else
				   tensSecondsRemaining--;
			}

			// count down ones place seconds
			else
			   onesSecondsRemaining--;

			// reset second counter
			downTimer = 1;


			if(fireballsReady < 20)
			{
				fireballTimer--;

				if(fireballTimer == 0)
				{
					fireballTimer = 2;
					//fireballsReady++;
				}
			}

			// ran out of time
			if((minutesRemaining == 0 && tensSecondsRemaining == 0 && onesSecondsRemaining == 0) || lives <= 0)
			{
				gameStart = false;
				mTrayMgr->destroyWidget("lives");
				mTrayMgr->destroyWidget("time");
				mTrayMgr->destroyWidget("fireball");
				mTrayMgr->showBackdrop("credits");
			}


			for (std::list<Entity381 *>::const_iterator it = engine->entityMgr->entities.begin(); it !=
				 engine->entityMgr->entities.end(); ++it)
			{
              if((*it)->meshfile == "ogrehead.mesh")
              {
                if((*it)->hit == true)
                {
                  (*it)->hit = false;
                  lives--;
                }
              }
			}
		}

		if(gameStart != false)
		{
		fireballsToString();
		timeRemainingToString();
		livesRemainingToString();
		timeLabel->setCaption(timeText);
		testLabel->setCaption(livesText);
		fireballLabel->setCaption(fireballText);
		}
	}
}

void UiMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = engine->inputMgr->mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void UiMgr::windowClosed(Ogre::RenderWindow* rw){//

}

bool UiMgr::keyPressed(const OIS::KeyEvent &arg) {
	std::cout << "Key Pressed: " << arg.key << std::endl;
	return true;
}
bool UiMgr::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
bool UiMgr::mouseMoved(const OIS::MouseEvent &arg){
    if (mTrayMgr->injectMouseMove(arg)) return true;
    /* normal mouse processing here... */
	return true;
}
bool UiMgr::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	std::cout << "mouse clicked" << std::endl;
	fireballsReady--;
	if (mTrayMgr->injectMouseDown(arg, id)) return true;
	    /* normal mouse processing here... */
	return true;
}
bool UiMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    /* normal mouse processing here... */
	return true;
}

void UiMgr::buttonHit(OgreBites::Button *b){
	if(b->getName()=="MyButton")
    {
    	// convert lives remaining into string
    	livesRemainingToString();

    	// hide the backdrop
    	mTrayMgr->hideBackdrop();

    	// show game HUD
    	if(!gameStart)
    	{
    		gameStart = true;
    		tensSecondsRemaining = 0;
    		onesSecondsRemaining = 0;
    		minutesRemaining = 2;
    		lives = 3;
    		testLabel = mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "lives", livesText, 200);
    		fireballLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "fireball", fireballText, 200);
    		timeLabel = mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "time", timeText, 200);
    		mTrayMgr->destroyWidget("MyButton");
    	}
    }
}

void UiMgr::itemSelected(OgreBites::SelectMenu *m){
    if(m->getName()=="MyMenu")
    {
    }
}

void UiMgr::timeRemainingToString(){
	// convert time into string
	timeText = "Time Remaining ";
	timeString = to_string(minutesRemaining);
	timeString.append(":");
	timeText.append(timeString);
	timeString = to_string(tensSecondsRemaining);
	timeText.append(timeString);
	timeString = to_string(onesSecondsRemaining);
	timeText.append(timeString);
}

void UiMgr::livesRemainingToString(){
	// convert lives into string
	livesText = "Lives remaining: ";
	livesRemainingString = to_string(lives);
	livesText.append(livesRemainingString);
}

void UiMgr::fireballsToString(){
	// convert fireballs ready to string
	fireballText = "Fireballs ready: ";
	fireballString = to_string(fireballsReady);
	fireballText.append(fireballString);
}









