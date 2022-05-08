// Written by: Eric Hamilton - 06/20/2021.

#include "Game.h"

//****************
// CONSTRUCTORS. *
//****************

// Constructor: Default.
Game::Game() {
}

//*******************
// SETTERS/GETTERS. *
//*******************

// Set the index of winning player.
void Game::SetWinnerIndex(const unsigned int& w) {
	winnerIndex = w;
}
// Get the index of winning player.
const int Game::GetWinnerIndex() {
	return winnerIndex;
}

//**********************
// GAMEPLAY FUNCTIONS. *
//**********************

// Game Setup: Prompt for number of players, input names.
void Game::Setup() {
	// Initialize game variables, set die sides, clear player list.
	dice.SetDieSides(6);
	dice.SetTotalDice(6);
	winnerIndex = -1;
	numPlayers = 0;
	player.clear();
	// Prompt for # of players.
	cout << "How many Players?" << endl;
	numPlayers = ValidNumberInput();
	player.resize(numPlayers);
	// Prompt for player names.
	for (unsigned int i = 0; i < numPlayers; i++) {
		string inName = "";
		cout << "Player #" << (i + 1) << ", what is your name?" << endl;
		cin >> inName;
		player[i].SetName(inName);
	}
}

// Begin gameplay.
void Game::BeginPlay() {
	cout << endl << endl
		<< "Welcome to your New game of Farkle!" << endl
	<< "Just a few questions to get setup and playing!" << endl << endl;
	// Set up game.
	Setup();
	cout << endl << endl
		 << "Thank you! Enjoy your Game and Good Luck!" << endl
		 << "-------------------------------------------------------" << endl << endl;

	// Play rounds until someone claims winner.
	// Round complets for remaining players following winner.
	do {
		PlayRound();
		// Run Countdown, break loop if reaches 0. (infinite loop prevention)
		if (!Countdown()) {
			break;
		}
	} while (winnerIndex == -1);
	
	//Final round for remaining players who have not had a final turn.
	FinalRound();

	//Present end game results
	EndGame();
}

// Start a round of game.
void Game::PlayRound() {
	static unsigned int roundCount = 1;
	// Play a round for each player in game.
	for (unsigned int i = 0; i < numPlayers; i++) {
		PlayTurn(i);
	}
	cout << endl
		 << "-------------------------------------------------------" << endl
		 << "-------------------------------------------------------" << endl
		<< "End of Round #" << roundCount << " - Here are the standings:" << endl;

	for (unsigned int j = 0; j < numPlayers; j++) {
		cout << player[j].GetName() << ": " << player[j].GetScore() << "/10,000" << endl;
	}
	cout << "-------------------------------------------------------" << endl
		<< "-------------------------------------------------------" << endl << endl;
	roundCount++;
}

