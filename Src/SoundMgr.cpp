/*
 * OgreSND::SoundMgr.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: sushil
 */

#include <stdlib.h>

#include "../Inc/SoundMgr.h"
#include "../Inc/engine.h"

OgreSND::SoundMgr::SoundMgr(Engine *eng): Mgr(eng){
	this->engine = eng;

}

OgreSND::SoundMgr::~SoundMgr(){

	std::cout << "Deleting Sound Manager" << std::endl;

	//Destroy sounds
	for(int i = 0; i < OgreSND::maxAudioSources; i++){
		if(alIsSource(this->sourceInfo[i].source)){
			alDeleteSources(1, &this->sourceInfo[i].source);
		}
	}

	//Destroy buffers
	for(int i = 0; i < OgreSND::maxAudioBuffers; i++){
		if(alIsBuffer(this->bufferInfo[i].buffer)){
			alDeleteBuffers(1, &this->bufferInfo[i].buffer);
		}
	}

	int ret;
	ret = alcMakeContextCurrent(NULL);
	if (!ret) {
		std::cerr << "Null current context" << std::endl;
	}
	alcDestroyContext(context);
	//printError("Destroy Context");
		this->device = alcOpenDevice(NULL);
		if(!device){
			std::cerr << "Sound ERROR: Bye, could not open default sound device" << std::endl;
		}
		alGetError();
	//close device
	alcCloseDevice(device);
	//printError("Device close");
	std::cout << "Bye audio. ....   Sounds good, bye" << std::endl;
}

void OgreSND::SoundMgr::init(){
	initialize();
}

void OgreSND::SoundMgr::initialize(void){
	this->device = alcOpenDevice(NULL);
	if(!device){
		std::cerr << "Sound ERROR: Bye, could not open default sound device" << std::endl;
	}
	alGetError();
	this->context = alcCreateContext(this->device, NULL);
	if (!alcMakeContextCurrent(this->context)) {
		std::cerr << "Sound ERROR: Cannot make default context" << std::endl;
	}
	//this->buffersInfo.buffersInUseCount = 0;
	for(int i = 0; i < OgreSND::maxAudioBuffers; i++){
		this->bufferInfo[i].buffer = i+1; // this is because openAl returns true for alIsBuffer(0) no matter what!
		//I'm also using bufferFilename == "" to check if the buffer is in use.
		this->bufferInfo[i].bufferFilename = "";
	}

	//this->sourcesInfo.sourcesInUseCount = 0;
	for(int i = 0; i < OgreSND::maxAudioSources; i++){
		this->sourceInfo[i].source = 0;
		this->sourceInfo[i].inUse = false;
                this->sourceDictionary.push_back("");
	}

	//alGenBuffers(OgreSND::maxAudioBuffers, this->buffersInfo.buffers);
	//printError("Generating buffers");

	//alGenSources(OgreSND::maxAudioSources, this->sourcesInfo.sources);
	//printError("Generating sources");

	//syncListenerToCamera(); //setup listener

        isEnabled = true;

        //initialize vectors
        for (int i = 0; i < 6; i++){
            for (int j = 0; j < soundPerEnt; j++){
                //this->battleSoundsDictionary[i][j] = -1;
                //this->creationSoundsDictionary[i][j] = -1;
                this->selectionSoundsDictionary[i][j] = -1;
            }
        }


	unsigned int sid;
        //background music
	std::string filename = "Sound/Ten_Seconds_to_Rush.wav";
	if (this->reserveAudio(filename, true, sid)){
		std::cout << "background music loaded" << std::endl;
                backgroundMusicSource = sourceInfo[sid].source;
                this->loadStartBackground();
        }
	std::cout << "background music loaded" << std::endl;


        //filename = "data/watercraft/sounds/explosion.wav";
        //default explosion sound for all entities
        if (this->reserveAudio(filename, false, sid)){
            battleSoundSource = sourceInfo[sid].source;
            alSourcei(this->battleSoundSource, AL_REFERENCE_DISTANCE, 2000.0f);
            alSourcei(this->battleSoundSource, AL_MAX_DISTANCE, 8000.0f);
        }

	return;

}


