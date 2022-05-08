//Written by: Eric Hamilton - 06/20/2021.

#pragma once

#include <cstdlib>
#include <iostream>

using namespace std;

// Dice class contains logic for rolling dice.
class Dice 
{
private:
	//*******************
	// CLASS VARIABLES. *
	//*******************

	unsigned int dieSides = 6;
	unsigned int totalDice = 6;

public:
	//****************
	// CONSTRUCTORS. *
	//****************

	// Constructor: Default.
	Dice() {
		// Seed random number generator.
		srand(static_cast<long int> (time(NULL)));
	}
	// Constructor: accepts # of sides.
	Dice(const int& s) {
		this; // Run default constructor.
		dieSides = s;
	}
	// Constructor: accepts # of sides, and # of dice.
	Dice(const int& s, const int& t) {
		this; // Run default constructor.
		dieSides = s;
		totalDice = t;
	}

	//*******************
	// SETTERS/GETTERS. *
	//*******************

	// Set # of sides on a die.
	void SetDieSides(const int& s) {
		dieSides = s;
	}
	// Get # of sides on a die.
	const int GetDieSides() {
		return dieSides;
	}
	// Set total # of dice.
	void SetTotalDice(const int& t) {
		totalDice = t;
	}
	// Get toal # of dice.
	const int GetTotalDice() {
		return totalDice;
	}

	//*********************
	// SPECIAL FUNCTIONS. *
	//*********************

	// Roll die with [dieSides] # of sides.
	int Roll() {
		return ((rand() % dieSides) + 1);
	}
};