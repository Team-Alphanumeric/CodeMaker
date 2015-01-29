/*
 * Mastermind.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: alex
 */

#include <iostream>
#include "Mastermind.h"
#include "Code.h"
#include "Response.h"
#include <vector>
#include <stdio.h>
#include <exception>

using namespace std;

//constructor method
Mastermind::Mastermind()
{
	cs.assign(pow(Code::base,4),true); // set all values as consistent
}

// determines if a guess (gc) is consistent with all previous guesses(gs) and responses(rp)
const bool Mastermind::isConsistent(const Code gc)
{
	// must make i an unsigned int because gs.size() returns a unsigned int and
	// for the for loop to work, both variables must be of the same type
	unsigned int i;
	// accumulator for validity
	bool temp = true;
	// check each past value
	for(i=0;i<(gs.size());i++)
	{
		temp = temp && (rp[i]==getResponse(gc,gs[i]));
		if(!temp){break;}
	}
	// return result
	return temp;
}

// determines if the remainder guess(gr) is consistent with all previous guesses(gs) as well 
// as the candidate guess(gc) and candidate reply (rc)
const bool Mastermind::isConsistent(const Code gc, const Code gr, Response rc)
{
	// check if remainder guess is consistent with all previous guesses
	bool temp = isConsistent(gr);
	// check if remainder guess is consistent with candidates
	temp = temp && (rc==getResponse(gr,gc));
	// return result
	return temp;
}

// determines the optimal Code for the game by searching through all permutations 
// of codes and finding the one with the smallest remaining number of consistent guesses
Code Mastermind::agentGuess()
{
	// for the guess candidate's score
	Code gc;
	Code gr;
	short int gscore;
	int arraySize = pow(Code::base,4);
	int score[arraySize];
	// check all codes as a guess candidate
	for(int k=0; k<pow(Code::base,4); k++)
	{
		// reset score to 0
		score[k]=0;
		// ignore value if already deemed inconsistent
		if(cs[k])
		{
			//throw expection if the value of k went over the potential index values for codes
			if(k > pow(Code::base,4))
			{
				throw "Value of k Exceeds index capability";
			}
			// generate corresponding guess candidate 
			gc = Code(k);
			// check if guess candidate is consistent
			if(!isConsistent(gc)) 
			{ cs[k] = false; /*score[k] = -1;*/ } // mark inconsistent codes in the class
			else
			{
				// check all remainder guesses
				for(int l=0;l<pow(Code::base,4);l++)
				{
					// ignore values already deeemed inconsistent
					if(!cs[l]) { break; }
					// create remaining guess object
					gr = Code(l);
					// create an integer to store this remainder guess' score
					gscore=0;
					// check all responses for validity
					for(int i=0;i<=4;i++)
					{
						for(int j=0;j<=4-i;j++)
						{
							// create response candidate
							Response rc(i,j);
							// if current remainder guess is consistent with candidates (response and guess),
							// increment score for this remainder guess
							if(isConsistent(gc,gr,rc))
							{
								gscore++;
							}
						}
					}
				}
				score[k] += gscore;
			}
		}
	}
	// now that we check all dem codes . . . 
	int mindex = 0;
	int minimum = score[0];
	// select the consistent code with the lowest score to
	//initalize the first possible Code
	for(int k=0;k<pow(Code::base,4);k++)
	{
		if(cs[k] == true && score[k] != 0)
		{
			mindex = k;
			minimum = score[k];
			break;
		}
	}
	if(score[mindex] == 0)
	{
		throw "The guess candidate score value equals zero";
	}
	else if(score[mindex] < 0)
	{
		throw "The guess candidate score value is less then zero";
	}
	//runs through all the possible scores so that the guess Candidate with the lowest possible
	//score is found and that score is returned to agent guess as the next guess the computer is going to make
	for(int k=0;k<pow(Code::base,4); k++)
	{
		// accumulate lowest value/index
		if(score[k] < minimum && cs[k] == true ) { mindex = k; minimum = score[k]; }
	}
	// return the lowest scoring code
	return Code(mindex);
}
//This function prompts the user to enter his guess which then stores it as a vector