void OgreSND::SoundMgr::enable(){
    isEnabled = true;
    resumeBackground();
}

void OgreSND::SoundMgr::disable(){
    isEnabled = false;
    stopAllAudio();
    stopBackground();
}

void OgreSND::SoundMgr::syncListenerToCamera(){
	//position from camera scene node
	Ogre::Vector3 cameraPosition = engine->gfxMgr->cameraNode->getPosition();
	this->position[0] = cameraPosition.x;
	this->position[1] = cameraPosition.y;
	this->position[2] = cameraPosition.z;
	alListener3f(AL_POSITION, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	//printError("Cannot set listener position");

	//Ogre::Vector3 cameraVelocity = engine->gfxMgr->cameraNode->velocity; // not set by graphics
	this->velocity[0] = 0;
	this->velocity[1] = 0;
	this->velocity[2] = 0;
	alListener3f(AL_VELOCITY, 0, 0, 0); // or we will use
	//alListener3f(AL_VELOCITY, cameraVelocity.x, cameraVelocity.y, cameraVelocity.z);// Don't use both:-)
	//printError("Cannot set listener velocity");

	//need to set orientation from camera scene node
	Ogre::Quaternion q = engine->gfxMgr->cameraNode->getOrientation();
	Ogre::Vector3 vDirection = q.zAxis();
	Ogre::Vector3 vUp = q.yAxis();

	this->orientation[0] = -vDirection.x;
	this->orientation[1] = -vDirection.y;
	this->orientation[2] = -vDirection.z;
	this->orientation[3] = vUp.x;
	this->orientation[4] = vUp.y;
	this->orientation[5] = vUp.z;

	alListenerfv(AL_ORIENTATION, this->orientation);
	//printError("Cannot set listener orientation!");
}


bool OgreSND::SoundMgr::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//tick(evt.timeSinceLastFrame);
	return true;
}
bool OgreSND::SoundMgr::frameStarted(const Ogre::FrameEvent& evt){
	return true;
}
bool OgreSND::SoundMgr::frameEnded(const Ogre::FrameEvent& evt){
	return true;
}

void OgreSND::SoundMgr::crosslink(void){
	syncListenerToCamera();
	return;
}

void OgreSND::SoundMgr::loadLevel(void){
	syncListenerToCamera();
	//load sounds, bind buffers, start background music
	//read sound files

	//load background, start, loop
	//loadStartBackground();

	return;
}
double static tmpT = 0.0;
bool static paused = false;


void OgreSND::SoundMgr::attachSelectedNodeToSoundIndex(Entity381 *ent, unsigned int index){
        if (index == -1) //if there is no defined sound for the specified type, don't do anything
                return;

        this->playAudio(this->sourceInfo[index].source, true);  //second argument is added as true since it was causing nonresponsiveness when method is called again before the sound ends
	Ogre::Vector3 pos = ent->pos;
	setSoundPosition(this->sourceInfo[index].source, pos);
}

void OgreSND::SoundMgr::tick(double dtime){

	syncListenerToCamera();

        //selection sound
		for(std::list<Entity381 *>::const_iterator it = engine->entityMgr->entities.begin(); it != engine->entityMgr->entities.end(); ++it){
           if ((*it)->isSelected && !(*it)->didSelectSoundPlay){
        	   playSelectionSound(*(*it));
        	   (*it)->didSelectSoundPlay = true;
           }
           else if (!(*it)->isSelected && (*it)->didSelectSoundPlay){
        	   (*it)->didSelectSoundPlay = false;
           }
        }
}

        //this was for moving sound but playing sound for all moving objects does not seem to be a good idea
        //copySoundState();


//	tmpT += dtime;
//	if(tmpT > 5.0 && !paused && tmpT < 6.0){
//		this->pauseBackground();
//		paused = true;
//	}
//	if(tmpT > 10.0 && paused){
//		paused = false;
//		this->resumeBackground();
//	}
//	return;
//}

