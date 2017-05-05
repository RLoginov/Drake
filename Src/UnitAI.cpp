/*
 * UnitAI.cpp
 *
 *  Created on: April 2, 2017
 *      Author: Tyler Goffinet, Robert Loginov
 *      tylergoffinet@cox.net, rloginov@nevada.unr.edu
 */

#include "../Inc/UnitAI.h"


UnitAI::UnitAI(Entity381 * ent): Aspect(ent) {
	commands = std::list<Command*>();
}

UnitAI::~UnitAI(){

}

void UnitAI::Tick(float dt){
	GrabNewCommand();

	// Execute first command
	if (!commands.empty()){
		commands.front()->tick(dt);
		if (commands.front()->done()){
			commands.pop_front();
		}
	}
}

void UnitAI::GrabNewCommand(){
	if (entity->isNewComm){
		if (entity->shouldAdd)
		{
			if (entity->isFollowComm){
				AddCommand(new Follow(entity, entity->leader));
			}
			else{
				AddCommand(new MoveTo(entity, entity->point));
			}
		}

		else
		{
			if (entity->isFollowComm){
				SetCommand(new Follow(entity, entity->leader));
			}
			else{
				SetCommand(new MoveTo(entity, entity->point));
			}
		}
		entity->isNewComm = false;
	}
}

void UnitAI::AddCommand(Command* comm){
	commands.push_back(comm);
	for(std::list<Command*>::const_iterator it = commands.begin(); it != commands.end(); ++it){
		(*it)->print();
	}
	std::cout << std::endl;
}

void UnitAI::SetCommand(Command* comm){
	commands.clear();
	commands.push_back(comm);
	for(std::list<Command*>::const_iterator it = commands.begin(); it != commands.end(); ++it){
		(*it)->print();
	}
	std::cout << std::endl;
}
