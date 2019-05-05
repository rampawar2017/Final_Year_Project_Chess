#include "UserPlayer.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "Chessboard.h"

using namespace std;

//Helper functions to verify user's input.
//>>>
bool UserPlayer::IsXCorrect(char c)
{
	return c >= 'a' && c <= 'h';
}

bool UserPlayer::IsYCorrect(char c)
{
	return c >= '1' && c <= '8';
}
//<<<

//Make a movement.
Movement UserPlayer::DoMovement(Chessboard& chessboard)
{
	auto movements = chessboard.GetCorrectMovements();

	//Repeat until correct input.
	do {
		cout << "Enter your move (e.g e2-e4): ";
		string input;
		getline(cin, input);
		transform(input.begin(), input.end(), input.begin(), tolower);

		if (input.size() == 5 && input[2] == '-')
		{
			const auto srcXSymbol = input[0];
			const auto srcYSymbol = input[1];
			const auto destXSymbol = input[3];
			const auto destYSymbol = input[4];

			if (IsXCorrect(srcXSymbol) && IsYCorrect(srcYSymbol) && IsXCorrect(destXSymbol) && IsYCorrect(destYSymbol))
			{
				auto srcCoord = Coord(srcXSymbol - 'a', srcYSymbol - '1');
				auto destCoord = Coord(destXSymbol - 'a', destYSymbol - '1');
				const auto movement = Movement(srcCoord, destCoord);

				//Check if input is in the list of correct movements.
				if (find(movements.begin(), movements.end(), movement) != movements.end())
					//Input is correct. Return it.
					return movement;
			}
		}

		cout << "Wrong input. Please try again using format 'e2-e4'." << endl;
	} while (true);
}