bool OgreSND::SoundMgr::playSelectionSound(Entity381 et){
        Ogre::Vector3 pos = et.pos;

        if (et.soundFile == ""){
            std::cout << "There is no registered selection sounds for this entity type" << std::endl;
            return false; //there is no sound to play
        }
        this->playAudioSourceIndex(et.audioId);
        setSoundPosition(sourceInfo[et.audioId].source, pos);

        return true;
}

/*bool OgreSND::SoundMgr::playEntityBornSound(FastEcslent::EntityType et, OgreGFX::GFXNode *gfxNode){
        Ogre::Vector3 pos = gfxNode->node->getPosition();

        int sounds = 0;
        int arrayIndex = 0;
        for (int i = 0; i < soundPerEnt; i++){
            if (creationSoundsDictionary[et][i] != -1)
                sounds++;
            else
                break;
        }
        if (sounds == 0){
            std::cout << "There is no registered new born sounds for this entity type" << std::endl;
            return false; //there is no sound to play
        }
        else if (sounds == 1)
            arrayIndex = 0;
        else{
            arrayIndex = rand() % sounds; //randomly choose
        }
        int sourceIndex = creationSoundsDictionary[et][arrayIndex];
        this->playAudioSourceIndex(sourceIndex, true);
        setSoundPosition(sourceInfo[sourceIndex].source, pos);

        return true;
}

bool OgreSND::SoundMgr::playExplosionSound(FastEcslent::EntityType et, OgreGFX::GFXNode *gfxNode){
        Ogre::Vector3 pos = gfxNode->node->getPosition();
        int sounds = 0;
        int arrayIndex = 0;
        for (int i = 0; i < soundPerEnt; i++){
            if (battleSoundsDictionary[et][i] != -1)
                sounds++;
            else
                break;
        }
        if (sounds == 0){
            std::cout << "There is no registered battle sounds for this entity type" << std::endl;
            return false; //there is no sound to play
        }
        else if (sounds == 1)
            arrayIndex = 0;
        else{
            arrayIndex = rand() % sounds; //randomly choose
        }
        int sourceIndex = battleSoundsDictionary[et][arrayIndex];
        this->playAudioSourceIndex(sourceIndex, true);
        setSoundPosition(sourceInfo[sourceIndex].source, pos);

        return true;
}

bool OgreSND::SoundMgr::playExplosionSound(OgreGFX::GFXNode *gfxNode){
        Ogre::Vector3 pos = gfxNode->node->getPosition();

        return false;

        if (this->playAudio(battleSoundSource, true)){
                return setSoundPosition(battleSoundSource, pos);
        }
        else
            return false;
}*/

void OgreSND::SoundMgr::releaseLevel(void){
	// release stuff loaded for this level
	return;
}

void OgreSND::SoundMgr::cleanup(void){
	return;
}

void OgreSND::SoundMgr::printAudioDevices(const ALCchar *devices){

	const ALCchar *device = devices;
	const ALCchar *next   = devices+1;
	size_t len = 0;

	std::cout << "Devices list:" << std::endl;
	while(device && *device != '\0' && next && *next != '\0') {
		std::cout << device << std::endl;
		len = strlen(device);
		device += len + 1;
		next   += len + 2;
	}
	std::cout << "------------------" << std::endl;
}

int OgreSND::SoundMgr::printError(const char *ermsg){
	ALCenum error = alGetError();
	if (error != AL_NO_ERROR){
		std::cerr << "SoundManager: ERROR: "<< ermsg << std::endl;
		return -1;
	}
	return 0;
}

inline ALenum OgreSND::SoundMgr::toALFormat(short channels, short samples) {
	bool stereo = (channels > 1);
	switch (samples){
	case 16:
		if (stereo) {
			return AL_FORMAT_STEREO16;
		} else {
			return AL_FORMAT_MONO16;
		}

	case 8:
		if (stereo) {
			return AL_FORMAT_STEREO8;
		} else {
			return AL_FORMAT_MONO8;
		}

	default:
		return -1;

	}
}