// Start a single player turn.
void Game::PlayTurn(const unsigned int& p) {
	bool rollAgain = true;
	unsigned int diceInCup = dice.GetTotalDice();
	unsigned int diceInHand = 0;
	player[p].ResetRoundScore();
	cout << "Player #" << (p + 1) << " : " << player[p].GetName() << " - YOUR TURN!" << endl
		<< "Current score: " << player[p].GetScore() << "/10,000" << endl << endl;
	if (winnerIndex > -1) {
		cout << "This is your last TURN! " << endl 
			<< player[winnerIndex].GetName() << " has declared a WIN with " << player[winnerIndex].GetScore() << " points!" << endl
			<< "Try to beat their score to steal the win!" << endl
			<< "........................................................";
	}

	// Play round while no farkles rolled, and player continues rolling.
	do {
		// Create vector to store numbers rolled for each die.
		vector<int> hand;
		// Roll Dice while there are dice in cup and player continues.
		do {
			// Roll each die and store to hand.
			hand.resize(diceInCup);
			for (unsigned int i = 0; i < diceInCup; i++) {
				hand[i] = dice.Roll();
			}
			// Show rolled dice to player.
			DrawDice(diceInCup, hand);

			// Check for a FARKLE. End turn if no point dice rolled.
			if (FarkleCheck(hand)) {
				cout << "Oh no! you got a FARKLE!" << endl
					 << "Turn is over. No points awarded! Sorry!" << endl << endl;
				player[p].ResetRoundScore();
				rollAgain = false;
			}
			// If no farkle, ask player to hold dice, score held dice, add to round score.
			else {
				vector<int> handHold = HoldDice(hand, diceInCup, diceInHand);

				int tempScore = ScoreDice(handHold);
				cout << "Adding " << tempScore << " points to your round score." << endl << endl;
				if (tempScore > 0) {
					player[p].AddRoundScore(tempScore);
				}
			}
			//Declare winner.
			bool pressLuck = false;
			if ((player[p].GetScore() + player[p].GetRoundScore()) >= 10000 && winnerIndex == -1) {
				cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl <<
					"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl <<
					"Congratulations! You have enough points to WIN (>10000)!" << endl <<
					"Total points: " << player[p].GetScore() << endl <<
					"Current Round points : " << player[p].GetRoundScore() << endl <<
					endl <<
					"Quit now with " << (player[p].GetScore() + player[p].GetRoundScore()) <<
					" points, or keep going for more!" << endl <<
					"Each Player will have one more chance to beat your score!" << endl <<
					"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl <<
					"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
				pressLuck = ContinueTurn();
				if (!pressLuck) {
					cout << endl << "Declaring yourself the WINNER!" << endl << endl;
					winnerIndex = p;
					rollAgain = false;
				}
			}  
			// If dice remain in cup, ask to roll remaining dice again.
			else if (diceInCup != 0 && rollAgain) {
				rollAgain = ContinueTurn();
			}
		} while (diceInCup != 0 && rollAgain);

		// If player runs out of dice in cup, must roll all dice again.
		if (diceInCup == 0) {
			cout << "All dice held! Must roll all dice again." << endl;
			diceInCup = dice.GetTotalDice();
			diceInHand = 0;
		}
	} while (rollAgain);

	player[p].AddRoundToTotal();

	cout << "*******************************************************" << endl
		 << "ROUND SCORE for player #" << (p + 1) << " - " << player[p].GetName() << ": " << player[p].GetRoundScore() << " points!" << endl
		 << "Total score: " << player[p].GetScore() << "/10,000" << endl
		 << "*******************************************************" << endl;
	if (player[p].GetRoundScore() == 0) {
		cout << "You didn't get any Points! Better luck next round!" << endl 
			 << "*******************************************************" << endl << endl;
	}
	

}

// Start final round of game.
void Game::FinalRound() {
	// If a winner is declared, allow players that had already played
	// that round to have a final turn, up to the winning player.
	if (winnerIndex != -1) {
		for (int i = 0; i < winnerIndex; i++) {
			PlayTurn(i);
		}
	}
}

// Start Hold Dice. 
const vector<int> Game::HoldDice(const vector<int>& h, unsigned int& diceInCup, unsigned int& diceInHand) {
	vector<int> heldDice;
	vector<int> playerHand = h;
	string playerChoice = "";

	// Prompt for player to hold until valid number input recieved.
	cout << "Please enter the ID number(s) of the dice " << endl 
		 << "you wish to hold (1-6) without spaces." << endl
		 << "(ex. \"123\" holds dice #1 #2 and #3): " << endl;
	
	bool pass = false;
	do {
		cin >> playerChoice;
		if (NumCheck(playerChoice)) {
			if (NumsInRange(playerChoice, diceInCup)) {
				cout << endl;
				int qty = playerChoice.length();
				heldDice.resize(qty);

				for (unsigned int i = 0; i < qty; i++) {
					if (playerChoice[i] == '1') {
						heldDice[i] = 1;
					} 
					else if (playerChoice[i] == '2') {
						heldDice[i] = 2;
					}
					else if (playerChoice[i] == '3') {
						heldDice[i] = 3;
					}
					else if (playerChoice[i] == '4') {
						heldDice[i] = 4;
					}
					else if (playerChoice[i] == '5') {
						heldDice[i] = 5;
					}
					else if (playerChoice[i] == '6') {
						heldDice[i] = 6;
					}
				}

				for (unsigned int j = 0; j < qty; j++) {
					heldDice[j] = playerHand[heldDice[j] - 1];
				}

				if (ValidHold(heldDice)) {
					diceInCup -= qty;
					diceInHand += qty;
					pass = true;
				}
				else {
					cout << "Invalid die selection. Please try again!" << endl
						<< "Be sure to only select Points-Dice" << endl
						<< "(1,5, or Three-of-a-Kind)" << endl << endl;

					DrawDice(playerHand.size(), playerHand);

					cout << "Please enter the ID number(s) of the dice " << endl
						 << "you wish to hold (1-6) without spaces:" << endl << endl;
					
				}
			}
		}
	} while (!pass);	
	// Return a vector of heldDice to be scored.
	return heldDice;
}

