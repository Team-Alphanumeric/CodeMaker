/*
 * Mastermind.h
 *
 *  Created on: Jan 15, 2015
 *      Author: alex
 */

#ifndef MASTERMIND_H_
#define MASTERMIND_H_
#include "Code.h"
#include "Response.h"
#include <cmath>

class Mastermind
{
public:
	Mastermind();
	void humanSet(Code c);
	Response getResponse(Code sc, Code gs);
	void playGame();
	void playComp();
	void playSeries();
	Code agentGuess();
	const bool isConsistent(const Code gc);
	const bool isConsistent(const Code gc, const Code gr, const Response rc);
	const short int getBase() {return base;}
	virtual ~Mastermind();
protected:
	Code secret; // secret for the game
	Code guess; // current guess
	Response r1; // current respone
	vector <Code> gs; // previous guesses
	vector <Response> rp; // previous replies
	vector <bool> cs; // currently consistent values (indexed)
private:
	short int base;
};

#endif /* MASTERMIND_H_ */