int OgreSND::SoundMgr::getBufferId(std::string filename){
	// two cases
	//1. sound is already in a buffer, then return the buffer's index
	//2. sound is not already in a buffer, then load it into an unused buffer index and return it
	//Case 1:
	for (int i = 0; i < OgreSND::maxAudioBuffers; i++){
		if (this->bufferInfo[i].bufferFilename == filename){
			return i;
		}
	}
	std::cout << "Cannot find buffer, attempting to load file: " << filename << std::endl;
	//Case 2
	for (int i = 0; i < OgreSND::maxAudioBuffers; i++){
		if (this->bufferInfo[i].bufferFilename == ""){
			if (loadAudio(filename, i)){
				return i;
			} else {
				std::cerr << "getBufferId::cannot load audio from file: " << filename << std::endl;
				return -1;
			}
		}
	}

	return -1;
}

int OgreSND::SoundMgr::getEmptySourceIndex(){
	for (int i = 0; i < OgreSND::maxAudioSources; i++){
		if (!this->sourceInfo[i].inUse){
			return i;
		}
	}
	return -1;
}

/*bool OgreSND::SoundMgr::registerCreate(FastEcslent::EntityType et, std::string filename){
    unsigned int sid;
    //check if that file is already assigned to a source
    for (int j = 0; j < maxAudioSources; j++){
        if (std::strcmp(sourceDictionary[j].c_str(), filename.c_str()) == 0){
                int lastIndex = -1;
                for (int i = 0; i < soundPerEnt; i++){
                    if (this->creationSoundsDictionary[et][i] == -1){
                        lastIndex = i;
                        break;
                    }
                }
                if (lastIndex == -1){ //all permitted number of sounds for this type are already assigned
                    std::cout << "Could not register new sound, max allowed number per entity reached" << std::endl;
                    return false;
                }
                this->creationSoundsDictionary[et][lastIndex] = j;
                return true;
        }
    }
    if (this->reserveAudio(filename, false, sid)){
                int lastIndex = -1;
                for (int i = 0; i < soundPerEnt; i++){
                    if (this->creationSoundsDictionary[et][i] == -1){
                        lastIndex = i;
                        break;
                    }
                }
                if (lastIndex == -1){ //all permitted number of sounds for this type are already assigned
                    std::cout << "Could not register new sound, max allowed number per entity reached" << std::endl;
                    return false;
                }
                this->creationSoundsDictionary[et][lastIndex] = sid;
                alSourcei(this->sourceInfo[sid].source, AL_REFERENCE_DISTANCE, 2000.0f);
                alSourcei(this->sourceInfo[sid].source, AL_MAX_DISTANCE, 8000.0f);

                sourceDictionary[sid] = filename;
                return true;
    }
    else
        return false;
}

*/
bool OgreSND::SoundMgr::registerSelection(Entity381 ent, std::string filename){
    unsigned int sid;
    if (this->reserveAudio(filename, false, sid)){
                int lastIndex = -1;
                for (int i = 0; i < soundPerEnt; i++){
                    if (this->selectionSoundsDictionary[ent.audioId][i] == -1){
                        lastIndex = i;
                        break;
                    }
                }
                if (lastIndex == -1){ //all permitted number of sounds for this type are already assigned
                    std::cout << "Could not register new sound, max allowed number per entity reached" << std::endl;
                    return false;
                }
                this->selectionSoundsDictionary[ent.audioId][lastIndex] = sid;
                alSourcei(this->sourceInfo[sid].source, AL_REFERENCE_DISTANCE, 2000.0f);
                alSourcei(this->sourceInfo[sid].source, AL_MAX_DISTANCE, 8000.0f);

                sourceDictionary[sid] = filename;
                ent.soundFile = filename;
                return true;
    }
    else
        return false;
}

