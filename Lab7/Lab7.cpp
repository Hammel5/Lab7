/*
Dalton Hammel
C++	Fall 2021
Due: 12/8/2021
Lab Name: Lab 7 Battleship

Description: For this lab you will make a simple battleship game. In this lab, you will need four
10 x 10 grids. The program will begin with setup. The program will need to randomly place the 
computers 5 ships, making sure the ships do not overlap or go off the board. 

For placing the user’s ships, the user should be able to choose a xy coordinate and a
direction to place the ship. Again, the program should check to make sure the ship
placement is valid. The grid and coordinate system should be setup with x in the
horizontal direction and y in the vertical direction.

For each turn of the game, the computer should randomly choose locations to fire and
the results should be shared and tracked on the screen. An example output on the
computer’s turn would show the computer firing and giving the coordinates, display the
result of “HIT!” and display an updated grid with ships and the locations of all of the
computer’s hits and misses. When displaying the grid, you must have the grid coordinates 
displayed on the boarder for reference.

You should give the user the option during each turn to fire, view user’s ship grid, or
surrender. When the fire function is called, display the user’s guess grid for reference.
The program must ensure the computer and user only fire at valid locations that have
not been checked. When one of the fleets is destroyed, the program should recognized
it, communicate the results to the users, given an update on wins and losses, and give
them the option to play again or quit. Make sure to use good programing practices for
this lab. I expect that you use functions for each action the program takes, your code is
well documented with pre/post conditions, and your code is reasonably efficient.
*/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cmath>

#define pShips 0
#define pShots 1
#define cShips 2
#define cShots 3

using namespace std;

void Introduction();
void Transition();
void printBoard(char board[10][10]);
void playerSetup(char gameBoard[10][10], struct shipType ship[5]);
void computerSetup(char gameBoard[10][10], struct shipType ship[5]);
bool placeShips(int x, int y, int length, char direction, char shipChar, char gameBoard[10][10]);
void playerFire(char playerShots[10][10], char computerFleet[10][10]);
bool computerFire(char computerShots[10][10], char playerFleet[10][10]);
bool checkWin(char gameBoard[10][10]);
bool playAgain(bool win, int wins, int losses);
void resetGame(char gameBoard[4][10][10]); // 1: player ships, 2: player shots, 3: computer ships, 4: computer shots 

struct shipType
{
	string name;
	int length;
};

int main()
{
	char gameBoard[4][10][10];

	shipType ship[5];	// struct of ship names and lengths

	ship[0].name = "CARRIER";
	ship[0].length = 5;

	ship[1].name = "BATTLESHIP";
	ship[1].length = 4;

	ship[2].name = "CRUISER";
	ship[2].length = 3;

	ship[3].name = "SUBMARINE";
	ship[3].length = 3;

	ship[4].name = "DESTROYER";
	ship[4].length = 2;

	bool playerWin;
	bool computerWin;
	int wins = 0;
	int losses = 0;

	do 
	{
		playerWin = false;
		computerWin = false;

		Introduction();
		resetGame(gameBoard);
		playerSetup(gameBoard[pShips], ship);
		computerSetup(gameBoard[cShips], ship);
		Transition();
		do
		{
			playerFire(gameBoard[pShots], gameBoard[cShips]);
			if (checkWin(gameBoard[cShips])) // returns true if player won
			{
				playerWin = true;
				wins++;
			}
			else
			{
				if (computerFire(gameBoard[cShots], gameBoard[pShips]) || checkWin(gameBoard[pShips]))
				{
					computerWin = true;
					losses++;
				}
			}
		} 
		while (!playerWin && !computerWin); // loops until computer or player win
	} 
	while (playAgain(playerWin, wins, losses));

	return 0;
}

// Precondition :	None
// Postcondition :	The intro page is displayed, and the ship placement instructions are displayed after
void Introduction()
{
	cout << "\t  BATTLESHIP \n\n";
	cout << "\t  This is a strategy game where you will test your skills against the computer.\n";
	cout << "\t  First you will place a fleet of 5 ships on the gameboard \n";
	cout << "\t  You will then alternate turns firing shots at the other player's ships.\n";
	cout << "\t  The objective of the game: destroy all of the opponent's ships before they sink yours.\n\n";

	cout << flush;
	system("PAUSE");	
	system("CLS");		

	cout << "\t\t   SHIP PLACEMENT \n\n";
	cout << "\t\t1. Enter the coordinate points of each ship placement. (1-10)\n";
	cout << "\t\t2. Enter the direction to place the ship. (U-D-L-R)\n\n";

	cout << flush;
	system("PAUSE");	
	system("CLS");		
}

