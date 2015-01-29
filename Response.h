/*
 * Response.h
 *
 *  Created on: Jan 15, 2015
 *      Author: alex, Thurston
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_
#include "Code.h"

class Response 
{
	public:
		Response(const int numGood=-1, const int numBad=-1); // create a response object with default invalid values
		const int getCorrect(); // return how many values are correct
		const int getIncorrect(); // return how many values are incorrect
		void setNumCorrect(int const newNumCorrect); // set how many values are correct 
		void setNumIncorrect(int const newNumIncorrect); // set how many values are incorrect
		const void printResponse(); // print a response
		const bool checkWin(const bool announce=false); // check if this is a winning response
		const bool checkSame(const Response r); // check if a given response matches this one
		friend ostream& operator<<(ostream &ostr,Response r); // creates an output stream object
		const bool operator==(Response r); // determines if two responses have equivalent data
		//const Response operator=(Response r); // assignes the data of the response from the given response
        Response operator=(Response g);
		virtual ~Response();
	private:
		 int numCorrect; // number of matching values
		 int numIncorrect; // number of correct values in incorrect position
};

#endif /* RESPONSE_H_ */