/*bool OgreSND::SoundMgr::registerBattleSound(FastEcslent::EntityType et, std::string filename){
    unsigned int sid;
    //check if that file is already assigned to a source
    for (int j = 0; j < maxAudioSources; j++){
        if (std::strcmp(sourceDictionary[j].c_str(), filename.c_str()) == 0){
                int lastIndex = -1;
                for (int i = 0; i < soundPerEnt; i++){
                    if (this->battleSoundsDictionary[et][i] == -1){
                        lastIndex = i;
                        break;
                    }
                }
                if (lastIndex == -1){ //all permitted number of sounds for this type are already assigned
                    std::cout << "Could not register new sound, max allowed number per entity reached" << std::endl;
                    return false;
                }
                this->battleSoundsDictionary[et][lastIndex] = j;
                return true;
        }
    }
    if (this->reserveAudio(filename, false, sid)){
                int lastIndex = -1;
                for (int i = 0; i < soundPerEnt; i++){
                    if (this->battleSoundsDictionary[et][i] == -1){
                        lastIndex = i;
                        break;
                    }
                }
                if (lastIndex == -1){ //all permitted number of sounds for this type are already assigned
                    std::cout << "Could not register new sound, max allowed number per entity reached" << std::endl;
                    return false;
                }

                this->battleSoundsDictionary[et][lastIndex] = sid;
                alSourcei(this->sourceInfo[sid].source, AL_REFERENCE_DISTANCE, 2000.0f);
                alSourcei(this->sourceInfo[sid].source, AL_MAX_DISTANCE, 8000.0f);

                sourceDictionary[sid] = filename;
                return true;
    }
    else
        return false;
}

//specific for sound managers everywhere
/**
 * Reserves a source name and binds it to a buffer. It returns status AND the index in sourcesInfo of this
 * sound's audioId
  */
bool OgreSND::SoundMgr::reserveAudio(std::string filename, bool loop, unsigned int &sourceInfoIndex){
//bool OgreSND::SoundMgr::reserveAudio(std::string filename, bool loop){ //

	alGetError();
	int bufferId = this->getBufferId(filename); // if buffer not generated - it is generated in loadAudio
	if(bufferId < 0) {
		std::cout << "GetBufferId: Error loading: " << filename << std::endl;
		std::cout << "All buffers in use, you will need to release buffers before you can get this sound to play: " << filename << std::endl;
		return false;
	}

	int index = getEmptySourceIndex();
	if (index < 0) {
		std::cout << "All sources in use, you will need to release sources before you can get this sound to play: " << filename << std::endl;
		return false;
	}
	if (!alIsSource(this->sourceInfo[index].source)){
		alGenSources(1, &this->sourceInfo[index].source);
		if(printError("ReserveAudio::Cannot Generate source") < 0){
			return false;
		}
		std::cout << "Generated Source " << std::endl;
	}

	resetSource(this->sourceInfo[index].source);

	if(loop){
		alSourcei(this->sourceInfo[index].source, AL_LOOPING, AL_TRUE);
		if(printError("Source looping") < 0){
			return false;
		}
	}
	/*******************************************************************************************/
	sourceInfoIndex = index; // to be returned**************************************
	/*******************************************************************************************/
	this->sourceInfo[index].inUse = true;
	alSourcei(this->sourceInfo[sourceInfoIndex].source, AL_BUFFER, this->bufferInfo[bufferId].buffer);
	if (printError("Error in binding source to buffer for ") < 0){
		return false;
	}
	return true; //return error code
}

bool OgreSND::SoundMgr::resetSource(ALuint sourceId){

	alSourcef(sourceId, AL_PITCH, 1);
	if (printError("Source pitch") < 0)
		return false;

	alSourcef(sourceId, AL_GAIN, 1);
	if(printError("Source Gain") < 0)
		return false;

	alSource3f(sourceId, AL_POSITION, 0, 0, 0);
	if(printError("Source position") < 0)
		return false;

	alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
	if (printError("Source velocity") < 0)
		return false;

	return true;
}

/**
 * Releases a source name. First finds it souceInfo, stopsAudio playing if any, then deletes the source
 *
 *
 */
bool OgreSND::SoundMgr::releaseSourceIndex(int sid){

	ALuint source = this->sourceInfo[sid].source;
	if (! alIsSource(source)){
		std::cerr << "ReleaseSource:: is not a source!" << source << std::endl;
		return false;
	}
	if(!stopAudio(source)){
		std::cerr << "ReleaseSource:: Could not stop audio before release" << source << std::endl;
		return false;
	}
	alDeleteSources(1, &this->sourceInfo[sid].source);
	if (printError("ReleaseAudio::Cannot delete source") < 0){
		return false;
	}
	this->sourceInfo[sid].inUse = false;
	return true;
}

