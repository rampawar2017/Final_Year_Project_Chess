#include "RandomPlayer.h"
#include "Chessboard.h"

Movement RandomPlayer::DoMovement(Chessboard& chessboard)
{
	auto movements = chessboard.GetCorrectMovements();
	return movements[rand() % movements.size()];
}
