//============================================================================
// Name        : CodeMaker.cpp
// Author      : Alex Agudelo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include "Mastermind.h"

short int Code::base = 6;

int main()
{
	Mastermind playGame1;
	playGame1.playComp();
	return 0;
}