void Mastermind::humanSet(Code &c)
{

	//create variable i to be used for the for loop
	int i=0;
	//create a temporary array that will be inputed into the .setCode function
	// from the Code class to convert the arrary into a vector
	int tempArr[4]={0,0,0,0};
	char charArr[5]={0,0,0,0,0};
	//this inputs the user for 4 digits which then puts them in to the array
	bool rangeDecline=true;
	while(rangeDecline)
	{
		try
		{
			cout << "Please type in 4 digits:" << "\n";
			while(i<4)
			{
				cin >> charArr[i];
				// convert to integer until termination character ( a 0)
				while(charArr[i] != 0)
				{
					int range=0; // determine which alphanumeric ASCII range the numbers fall into
					if((charArr[i]>=48)&&(charArr[i]<=57)) {range = 1;} // range for 0-9
					if((charArr[i]>=65)&&(charArr[i]<=70)) {range = 2;} // range for A-F
					if((charArr[i]>=97)&&(charArr[i]<=102)) {range = 3;} // range for a-f
					switch (range)
					{
						case 1: tempArr[i] = ((int)charArr[i]) - 48; break; // conversion for 0-9
						case 2: tempArr[i] = 10 + ((int)charArr[i]) - 65; break; // conversion for A-F
						case 3: tempArr[i] = 10 + ((int)charArr[i]) - 97; break; // conversion for a-f
						default: throw rangeError("Input invalid!"); break; // error case
					}
					if(tempArr[i] >= Code::base) // for values greater than the radix
					{ throw rangeError("Input greater than accepted values!"); }
					
					else if(tempArr[i] < 0) // for values somehow negative
					{ throw rangeError("Negative value input!"); }
					
					i++; // move to next character
				}
			}
			rangeDecline = 0; // move on if all characters are accepted
		}
		catch(rangeError) { cout rangeError.what(); }
	}
	// This passes the array of the 4 digits that the user entered into the 
	// setCode function of the guess object that will store the human guess as a vector
	c.setCode(tempArr);
	return;

}

// this function is passed the secret and the guess and run the check correct,
// and check incorrect and returns the response which is the number correct and the num incorrect
// this returns a boolean to main which decides whether the human guessed the write code or has to guess again
Response Mastermind::getResponse(Code sc, Code gs)
{
	// set numCorrect and inCorrect for a new response r and return r
	Response r;
	return (r.setNumCorrect(sc.checkCorrect(gs)), r.setNumIncorrect(sc.checkIncorrect(gs)), r);
}

void Mastermind::playGame()
{
	// number of guesses the user has entered and can enter
	int numIterations=0; const  int guesses = 10;

	// create a new code for a new game
	secret.setRandomCode();
	
	// reset response
	r1.setNumCorrect(0); r1.setNumIncorrect(0);
		
	// print out the secret code to the screen for testing purposes
	cout << "The secret code is "; secret.printCode();
	//for first iteration, setting a default start value

	while(!(r1.checkWin(numIterations>0)) && (numIterations < guesses))
	{
		// prompt user for a guess
		humanSet(guess);

		// receive and print response
		// note: getResponse returns a response object that is printed by printResponse
		getResponse(secret,guess).printResponse();

		// move to next guess
		numIterations++;
	}
	
	// shame losers for losing
	if(numIterations == guesses)	{ cout << "You have had 10 guesses, now you lost.\n"; }
	
	return;
}

