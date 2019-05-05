#pragma once
#include "Movement.h"
#include "Chessboard.h"

//Artificial intelligence for chess.
class Ai
{
public:
	static int evaluateCell(const Cell cell);
	static int evaluateBoard(Chessboard& chessboard);
	static int AlphaBeta(Chessboard& chessboard, int depth, int alpha, int beta, bool maximizingPlayer);
	static int MiniMax(Chessboard& chessboard, int depth, bool maximizingPlayer);
	static Movement FindBestMovement(Chessboard& chessboard, int depth, bool maximizingPlayer);
};