// Precondition :	using namespace std, the iostream library is included
// Postcondition :	the transition page and firing instructions are printed, then cleared after keystroke
void Transition()
{
	cout << "\t\t   ATTACK \n\n";
	cout << "\t\t1. Enter the target coordinate to fire upon. (1-10)\n";
	cout << "\t\t2. View the computers shot upon your fleet.\n\n";

	cout << flush;
	system("PAUSE");
	system("CLS");
}

void resetGame(char gameBoard[4][10][10])
{
	for (int board = 0; board < 4; board++)
	{
		for (int column = 0; column < 10; column++)
		{
			for (int row = 0; row < 10; row++)
			{
				gameBoard[board][row][column] = '~';
			}
		}
	}
}

// Precondition :	using namespace std, the iostream library is included, one game board (2 diminsional character array 10*10) is passed by reference
// Postcondition :	the game board is printed to the console accompanied by x and y axis labels
void printBoard(char gameBoard[10][10])
{
	cout << "   1 2 3 4 5 6 7 8 9 10\n"; // x axis label
	for (int column = 0; column < 10; column++)
	{
		cout << setw(2) << column + 1; // y axis laebel
		for (int row = 0; row < 10; row++)
		{
			cout << " " << gameBoard[row][column];
		}
		cout << endl;
	}
}


bool placeShips(int row, int column, int length, char direction, char shipChar, char gameBoard[10][10])
{
	switch (direction)
	{
	case 'U':
	case 'u':
		for (int i = column; i > column - length; i--)
			if (gameBoard[row][i] == '#' || i < 0) return false; // checks upward ship placement, returns false if already occupied
		for (int i = column; i > column - length; i--)
			gameBoard[row][i] = shipChar; // sets gameboard to shipChar at ship placement location
		break;
	case 'D':
	case 'd':
		for (int i = column; i < column + length; i++)
			if (gameBoard[row][i] == '#' || i > 9) return false; // checks downward ship placement, returns false if already occupied
		for (int i = column; i < column + length; i++)
			gameBoard[row][i] = shipChar;	// sets gameboard to shipChar at ship placement location
		break;
	case 'R':
	case 'r':
		for (int i = row; i < row + length; i++)
			if (gameBoard[i][column] == '#' || i > 9) return false; // checks right ship placement, returns false if already occupied
		for (int i = row; i < row + length; i++)
			gameBoard[i][column] = shipChar;	// sets gameboard to shipChar at ship placement location
		break;
	case 'L':
	case 'l':
		for (int i = row; i > row - length; i--)
			if (gameBoard[i][column] == '#' || i < 0) return false; // checks left ship placement, returns false if already occupied
		for (int i = row; i > row - length; i--)
			gameBoard[i][column] = shipChar;	// sets gameboard to shipChar at ship placement location
		break;
	}
	return true;
}