void Mastermind::playComp()
{
	//reduces the size of these vectors to zero in case of the case that the user wants to replay the game so it resets the vectors to
	//the initalaize size
	gs.resize(0);
	rp.resize(0);
	//variable is meant for the humset exception and it loops until the user enters a code within the correct range
	bool humanSetTest=true;
	// number of guesses the computer has entered and can enter
	int numIterations=0; const int guesses = 10;
	// user creates a new secret for a new game
	//exception is called on humanSet testing whether or not the values entered by the user for the
	//secret code is within the appropriate bounds, if it isn't, it calls the exception and loops for the proper numbers to be entered
	while(humanSetTest)
	{
		try
		{
			humanSet(secret);
			humanSetTest=false;
		}
		catch(const char* msg)
		{
			cout << msg << endl;
			humanSetTest = true;
		}
	}
	// reset response
	r1.setNumCorrect(0); r1.setNumIncorrect(0);
	// print out the secret code to the screen for testing purposes
	cout << "The secret code is ";cout << secret;
	//this sets the default first guess to (2,2,4,4) and stores it in the gs vector as element [0]
	//this is necessary because the agent guess does not have a default case for the first code yet
	int firstIteration[] = {2,2,4,4};
	guess.setCode(firstIteration);
	gs.push_back(guess);
	//stores the response of the first default guess into the vector rp as element [0]
	rp.push_back(getResponse(secret, guess));
	while(!(r1.checkWin(numIterations>0)) && (numIterations < guesses))
	{
		//add spacing in console to output the result neater
		cout << endl;
		// generate a guess
		try
		{
			guess = agentGuess();
		}
		catch(const char* msg)
		{
			cout << msg << endl;
		}
		cout << "Computer Guesses " << guess;
		// receive and print response
		r1=getResponse(secret,guess); cout<<r1;

		// store guess and response
		gs.push_back(guess); rp.push_back(r1);
		
		// move to next guess
		numIterations++;

	}
	
	// shame losers for losing
	if(numIterations == guesses)	{ cout << "You have had 10 guesses, now you lost.\n"; }
	
	return;
}

void Mastermind::playSeries()
{
	int playGameAgain=1; // whether or not to continue playing
	
	while(playGameAgain)
	{
		// play a game
		playGame();
		
		// prompts user to play again
		cout << "Do you want to play again? 1 = yes, 2 =no.\n";
		
		// receive the response for the user
		cin >> playGameAgain;
		
		// switch case on the decision of playing again or not
		switch (playGameAgain)
		{
		case 1: // they want to play again	
			// prints out that the happiness that is the user playing again and end case
			cout << "YAYAY try again. \nRestarting game, Creating new Secret code!\n";	break;
			

		case 2: // the user wants to stop playing
			// prints out the sadness that is the game being quit
			cout << "OK fine you had enough, game quitting.\n";
			// change response to boolean false and end case
			playGameAgain =  0; break;
			
			
		default: // an inappropriate response was given
			//prompts user that the input was invalid and end case
			cout << "That is not a correct input, try again.\n"; break;
		
		}
	}	
	// return when finished
	return;
}
void Mastermind::playSeriesComp()
{
	int playGameAgain=1; // whether or not to continue playing

	while(playGameAgain)
	{
		// play a game
		playComp();

		// prompts user to play again
		cout << "Do you want to play again? 1 = yes, 2 =no.\n";

		// receive the response for the user
		cin >> playGameAgain;

		// switch case on the decision of playing again or not
		switch (playGameAgain)
		{
		case 1: // they want to play again
			// prints out that the happiness that is the user playing again and end case
			cout << "YAYAY you want to lose again. \nRestarting game, get ready to enter a new secret code!\n";	break;


		case 2: // the user wants to stop playing
			// prints out the sadness that is the game being quit
			cout << "OK fine you had enough losing to the computer, game quitting.\n";
			// change response to boolean false and end case
			playGameAgain =  0; break;


		default: // an inappropriate response was given
			//prompts user that the input was invalid and end case
			cout << "That is not a correct input, try again.\n"; break;

		}
	}
	// return when finished
	return;
}

Mastermind::~Mastermind() {
	/// TODO Auto-generated destructor stub
}
