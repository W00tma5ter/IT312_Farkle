// Written by: Eric Hamilton - 06/20/2021.

#pragma once

#include <cstdlib>
#include <iostream>

using namespace std;

// Player class, holds name and score variables with associated setters/getters.
// Special functions to add/subtract player score are also provided.
class Player
{
private:
	//*******************
	// CLASS VARIABLES. *
	//*******************

	string name = "";
	unsigned int score = 0;
	unsigned int roundScore = 0;

public:
	//****************
	// CONSTRUCTORS. *
	//****************

	// Constructor: default.
	Player() {
		name = "NONE";
		score = 0;
		roundScore = 0;
	}
	// Constructor: accepts name.
	Player(const string& n) {
		name = n;
		score = 0;
	}
	// Constructor: accepts name, score.
	Player(const string& n, const int& s) {
		name = n;
		score = s;
	}

	//*******************
	// SETTERS/GETTERS. *
	//*******************

	// Set player name.
	void SetName(const string& n) {
		name = n;
	}
	// Get player name.
	const string GetName() {
		return name;
	}
	// Set player score.
	void SetScore(const int& s) {
		score = s;
	}
	// Get player score.
	const unsigned int GetScore() {
		return score;
	}
	// Set round score.
	void SetRoundScore(const int& s) {
		roundScore = s;
	}
	// Get round score
	const unsigned int GetRoundScore() {
		return roundScore;
	}

	//*********************
	// SPECIAL FUNCTIONS. *
	//*********************

	// Add points to player score.
	void AddScore(const int& a) {
		score += a;
	}
	// Subtract points from player score.
	void SubScore(const int& s) {
		score -= s;
	}
	// Add to roundScore.
	void AddRoundScore(const int& s) {
		roundScore += s;
	}
	// Reset roundScore to 0;
	void ResetRoundScore() {
		roundScore = 0;
	}
	// Add roundScore to total score
	void AddRoundToTotal() {
		score += roundScore;
	}

};