bool OgreSND::SoundMgr::releaseSource(ALuint audioId){
	for(int i = 0; i < OgreSND::maxAudioSources; i++){
		if (this->sourceInfo[i].source == audioId){
			return releaseSourceIndex(i);
		}
	}
	return false;
}



std::string OgreSND::SoundMgr::getFQFNFromFilename(std::string filename){
//	Ogre::FileInfoListPtr files = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", filename, true);
//	Ogre::ConstVectorIterator<Ogre::FileInfoList> fileInfoIter(files.get()->begin(), files.get()->end());
//	std::string pathname, fname;
//	while(fileInfoIter.hasMoreElements()) {
//		pathname = fileInfoIter.peekNext().archive->getName();
//		fname = fileInfoIter.peekNext().filename;
//	}
//	return pathname+fname;
	return filename;
}

//specific to FastEcslsent------------------------------------------------------------------------------------

bool OgreSND::SoundMgr::stopBackground(){

	alSourceStop(this->backgroundMusicSource);
	if (printError("Stop background music") < 0){
		return false;
	} else {
		return true;
	}


}

bool OgreSND::SoundMgr::pauseBackground(){
	alSourcePause(this->backgroundMusicSource);
	if(printError("PauseBackground Source") < 0){
		return false;
	} else {
		return true;
	}
}

bool OgreSND::SoundMgr::resumeBackground(){
	alSourcePlay(this->backgroundMusicSource);
	if(printError("Resume background Source") < 0){
		return false;
	} else {
		return true;
	}
}

bool OgreSND::SoundMgr::loadAudio(std::string filename, int index){
	alGetError();//init errors
	std::string fqfn = getFQFNFromFilename(filename);
	std::cout << "SoundManager Music file: " << fqfn << " is being readied" << std::endl;
	if(fqfn == "")
		return false;

	this->bufferInfo[index].wave = WaveOpenFileForReading(filename.c_str());

	if(!this->bufferInfo[index].wave){
		std::cerr << "OgreSND::SoundMgr::loadAudio::ERROR: Cannot open wave file for reading" << std::endl;
		return false;
	}
	int ret = WaveSeekFile(0, this->bufferInfo[index].wave);
	if (ret) {
		std::cerr << "OgreSND::SoundMgr::loadAudio::ERROR: Cannot seek" << std::endl;
		return false;
	}
	char *tmpBuf = (char *) malloc(this->bufferInfo[index].wave->dataSize);
	//this->backgroundBufferData = (char *) malloc(this->backgroundWaveInfo->dataSize);
	if(!tmpBuf){
		std::cerr << "OgreSND::SoundMgr::loadAudio::ERROR: in malloc" << std::endl;
		return false;
	}
	ret = WaveReadFile(tmpBuf, this->bufferInfo[index].wave->dataSize, this->bufferInfo[index].wave);
	if(ret != (int) this->bufferInfo[index].wave->dataSize){
		std::cerr << "ERROR: OgreSND::SoundMgr::loadAudio: short read " << ret << " wanted: " << this->bufferInfo[index].wave->dataSize << std::endl;
		return false;
	}

	if(!alIsBuffer(this->bufferInfo[index].buffer) || this->bufferInfo[index].bufferFilename == ""){
		//bufferFilename == "" means not a buffer
		alGenBuffers(1, &this->bufferInfo[index].buffer);
		if (printError("Cannot generate buffers") < 0) {
			return false;
		}
		std::cout << "Generated Buffer " << std::endl;
	}

	alBufferData(this->bufferInfo[index].buffer,
			toALFormat(this->bufferInfo[index].wave->channels, this->bufferInfo[index].wave->bitsPerSample),
			tmpBuf, this->bufferInfo[index].wave->dataSize, this->bufferInfo[index].wave->sampleRate);
	free(tmpBuf);
	if(printError("Failed to load bufferData") < 0){
		return false;
	}

	this->bufferInfo[index].bufferFilename = filename;

	return true;
}

