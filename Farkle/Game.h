// Written by: Eric Hamilton - 06/20/2021.

#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Dice.h"

using namespace std;

// Game class contains all logic for the game of Farkle.
class Game 
{
private:
	//*******************
	// CLASS VARIABLES. *
	//*******************

	Dice dice;
	vector<Player> player;
	unsigned int numPlayers = 0;
	int winnerIndex = -1;
	int maxRounds = 100; // Infinite loop prevention.

	// Prompt for number of players, input names.
	void Setup();
	// Start a round of game.
	void PlayRound();
	// Start a single player turn.
	void PlayTurn(const unsigned int& p);
	// Ask player to hold dice. Returns a vector of dice values held from dice rolled.
	const vector<int> HoldDice(const vector<int>& h, unsigned int& diceInHand, unsigned int& diceHeld);
	// Calclulate score for a vector of dice values.
	const int ScoreDice(const vector<int>& h);
	// Start final round of game.
	void FinalRound();
	// Set the index of winning player.
	void SetWinnerIndex(const unsigned int& w);
	// Get the index of winning player.
	const int GetWinnerIndex();
	// Prompt to continue turn.
	const bool ContinueTurn();
	// Countdown function to break gameplay loop after maxRounds.
	bool Countdown();
	// Check if a string is all numeric.
	const bool NumCheck(const string& s);
	// Check if a string of numbers are in valid range.
	const bool NumsInRange(const string& s, const int& n);
	// Validated user input function for yes/no quesiton.
	const bool ValidYesNoInput();
	// Validated user input function for integer input.
	const int ValidNumberInput();
	// Dice drawing function.
	void DrawDice(const unsigned int& q, const vector<int>& h);
	// Check for a farkle in a roll (no point dice).
	const bool FarkleCheck(const vector<int>& h);
	// Validate if held dice are points dice only.
	const bool ValidHold(const vector<int>& h);
	// Display end game results.
	void EndGame();

public:
	//Constructor. Default.
	Game();

	// Begin Gameplay.
	void BeginPlay();

	

	// FOR TESTING: Sets up 3 Players, modifies variables and outputs changes.
	//Takes in string for caseID, to differentiate runs in console log.
	void PlayerTestCases(const string& caseID);
};