// Score held dice.
const int Game::ScoreDice(const vector<int>& h) {

	bool farkle = false;
	unsigned int ones = 0;
	unsigned int twos = 0;
	unsigned int threes = 0;
	unsigned int fours = 0;
	unsigned int fives = 0;
	unsigned int sixes = 0;

	unsigned int holdScore = 0;

	for (unsigned int i = 0; i < h.size(); i++) {
		// count face values of dice.
		if (h[i] == 1) {
			ones++;
		}
		else if (h[i] == 2) {
			twos++;
		}
		else if (h[i] == 3) {
			threes++;
		}
		else if (h[i] == 4) {
			fours++;
		}
		else if (h[i] == 5) {
			fives++;
		}
		else if (h[i] == 6) {
			sixes++;
		}
	}
	

	// Add up points
	do {
		if (ones >= 3) {
			cout << "Three ONES dies: 1000 points!" << endl;
			holdScore += 1000;
			ones -= 3;
		}
		else if (ones > 0 && ones <= 2) {
			cout << ones << "x ONES die(s): " << (ones * 100) << " points!" << endl;
			holdScore += (ones *100);
			ones = 0;
		}
	} while (ones > 0);

	do {
		if (twos >= 3) {
			cout << "Three TWOS dies: 200 points!" << endl;
			holdScore += 200;
			twos -= 3;
		}
	} while (twos > 0);

	do {
		if (threes >= 3) {
			cout << "Three THREES dies: 300 points!" << endl;
			holdScore += 300;
			threes -= 3;
		}
	} while (threes > 0);

	do {
		if (fours >= 3) {
			cout << "Three FOURS dies: 400 points!" << endl;
			holdScore += 400;
			fours -= 3;
		}
	} while (fours > 0);

	do {
		if (fives >= 3) {
			cout << "Three FIVES dies: 500 points!" << endl;
			holdScore += 500;
			fives -= 3;
		}
		else if (fives > 0 && fives <= 2) {
			cout << fives << "x FIVES die(s): " << (fives * 50) << " points!" << endl;
			holdScore += (fives * 50);
			fives = 0;
		}
	} while (fives > 0);

	do {
		if (sixes >= 3) {
			cout << "Three sixes die: 600 points!" << endl;
			holdScore += 600;
			sixes -= 3;
		}
	} while (fours > 0);

	return holdScore;
}

void Game::EndGame() {
	int winnerScore = 0;
	for (int i = 0; i < numPlayers; i++) {
		if (player[i].GetScore() >= winnerScore) {
			winnerScore = player[i].GetScore();
			winnerIndex = i;
		}
	}
	cout << endl << endl <<
		"#######################################################" << endl <<
		"#######################################################" << endl <<
		"Player #" << (winnerIndex + 1) << " - " << player[winnerIndex].GetName() << " WINS!" << endl <<
		"#######################################################" << endl <<
		"#######################################################" << endl;
}

//********************
// HELPER FUNCTIONS. *
//********************

// Prompt player to continue turn.
const bool Game::ContinueTurn() {
	cout << "Would you like to roll again? (y/n)" << endl;
	return ValidYesNoInput();
}

