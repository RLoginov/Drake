/*
 * UnitAI.cpp
 *
 *  Created on: April 2, 2017
 *      Author: Tyler Goffinet, Robert Loginov
 *      tylergoffinet@cox.net, rloginov@nevada.unr.edu
 */

#ifndef UNITAI_H_
#define UNITAI_H_


#include "../Inc/Aspect.h"
#include "../Inc/Command.h"

class UnitAI: public Aspect {
public:
	UnitAI(Entity381* ent);
	~UnitAI();
	virtual void Tick(float dt);

	void GrabNewCommand();
	void AddCommand(Command* comm);
	void SetCommand(Command* comm);

	std::list<Command*> commands;
};

#endif /* UNITAI_H_ */
