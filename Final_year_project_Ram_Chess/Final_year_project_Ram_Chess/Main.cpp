#include <iostream>
#include <windows.h>
#include "Chessboard.h"
#include <ctime>
#include "Ai.h"
#include <string>
#include "RandomPlayer.h"
#include "AiPlayer.h"
#include "UserPlayer.h"

using namespace std;

int main()
{
	//Initialize random generator.
	srand(static_cast<unsigned>(time(nullptr)));

	//Initialize chessboard.
	Chessboard chessboard;

	//Specify player type for each of the players.
	/*Player* whitePlayer = &RandomPlayer(); 
	Player* blackPlayer = &UserPlayer();*/
	Player* whitePlayer = &UserPlayer();
	Player* blackPlayer = &AiPlayer();

	//Play the game.
	const int maxTurnCount = 100;
	for (int i = 0; i <= maxTurnCount; ++i)
	{
		const string playerName = chessboard.is_black() ? "Black" : "White";
		const string enemyName = chessboard.is_black() ? "White" : "Black";

		chessboard.Show();
		chessboard.UpdateState(true);

		//Process specific states of chessboard (checkmate, stalemate, mate).
		//>>>
		const bool isMate = chessboard.is_mate();
		const bool isStalemate = chessboard.is_stalemate();

		if (isMate || isStalemate)
		{
			if (isMate) cout << "Checkmate. " << enemyName << " won." << endl;
			else if (isStalemate) cout << "Stalemate." << endl;
			break;
		}

		if (chessboard.is_check())
		{
			cout << "Check." << endl;
		}

		if (i >= maxTurnCount)
		{
			cout << "Maximum turn count achieved. Draw." << endl;
			break;
		}
		//<<<

		cout << "Move #" << i + 1 << ". Player: " << playerName << "." << endl;

		//Choose a movement.
		auto movements = chessboard.GetCorrectMovements();

		if (movements.empty())
		{
			cout << "Something is wrong. There are no valid movements." << endl;
			break;
		}
		else
		{
			auto player = chessboard.is_black() ? blackPlayer : whitePlayer;
			auto movement = player->DoMovement(chessboard);
			cout << chessboard.CellToSymbol(chessboard.cells[movement.srcCoord.x][movement.srcCoord.y]) << " "
				<< char('a' + movement.srcCoord.x) << movement.srcCoord.y + 1 << "-"
				<< char('a' + movement.destCoord.x) << movement.destCoord.y + 1 << endl << endl;

			chessboard.Move(movement);
		}
	}

	system("pause");
	return 0;
}