void playerSetup(char gameBoard[10][10], struct shipType ship[5])
{
	char gameBoardTemp[10][10];	// temporary game board to print placement crosshair and direction options
	int xCoord;
	int yCoord;
	char direction;
	bool inputError = false;	// true if unexpected input type/character from the player
	bool spaceOccupied = false;	// true if the placement location is already occupied by a ship or is off the board
	string playerInput;

	for (int i = 0; i < 5; i++) // increments through 5 ship types
	{
		memcpy(gameBoardTemp, gameBoard, sizeof(gameBoardTemp));	// actual player fleet board is copied to gameBoardTemp
		for (int j = 0; j < 3; j++)	// increments through three input cases ( x, y, direction )
		{
			printBoard(gameBoardTemp);
			cout << endl << ship[i].name << endl;			// prints ship name
			cout << "Length: " << ship[i].length << endl;	// prints ship length
			switch (j)
			{
			case 0:	// x coordinate input
				if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";	// error message printed if last input was out of range
				if (spaceOccupied) cout << "\t[SPACE ALREADY OCCUPIED]\r";				// error message printed if x coodinate collumn is occupied
				cout << "X: ";	// prompts for x coordinate input

				if (!(cin >> xCoord) || xCoord < 1 || xCoord > 10)	// check if cin fails, x < 1, or x > 10
				{
					cin.clear();				// clears input stream to remove invalid input
					cin.ignore(999, '\n');		// ignores the entire line of input (up to 999 characters)
					inputError = true;
				}
				else
				{
					inputError = false;
					spaceOccupied = true;
					for (int column = 0; column < 10; column++) // checks evey possible placement loaction and direction in x coordinate selection
					{
						if (placeShips(xCoord - 1, column, ship[i].length, 'U', '~', gameBoard) ||
							placeShips(xCoord - 1, column, ship[i].length, 'D', '~', gameBoard) ||
							placeShips(xCoord - 1, column, ship[i].length, 'L', '~', gameBoard) ||
							placeShips(xCoord - 1, column, ship[i].length, 'R', '~', gameBoard))
						{
							spaceOccupied = false;
						}
					}
				}

				if (inputError || spaceOccupied) j = j - 1;	// repeats y input case
				else
				{
					for (int column = 0; column < 10; column++) if (gameBoardTemp[xCoord - 1][column] == '~') gameBoardTemp[xCoord - 1][column] = '|'; // prints vertical crosshair on temp board
				}
				break;
			case 1: // y coordinate input
				cout << "X: " << xCoord << endl;
				if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";	// error message printed if last input was out of range
				if (spaceOccupied) cout << "\t[SPACE ALREADY OCCUPIED]\r";				// error message printed if placement location is occupied
				cout << "Y: ";	// prompts for y coordinate input

				if (!(cin >> yCoord) || yCoord < 1 || yCoord > 10) // checks if cin fails, y < 1, or y > 10
				{
					cin.clear();				// clears input stream to remove invalid input
					cin.ignore(999, '\n');		// ignores the entire line of input (up to 999 characters)
					inputError = true;
				}
				else
				{
					inputError = false;
					if (placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'U', '~', gameBoard) ||
						placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'D', '~', gameBoard) ||
						placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'L', '~', gameBoard) ||
						placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'R', '~', gameBoard))		// checks if placement is possible in any direction at coordinates
					{
						spaceOccupied = false;
					}
					else spaceOccupied = true;
				}

				if (inputError || spaceOccupied) j = j - 1;	// repeeats y input case
				else
				{
					for (int x = 0; x < 10; x++) if (gameBoardTemp[x][yCoord - 1] == '~') gameBoardTemp[x][yCoord - 1] = '-'; // prints horizontal crosshair on temp board
					placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'U', 'U', gameBoardTemp);
					placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'D', 'D', gameBoardTemp);
					placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'L', 'L', gameBoardTemp);
					placeShips(xCoord - 1, yCoord - 1, ship[i].length, 'R', 'R', gameBoardTemp); // prints UDLR characters in all possible placement directions on the croshair
					gameBoardTemp[xCoord - 1][yCoord - 1] = 'X'; // prints X at center of crosshair on temp board
				}
				break;
			case 2:	// direction input
				cout << "X: " << xCoord << endl;
				cout << "Y: " << yCoord << endl;
				if (spaceOccupied) cout << "\t\t[SPACE ALREADY OCCUPIED]\r";				// error message printed if placement location is occupied
				if (inputError) cout << "\t\t[INVALID DIRECTION SELECTION: U-D-L-R ]\r";	// error message printed if last input character was invalid
				cout << "Direction: ";	// prompts user for direction input

				cin >> playerInput;		// temporarily stores user input in a string
				direction = playerInput.at(0);	// sets direction to first character of string ( not a perfect solution, but removes input error problems and accept yes/no full word answers)

				if (!(direction == 'U' || direction == 'u' ||
					direction == 'D' || direction == 'd' ||
					direction == 'L' || direction == 'l' ||
					direction == 'R' || direction == 'r')) // checks if direction character is valid
				{
					inputError = true;
				}
				else
				{
					inputError = false;
					if (!(placeShips(xCoord - 1, yCoord - 1, ship[i].length, direction, '#', gameBoard))) spaceOccupied = true; // checks if valid placement location, and places ship on player fleet board
					else spaceOccupied = false;
				}

				if (inputError || spaceOccupied) j = j - 1; // repeats direction selection case
				break;
			}
			cout << flush;
			system("CLS"); // clears screen after each input
		}
	}
}