// Counts down from maxRounds value.
bool Game::Countdown() {
	maxRounds--;
	if (maxRounds <= 0) {
		return false;
	}
	return true;
}

// Check for a FARKLE on a roll (no point dice).
const bool Game::FarkleCheck(const vector<int>& h) {

	bool farkle = false;
	unsigned int ones = 0;
	unsigned int twos = 0;
	unsigned int threes = 0;
	unsigned int fours = 0;
	unsigned int fives = 0;
	unsigned int sixes = 0;

	for (unsigned int i = 0; i < h.size(); i++) {
		// Count face values of dice.
		switch (h[i]) {
		case 1:
			ones++;
			break;
		case 2:
			twos++;
			break;
		case 3:
			threes++;
			break;
		case 4:
			fours++;
			break;
		case 5:
			fives++;
			break;
		case 6:
			sixes++;
			break;
		default:
			cout << "OOPS! invalid Number!" << endl;
			break;
		}
	}
	// if farkle condition
	if ((ones < 1) && (twos < 3) && (threes < 3) && (fours < 3) && (fives < 1) && (sixes < 3)) {
		farkle = true;
	}
	else farkle = false;

	return farkle;
}

// Check if a string is all numeric.
const bool Game::NumCheck(const string& s) {
	bool pass = false;
	// Check each character in input string for digit.
	for (unsigned int i = 0; i < s.length(); i++) {
		if (!isdigit(s[i])) {
			cout << "Not a valid number, Please Try again." << endl;
			// If non-digit return false.
			pass = false;
			break;
		}
		else pass = true;
	}
	return pass;
}

// Check if a string is all numbers 1-6.
const bool Game::NumsInRange(const string& s, const int& n) {
	char number = ' ';
	if (n == 1) {
		number = '1';
	}
	else if ( n == 2) {
		number = '2';
	}
	else if (n == 3) {
		number = '3';
	}
	else if (n == 4) {
		number = '4';
	}
	else if (n == 5) {
		number = '5';
	}
	else if (n == 6) {
		number = '6';
	}
	bool pass = false;
	// Check each character in input string for digit.
	for (unsigned int i = 0; i < s.length(); i++) {
		if (s[i] != '1' && s[i] != '2' && s[i] != '3' && s[i] != '4' && s[i] != '5' && s[i] != '6') {
			cout << "Enter only numbers 1-6." << endl;
			pass = false;
			break;
		}
		else if (s[i] > number) {
			cout << "Only Dice 1-" << n << " remain. Try again." << endl;

		}
		else pass = true;
	}
	return pass;
}

// Validated user input for integer.
const int Game::ValidNumberInput() {
	string playerInput = "";
	do {
		cin >> playerInput;
	} while (!NumCheck(playerInput));
	return stoi(playerInput);
}

// Validated user input for yes/no question.
const bool Game::ValidYesNoInput() {
	string playerInput = "";
	bool result;
	bool done = false;
	do {
		cin >> playerInput;
		if (playerInput == "y" || playerInput == "Y") {
			result =  true;
			done = true;
		}
		else if (playerInput == "n" || playerInput == "N") {
			result = false;
			done = true;
		}
		else {
			cout << "Invalid input! Please enter (y/n) only." << endl;
		}
	} while (!done);
	return result;
}

// Validate if hold is holding only points dice.
const bool Game::ValidHold(const vector<int>& h) {
	unsigned int inputSize = h.size();
	bool valid = false;
	unsigned int ones = 0;
	unsigned int twos = 0;
	unsigned int threes = 0;
	unsigned int fours = 0;
	unsigned int fives = 0;
	unsigned int sixes = 0;

	for (unsigned int i = 0; i < inputSize; i++) {
		// count face values of dice.
		if (h[i] == 1) {
			ones++;
		}
		if (h[i] == 2) {
			twos++;
		}
		if (h[i] == 3) {
			threes++;
		}
		if (h[i] == 4) {
			fours++;
		}
		if (h[i] == 5) {
			fives++;
		}
		if (h[i] == 6) {
			sixes++;
		}
	}
	
	int badnum = 0;
	if (twos != 0 && twos != 3 && twos != 6) {
		badnum++;
	}
	if (threes != 0 && threes != 3 && threes != 6) {
		badnum++;
	}
	if (fours != 0 && fours != 3 && fours != 6) {
		badnum++;
	}
	if (sixes != 0 && sixes != 3 && sixes != 6) {
		badnum++;
	}
	if (badnum == 0) {
		valid = true;
	}
	
	return valid;
}

