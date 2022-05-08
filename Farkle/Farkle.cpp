/*----------------------------------------------------------------------------
* Student: 		Eric Hamilton
* Course: 		IT-312-J5323 Software Devel w/C++.Net 21EW5
* Instructor: 	Professor Jeff H. Sanford, M.S.
* Date:			06/20/2021
* Project:		FINAL PROJECT - Farkle Game
*
* Discussion:	I had an absolute blast creating this project! From start to
*				finish i have grown tremendously in my coding ability and 
*				confidence! This has been a great learning experience.
----------------------------------------------------------------------------*/


// Written by: Eric Hamilton - 06/21/2021.

// Farkle.cpp - Implementation of the game Farkle. This file contains the menu logic.
// Game.h - This is the header file for main game logic.
// Game.cpp - This contains all the game logic.
// Player.h - Player class that contains name and scores.
// Dice.h - Dice class for creating dice of x sides and rolling logic.

#include <string>
#include <iostream>
#include <fstream>
#include "Game.h"

using namespace std;

int ValidatedInputNum();
void PrintRules();

int main() {
	Game newGame;
	string strUserInput = "";
	int userInput = 0;

	// Welcome Player.
	cout << "****** Welcome to Farkle! A fun dice game! ******" << endl << endl;
	do {
		cout << endl 
			<< "Please select from the following options:" << endl
			<< "1 : Play Farkle" << endl
			<< "2 : Read Rules of Game" << endl
			<< "3 : Quit Game" << endl << endl
			<< "What is your choice (1-3):" << endl;
		// Get a valid number input from player.
		userInput = ValidatedInputNum();

		switch (userInput) {
		// Start the game.
		case 1:
			newGame.BeginPlay();
			break;
		// Read the rules.
		case 2:
			PrintRules();
			break;
		// Quit the game.
		case 3:
			cout << "Thank you for Playing! Have a nice day!" << endl;
			break;
		default:
			cout << "Invalid choice!" << endl << endl;
		}

		
	} while (userInput != 3);

	return 0;
}

int ValidatedInputNum() {

	string playerInput = "";
	bool pass = false;

	do {
		cin >> playerInput;
		// Check each character in input string for digit.
		for (unsigned int i = 0; i < playerInput.length(); i++) {
			if (!isdigit(playerInput[i])) {
				cout << "Not a valid number, Please Try again." << endl;
				// If non-digit return false.
				pass = false;
				break;
			}
			else pass = true;
		}
	} while (!pass);
	return stoi(playerInput);
}

void PrintRules() {
	string line;
	// Place Rules.txt in root folder with Farkle.exe
	ifstream inFile("Rules.txt");
	// Read each line of file, and print to console.
	cout << endl << endl;
	while (getline(inFile, line)) {
		cout << line << endl;
	}
	// Close file.
	inFile.close();
}