void computerSetup(char gameBoard[10][10], struct shipType ship[5])
{
	srand(time(NULL)); // sets random number seed to current time

	for (int i = 0; i < 5; i++) // increments through all 5 ship types
	{
		int x = rand() % 9; // generates random coordinates and direction selection
		int y = rand() % 9;
		char direction;
		switch (rand() % 3)
		{
		case 0: direction = 'U';
			break;
		case 1: direction = 'D';
			break;
		case 2: direction = 'L';
			break;
		case 3: direction = 'R';
			break;
		}

		if (!(placeShips(x, y, ship[i].length, direction, '#', gameBoard))) i = i - 1; // repeats ship type if the space is already occupied
	}
}

void playerFire(char playerShots[10][10], char computerFleet[10][10])
{
	int x;
	int y;
	bool inputError = false;		// true if unexpected input type/character from the player
	bool spaceOccupied = false;		// true if the placement location is already occupied by a ship or is off the board
	char playerShotsTemp[10][10];	// temporary game board to print crosshair

	memcpy(playerShotsTemp, playerShots, sizeof(playerShotsTemp)); // copies player shots to temporary board
	for (int i = 0; i < 3; i++) // increments through three input cases ( x, y, direction )
	{
		printBoard(playerShotsTemp);	// prints temp shots board
		cout << "\nTARGET COORDINATES\n";
		switch (i)
		{
		case 0: // x coordinate input
			if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r"; // error message printed if last input was out of range
			if (spaceOccupied) cout << "\t[SPACE ALREADY FIRED UPON]\r";		 // error message printed if shot location is occupied
			cout << "X: "; // prompts player for x input

			if (!(cin >> x) || x > 10 || x < 0) // check if cin fails, x < 1, or x > 10
			{
				cin.clear();				// clears input stream to remove invalid input
				cin.ignore(999, '\n');		// ignores the entire line of input (up to 999 characters)
				inputError = true;
			}
			else
			{
				inputError = false;
				spaceOccupied = true;
				for (int y1 = 0; y1 < 10; y1++) if (playerShots[x - 1][y1] == '~') spaceOccupied = false; // checks if entire column has been fired upon
			}

			if (inputError || spaceOccupied) i = i - 1; // repeats x case
			else for (int y1 = 0; y1 < 10; y1++) if (playerShotsTemp[x - 1][y1] == '~') playerShotsTemp[x - 1][y1] = '|'; // prints vertical crosshair to tempoaray shots board
			break;
		case 1: // y coordinate input
			cout << "X: " << x << endl;
			if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r"; // error message printed if last input was out of range
			if (spaceOccupied) cout << "\t[SPACE ALREADY FIRED UPON]\r";		 // error message printed if shot location is occupied
			cout << "Y: "; // prompts player for y input

			if (!(cin >> y) || y > 10 || y < 0) // checks if cin fails, y < 1, or y > 10
			{
				cin.clear();				// clears input stream to remove invalid input
				cin.ignore(999, '\n');		// ignores the entire line of input (up to 999 characters)
				inputError = true;
			}
			else
			{
				inputError = false;
				if (playerShots[x - 1][y - 1] != '~') spaceOccupied = true; // checks if space has already been fired upon
				else spaceOccupied = false;
			}
			if (inputError || spaceOccupied) i = i - 1;	// repeats y case
			else
			{
				for (int x1 = 0; x1 < 10; x1++) if (playerShotsTemp[x1][y - 1] == '~') playerShotsTemp[x1][y - 1] = '-'; // prints horizontal crosshair to tempoaray shots board
				if (computerFleet[x - 1][y - 1] == '#') // sets all corresponding boards to hit 'H' or miss 'M'
				{
					playerShotsTemp[x - 1][y - 1] = 'H';
					playerShots[x - 1][y - 1] = 'H';
					computerFleet[x - 1][y - 1] = 'H';
				}
				else
				{
					playerShotsTemp[x - 1][y - 1] = 'M';
					playerShots[x - 1][y - 1] = 'M';
					computerFleet[x - 1][y - 1] = 'M';
				}
			}
			break;
		case 2: // shot result printout
			cout << "X: " << x << endl;
			cout << "Y: " << y << endl << endl;
			if (computerFleet[x - 1][y - 1] == 'H') cout << "HIT!"; // prints resuts HIT/MISS after each
			else cout << "MISS!";

			cout << endl << endl << flush;
			system("PAUSE"); // waits for keystroke to continue
			break;
		}
		cout << flush;
		system("CLS"); // clears screen after each input/printout
	}
}