bool OgreSND::SoundMgr::loadStartBackground(){
	WaveInfo *wave;


	alGenSources((ALuint)1, &this->backgroundMusicSource);
	printError("Cannot generate source with id 1");

	alSourcef(this->backgroundMusicSource, AL_PITCH, 1);
	printError("Source pitch");

	alSourcef(this->backgroundMusicSource, AL_GAIN, 1);
	printError("Source Gain");

	alSource3f(this->backgroundMusicSource, AL_POSITION, 0, 0, 0);
	printError("Source position");

	alSource3f(this->backgroundMusicSource, AL_VELOCITY, 0, 0, 0);
	printError("Source velocity");

	alSourcei(this->backgroundMusicSource, AL_LOOPING, AL_TRUE);
	printError("Source looping");

	alGenBuffers(1, &this->backgroundMusicBuffer);
	printError("Buffer generation");

	std::string fqfn = getFQFNFromFilename(OgreSND::backgroundMusicFilename);
	std::cout << "SoundManager backgroundMusic file: " << fqfn << " is being readied" << std::endl;
	if(fqfn == "")
		return false;

	this->backgroundWaveInfo = WaveOpenFileForReading(fqfn.c_str());
	if(!this->backgroundWaveInfo){
		std::cerr << "ERROR: Cannot open wave file for reading" << std::endl;
		return false;
	}
	int ret = WaveSeekFile(0, this->backgroundWaveInfo);
	if (ret) {
		std::cerr << "ERROR: Cannot seek" << std::endl;
		return false;
	}
	char *tmpBuf = (char *) malloc(this->backgroundWaveInfo->dataSize);
	//this->backgroundBufferData = (char *) malloc(this->backgroundWaveInfo->dataSize);
	if(!tmpBuf){
		std::cerr << "ERROR: in malloc" << std::endl;
		return false;
	}
	ret = WaveReadFile(tmpBuf, this->backgroundWaveInfo->dataSize, this->backgroundWaveInfo);
	if(ret != (int) this->backgroundWaveInfo->dataSize){
		std::cerr << "ERROR: short read " << ret << " wanted: " << this->backgroundWaveInfo->dataSize << std::endl;
		return false;
	}
	alBufferData(this->backgroundMusicBuffer,
			toALFormat(this->backgroundWaveInfo->channels, this->backgroundWaveInfo->bitsPerSample),
			tmpBuf, this->backgroundWaveInfo->dataSize, this->backgroundWaveInfo->sampleRate);
	if(printError("Failed to load bufferData") < 0){
		return false;
	}

	free(tmpBuf);

	alSourcei(this->backgroundMusicSource, AL_BUFFER, this->backgroundMusicBuffer);
	printError("Source binding");

	alSourcePlay(this->backgroundMusicSource);
	printError("Playing");


	return true;
}


// Returns true if we can play the sound. Rewinds sound if already playing and forceRestart is true,
// false if error
bool OgreSND::SoundMgr::playAudio(ALuint audioId, bool forceRestart ){
        if (!this->isEnabled)
                return false;
	if (!alIsSource(audioId))
		return false;

	ALint source_state;
	alGetSourcei(audioId, AL_SOURCE_STATE, &source_state);
	if(printError("Get source state") < 0)
		return false;
	if(source_state == AL_PLAYING){
		if (forceRestart){
			stopAudio(audioId);
			if (printError("PlayAudio:: Could not stop already playing song") < 0){
				return false;
			}
                        alSourcePlay(audioId);
		}
		return true;
	}
	alSourcePlay(audioId);
	if(printError("PlayAudio:: Could not play") < 0)
		return false;
	return true;
}

bool OgreSND::SoundMgr::playAudio(ALuint audioId){
	return playAudio(audioId, false);
}

bool OgreSND::SoundMgr::playAudioSourceIndex(int sid, bool forceRestart){
	return playAudio(this->sourceInfo[sid].source, forceRestart);
}

bool OgreSND::SoundMgr::playAudioSourceIndex(int sid){
	return playAudio(this->sourceInfo[sid].source, false);
}

bool OgreSND::SoundMgr::stopAudio(ALuint audioId){
	if (alIsSource(audioId)){
		alSourceStop(audioId);
		if (printError("StopAudio::cannot stop source: " + audioId) < 0){
			return false;
		}
	} else {
		std::cerr << "StopAudio:: Is not a source: " << audioId << std::endl;
		return false;
	}
	return true;
}

