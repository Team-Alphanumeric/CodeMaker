/*
 * Response.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: alex, Thurston
 */

#include "Response.h"
#include <iostream>
using namespace std;
#include <stdexcept>


// creates a default response
Response::Response(const int numGood, const int numBad)
{
	//sets the numCorrect and numIncorrect to -1 for debugging purposes
	//if either of them remain the default, then those numbers haven't been touched
	if(numGood > 4 || numBad > 4)
	{ throw invalid_argument("numBad or numGood > 4 instead of between 0-4"); }
	numCorrect = numGood;
	numIncorrect = numBad;
}

//get function that returns the number of Correct positions
const int Response::getCorrect()
{
	return numCorrect;
}

//get function that returns the number of Incorrect positions
const int Response::getIncorrect()
{
	return numIncorrect;
}

//set function to manually set the value of numCorrect
void Response::setNumCorrect(int const newNumCorrect)
{
	if(newNumCorrect > 4)
	{ throw invalid_argument("newNumCorrect > 4 instead of between 0-4"); }
	numCorrect = newNumCorrect; return;
}

//set function to manually set the value of incorrect
void Response::setNumIncorrect(int const newNumIncorrect)
{

	if(newNumIncorrect > 4)
	{ throw invalid_argument("newNumIncorrect > 4 instead of between 0-4"); }
	numIncorrect = newNumIncorrect; return;
}

// print the response
const void Response::printResponse()
{
	cout << "There are "<<numCorrect<<" digits in the correct position and "<<numIncorrect<<" in the wrong position.\n";
	/* printf("Correct: %i\tIncorrect: %i\n",numCorrect,numIncorrect); return; */ /* alternate implementation */
}

//This function checks to see if the two codes are the same and outputs true if they are the same
//and outputs false if they are different
const bool Response::checkSame(Response r)
{
	return (numCorrect == r.getCorrect()) && (numIncorrect == r.getIncorrect());
}

const bool Response::checkWin(const bool announce)
{
	//to check if the user won, it checks to see if the numCorrect is 4
	if(!announce) { return numCorrect==4;	}
	else
	{
		if(numCorrect == 4)
		{
			//returns true which indicates the human guessed the code
			cout << "The computer has guessed the right code!!!! You lost!! Now play again.....\n";
			return true;
		}
		else 
		{
			//returns false which indicates that the human as not guessed the secret code yet
			cout << "Well the computer guessed it wrong...trying again..." << endl;
			return false;
		}
	}
}

// Response assignment operation
Response Response::operator=(Response g)
{ // creates a Response 'r' initialized to current object, then returns 'r'
  // uses parenthesis and comma (sequence operator) to execute 2 commands
	return (numCorrect= g.getCorrect(), numIncorrect=g.getIncorrect(), *this);
}

// Comparison operator
const bool Response::operator==(Response r)
{	return (*this).checkSame(r); }

// prints a response
ostream& operator<<(ostream &ostr,Response r)
{
	return ostr << "Correct: " << r.getCorrect() << "\tIncorrect: " << r.getIncorrect() << endl;
}

// deconstructor stub
Response::~Response()
{	/* TODO Auto-generated destructor stub */ }



