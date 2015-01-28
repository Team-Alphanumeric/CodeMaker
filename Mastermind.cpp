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

using namespace std;

//constructor method
Mastermind::Mastermind()
{
	base = secret.getBase(); // pull the value of the base from the Code object
	cs.assign(pow(base,4),true); // set all values as consistent
}

// determines if a guess (gc) is consistent with all previous guesses(gs) and responses(rp)
const bool Mastermind::isConsistent(const Code gc)
{
	int i;
	// accumulator for validity
	bool temp = true;
	// check each past value
	for(i=0;i<(gs.size);i++)
	{
		temp = temp && (rp[i]==getResponse(gc,gs[i]));
		if(!temp){break;}
	}
	// return result
	return temp;
}

// determines if the remainder guess(gr) is consistent with all previous guesses(gs) as well 
// as the candidate guess(gc) and candidate reply (rc)
const bool Mastermind::isConsistent(const Code gc, const Code gr, const Response rc)
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
	short int score[base];
	// check all codes as a guess candidate
	for(int k=0; k<pow(base,4); k++)
	{
		// reset score to 0
		score[k]=0;
		// ignore value if already deemed inconsistent
		if(!cs[k])
		{
			// generate corresponding guess candidate 
			gc = Code(k);
			// check if guess candidate is consistent
			if(!isConsistent(gc)) 
			{ cs[k] = false; score[k] = -1; } // mark inconsistent codes in the class
			else
			{
				// check all remainder guesses
				for(int l=0;l<pow(base,4);l++)
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
							if(isConsistent(gc,gr,rc)) {gscore++;}
						}
					}
				}
				// add score from the remainder guess
				score[k] += gscore;
			}
		}
	}
	// now that we check all dem codes . . . 
	// select the consistent code with the lowest score
	int mindex = 0, minimum = score[0];
	for(int k=0;k<pow(base,4); k++)
	{
		// accumulate lowest value/index
		if(score[k] < minimum ) { mindex = k; minimum = score[k]; } 
	}
	// return the lowest scoring code
	return Code(mindex);
}
//This function prompts the user to enter his guess which then stores it as a vector
void Mastermind::humanSet(Code c)
{
	//create variable i to be used for the for loop
	int i=0;
	
	//create a temporary array that will be inputed into the .setCode function 
	// from the Code class to convert the arrary into a vector
	int tempArr[4]={0,0,0,0};
	char charArr[5]={0,0,0,0,0};
		
	//this inputs the user for 4 digits which then puts them in to the array
	cout << "Please type in 4 digits:" << "\n";
	while(i<4)
	{
		scanf("%s",&charArr[i]); // read in each digit as a character
		// convert to integer until termination character ( a 0)
		while(charArr[i] != 0)
		{	
			tempArr[i] = ((int)charArr[i]) - 48;	
			i++;
		}
		
	}
	//This passes the array of the 4 digits that the user entered into the setCode function of the guess object that will store the human guess as a vector
	c.setCode(tempArr);
	
	return;
}

//this function is passed the secret and the guess and run the check correct, and check incorrect and returns the response which is the number correct and the num incorrect
//this returns a bool to main which decides whether the human guessed the write code or has to guess again
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
	// number of guesses the computer has entered and can enter
	int numIterations=0; const  int guesses = 10;

	// user creates a new secret for a new game
	humanSet(secret);
	
	// reset response
	r1.setNumCorrect(0); r1.setNumIncorrect(0);
		
	// print out the secret code to the screen for testing purposes
	cout << "The secret code is "; secret.printCode();
	
	while(!(r1.checkWin(numIterations>0)) && (numIterations < guesses))
	{
		// generate a guess
		guess = agentGuess();

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
			// prints out the saddiness that is the game being quit
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

Mastermind::~Mastermind() {
	/// TODO Auto-generated destructor stub
}