bool computerFire(char computerShots[10][10], char playerFleet[10][10])
{
	int x;
	int y;
	bool invalidShot;
	do
	{
		x = rand() % 9;	// generates random shot coordinates
		y = rand() % 9;
		if (computerShots[x][y] != '~') invalidShot = true; // checks if valid shot location
		else
		{
			invalidShot = false;
			if (playerFleet[x][y] == '#') // marks boards 'H' if hit
			{
				computerShots[x][y] = 'H';
				playerFleet[x][y] = 'H';
			}
			else
			{
				computerShots[x][y] = 'M'; // marks boards 'M' if not hit
				playerFleet[x][y] = 'M';
			}
		}
	} 
	while (invalidShot);


	string playerInput;
	bool inputError = false;
	do
	{
		printBoard(playerFleet);	// printout of computer's shots on players fleet with hit/miss message
		if (playerFleet[x][y] == 'H') cout << "\nCOMPUTER HIT!\n\n";
		else cout << "\nCOMPUTER MISSED!\n\n";

		if (inputError) cout << "\t\t\t\t\t[Invalid Response: Y/N]\r";
		cout << "Surrender? : ";	// asks player if they want to surrender
		cin >> playerInput;
		switch (playerInput.at(0))	// takes only first character of input stream
		{
		case 'y':
		case 'Y':
			cout << flush;
			system("CLS");
			return true;	// returns true if surrender
		case 'n':
		case 'N':
			cout << flush;
			system("CLS");
			return false;	// returns false if not surrender
		default:
			inputError = true;
		}
	}
	while (inputError);	// repears if not y/n answer
}


bool checkWin(char gameBoard[10][10])
{
	bool win = true;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			if (gameBoard[x][y] == '#') win = false; // check every board position for prensence of any remaining ship character '#'
		}
	}
	return win; // returns true if win, false if no win
}

// Precondition :	using namespace std, the iostream library is included, the player 'win' boolean, and wins and losses integers are passed by value
// Postcondition :	a win or loss screen will be displayed accoriding to the 'win' boolean, along with a counter of total wins and losses. The player should then be asked to
//					play again, if yes, the function should return true. Any error in player input should be handled appropiately. 
bool playAgain(bool win, int wins, int losses)
{
	bool inputError = false;
	bool thanks = false;
	string playerInput;

	do {
		if (win)
		{
			cout << "\t VICTORY \n\n";
			cout << "\t\t\t\t WINS: " << wins << "\t\t LOSSES: " << losses << endl << endl << endl; // outputs win/loss count
		}
		else
		{
			cout << "\t DEFEAT \n\n";
			cout << "\t\t\tWINS: " << wins << "\t\tLOSSES: " << losses << endl << endl << endl;	// outputs win/loss count
		}
		if (inputError) cout << "\t\t\t\t\t[Invalid Response: Y/N]\r";	// error message if invalid input on last input
		if (thanks)	// outputs thank you message if done playing
		{
			cout << "Would you like to play again? : " << playerInput.at(0) << "\tThanks for playing!\n\n\n";
			return false; // returns false, ending program
		}
		cout << "Would you like to play again? : "; // prompts to play again
		cin >> playerInput;
		switch (playerInput.at(0))
		{
		case 'y':
		case 'Y':
			cout << flush;
			system("CLS");
			return true; // returns true if yes
		case 'n':
		case 'N':
			inputError = false;
			thanks = true;
			break;		// loops back to print thank you message if no
		default:
			inputError = true;	// loops back for new input if invalid
		}
		cout << flush;
		system("CLS");	// clears screen for reprint
	} 
	while (inputError || thanks);
}