bool OgreSND::SoundMgr::stopAudioSourceIndex(int sid){
        return this->stopAudio(this->sourceInfo[sid].source);
}

bool OgreSND::SoundMgr::isAudioPlaying(ALuint audioId){
    	if (!alIsSource(audioId))
		return false;

	ALint source_state;
	alGetSourcei(audioId, AL_SOURCE_STATE, &source_state);
	if(printError("Get source state") < 0)
		return false;
	if(source_state == AL_PLAYING){
		return true;
	}
        if(printError("PlayAudio:: Could not play") < 0)
		return false;
}

bool OgreSND::SoundMgr::stopAllAudio( void ){
	for(int i = 0; i < OgreSND::maxAudioSources; i++){
		stopAudio(this->sourceInfo[i].source);
	}
	return true;
}

bool OgreSND::SoundMgr::pauseAudioSourceIndex(int sid){

	pauseAudio(this->sourceInfo[sid].source);
	return true;
}
bool OgreSND::SoundMgr::pauseAudio(ALuint audioId ){
	if(!alIsSource(audioId))
		return false;
	alSourcePause(audioId);
	if (printError("PauseAudio::Cannot pause: " + audioId) < 0){
		return false;
	}
	return true;
}

bool OgreSND::SoundMgr::pauseAllAudio( void ){
	for(int i = 0; i < OgreSND::maxAudioSources; i++){
		pauseAudio(this->sourceInfo[i].source);
	}
	return true;
}

bool OgreSND::SoundMgr::resumeAudio(ALuint audioId){
	return playAudio(audioId);
}

bool OgreSND::SoundMgr::resumeAllAudio( void ){
	for(int i = 0; i < OgreSND::maxAudioSources; i++){
		playAudio(this->sourceInfo[i].source);
	}
	return true;
}

bool OgreSND::SoundMgr::setSoundPosition(ALuint audioId, Ogre::Vector3 position ){

	alSourcef(audioId, AL_GAIN, 1.0);
	if(printError("Source Gain") < 0)
		return false;


	alSource3f(audioId, AL_POSITION, position.x, position.y, position.z);
	if (printError("SetSoundPosition::Source position") < 0) return false;
	return true;;
}


bool OgreSND::SoundMgr::setSoundDisposition( ALuint audioId, Ogre::Vector3 position, Ogre::Vector3 velocity, Ogre::Vector3 direction ){
	alSource3f(audioId, AL_POSITION, position.x, position.y, position.z);
	if (printError("SetSoundDisPosition::Source position") < 0) return false;

	alSource3f(audioId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	if (printError("SetSoundDisPosition::Source velocity") < 0) return false;

	return true;
}


bool OgreSND::SoundMgr::setSound( ALuint audioID, Ogre::Vector3 position,
    Ogre::Vector3 velocity, Ogre::Vector3 direction, float maxDistance,
    bool playNow, bool forceRestart, float minGain ){
	return false;
}


bool OgreSND::SoundMgr::setListenerDisposition( Ogre::Vector3 position, Ogre::Vector3 velocity, Ogre::Quaternion orientation ){
	return false;
}

//void OgreSND::SoundMgr::copySoundState(){
//	for(int i = 0; i < gim->nGFXNodes; i++){
//                FastEcslent::UnitAI* ai = dynamic_cast<FastEcslent::UnitAI*>(gim->engine->entityMgr->ents[i]->getAspect(FastEcslent::UNITAI));
//                for(std::deque<FastEcslent::Command*>::iterator it = ai->commands.begin(); it!= ai->commands.end();it++){
//                   if((*it)->commandType == FastEcslent::MoveCommand){// || (*it)->commandType == FastEcslent::GatherCommand){
//                        FastEcslent::EntityType et = gim->engine->entityMgr->ents[i]->entityType;
//                        attachSelectedNodeToSoundIndex(&(gim->gfxNodes[i]), /*scvId, soundDictionary[et],*/ (*it)->commandType);
//                    }
//                }
//	}
//}