// Dice drawing function. Takes in dice quantity, and vector of rolled values.
void Game::DrawDice(const unsigned int& q, const vector<int>& h) {
	// Validate dice quantity and vector size match. (all dice have values)
	if (q == h.size()) {
		// Draw Dice #s.
		for (unsigned int d = 0; d < q; d++) {
			cout << " #" << (d + 1) << "     ";
		}
		cout << endl;
		// Draw top of dice.
		for (unsigned int d = 0; d < q; d++) {
			cout << "-----   ";
		}
		cout << endl;
		// Draw faces of dice.
		for (unsigned int d = 0; d < q; d++) {
			cout << "| " << h[d] << " |   ";
		}
		cout << endl;
		// Draw bottom of dice.
		for (unsigned int d = 0; d < q; d++) {
			cout << "-----   ";
		}
		cout << endl << endl;
	}
	else {
		cout << "Quantity/Value mismatch";
	}
}

// FOR TESTING: Sets up 3 Players, modifies variables and outputs changes.
//Takes in string for caseID, to differentiate runs in console log.
void Game::PlayerTestCases(const string& caseID) {

	// Test Roll() function from Dice.h
	cout << "Test Case: " << caseID << endl;
	for (int i = 0; i < 10; i++) {
		cout << "Roll #" << (i + 1) << ": " << dice.Roll() << endl;
	}
	cout << endl;

	Player player1;
	Player player2("Bread");
	Player player3("Toast", 1000);

	cout << "Player1 created! Name: " << player1.GetName() << ", Score: " << player1.GetScore() << endl;
	cout << "Player2 created! Name: " << player2.GetName() << ", Score: " << player2.GetScore() << endl;
	cout << "Player3 created! Name: " << player3.GetName() << ", Score: " << player3.GetScore() << endl << endl;

	player1.SetName("Ice");
	player2.SetName("Cream");
	player3.SetName("Sandwich");


	cout << "Player1 name changed! Name: " << player1.GetName() << ", Score: " << player1.GetScore() << endl;
	cout << "Player2 name changed! Name: " << player2.GetName() << ", Score: " << player2.GetScore() << endl;
	cout << "Player3 name changed! Name: " << player3.GetName() << ", Score: " << player3.GetScore() << endl << endl;

	player1.SetScore(10);
	player2.SetScore(20);
	player3.SetScore(30);

	cout << "Player1 score changed! Name: " << player1.GetName() << ", Score: " << player1.GetScore() << endl;
	cout << "Player2 score changed! Name: " << player2.GetName() << ", Score: " << player2.GetScore() << endl;
	cout << "Player3 score changed! Name: " << player3.GetName() << ", Score: " << player3.GetScore() << endl << endl;

	player1.AddScore(100);
	player2.AddScore(200);
	player3.AddScore(300);

	cout << "Player1 score added! Name: " << player1.GetName() << ", Score: " << player1.GetScore() << endl;
	cout << "Player2 score added! Name: " << player2.GetName() << ", Score: " << player2.GetScore() << endl;
	cout << "Player3 score added! Name: " << player3.GetName() << ", Score: " << player3.GetScore() << endl << endl;

	player1.SubScore(11);
	player2.SubScore(22);
	player3.SubScore(33);

	cout << "Player1 score subtracted! Name: " << player1.GetName() << ", Score: " << player1.GetScore() << endl;
	cout << "Player2 score subtracted! Name: " << player2.GetName() << ", Score: " << player2.GetScore() << endl;
	cout << "Player3 score subtracted! Name: " << player3.GetName() << ", Score: " << player3.GetScore() << endl << endl;
}