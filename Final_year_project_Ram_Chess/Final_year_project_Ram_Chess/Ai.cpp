#include "Ai.h"
#include <algorithm>
#include <iostream>
//connect
//Evaluates cell score.
int Ai::evaluateCell(const Cell cell)
{
	const bool isBlack = Chessboard::IsBlack(cell);

	int value;

	//Set score depending on cell.
	switch (cell)
	{
	case WhitePawn:
	case BlackPawn:
		value = 10;
		break;
	case WhiteKnight:
	case BlackKnight:
		value = 30;
		break;
	case WhiteBishop:
	case BlackBishop:
		value = 30;
		break;
	case WhiteRook:
	case BlackRook:
		value = 50;
		break;
	case WhiteQueen:
	case BlackQueen:
		value = 90;
		break;
	case WhiteKing:
	case BlackKing:
		value = 10000; //very big value for king
		break;
	default:
		value = 0;


		// Should have very big value for king. As it is the goal. Other values should have such or similar proportions.They are just typical weights of pieces. Usually 1, 3, 5 and 8..10 are used. But here I needed some randomness. So I just multiplied the weights by 10 and added small random values below 10. We may try other values or add some other metrics.It will change strategy.
	}

	//Some randomness to have different moves each time.
	value += rand() % 10;

	//Black figures should have negative score.
	return isBlack ? -value : value;
}

//Evalutes entire chessboard score.
int Ai::evaluateBoard(Chessboard& chessboard)
{
	//Sum of cell scores.
	int res = 0;
	for (int x = 0; x < Chessboard::size; x++) {
		for (int y = 0; y < Chessboard::size; y++) {
			res += evaluateCell(chessboard.cells[x][y]);
		}
	}


	//Additional scores can be used for checkmate of mate, but it requires more CPU time.

	//chessboard.UpdateState(true);
	//chessboard.UpdateState(false);

	//if (chessboard.is_check())
	//{
	//	res += 5;// 0000;
	//	//cout << "sdaf" << endl;
	//	//chessboard.Show();
	//	//getchar();
	//}

	//if (chessboard.is_mate())
	//{
	//	res += 100000;
	//	//cout << "Mate detected" << endl;
	//	//chessboard.Show();
	//	//getchar();
	//}

	return res;
}

//Implementation of Alpha-beta prune algorithm
int Ai::AlphaBeta(Chessboard& chessboard, int depth, int alpha, int beta, bool maximizingPlayer)
{
	if (depth == 0) {
		return evaluateBoard(chessboard);
	}

	auto movements = chessboard.GetCorrectMovements();

	if (maximizingPlayer) {
		auto value = INT_MIN;

		for (auto movement : movements)
		{
			chessboard.Move(movement);
			value = max(value, AlphaBeta(chessboard, depth - 1, alpha, beta, false));
			chessboard.Rollback();

			alpha = max(alpha, value);
			if (beta <= alpha) {
				return value;
			}
		}

		return value;
	}
	else {
		auto value = INT_MAX;

		for (auto movement : movements)
		{
			chessboard.Move(movement);
			value = min(value, AlphaBeta(chessboard, depth - 1, alpha, beta, true));
			chessboard.Rollback();

			beta = min(beta, value);
			if (beta <= alpha) {
				return value;
			}
		}
		return value;
	}
}

//Implementaion of MiniMax algorithm.
int Ai::MiniMax(Chessboard& chessboard, int depth, bool maximizingPlayer)
{
	if (depth == 0) {
		return evaluateBoard(chessboard);
	}

	auto movements = chessboard.GetCorrectMovements();

	if (maximizingPlayer) {
		auto value = INT_MIN;

		for (auto movement : movements)
		{
			chessboard.Move(movement);
			value = max(value, MiniMax(chessboard, depth - 1, false));
			chessboard.Rollback();
		}

		return value;
	}
	else {
		auto value = INT_MAX;

		for (auto movement : movements)
		{
			chessboard.Move(movement);
			value = min(value, MiniMax(chessboard, depth - 1, true));
			chessboard.Rollback();
		}
		return value;
	}
	//Minimax and Alphabeta are just typical blocks of code, which we can easily see on Wikipedia, but they do most of the job. Functions, used for evaluations, could differ significantly in different implementations.But they can be pretty simple like here.
}

//Entry point for Alpha-beta or MiniMax algorithm.
//Return best estimated movement.
Movement Ai::FindBestMovement(Chessboard& chessboard, int depth, bool maximizingPlayer)
{
	auto movements = chessboard.GetCorrectMovements();
	int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;
	Movement* bestMovement = nullptr;

	for (auto& movement : movements)
	{
		chessboard.Move(movement);

		//Choose Alpha-beta or MiniMax algorithm.
		const auto value = AlphaBeta(chessboard, depth - 1, INT_MIN, INT_MAX, !maximizingPlayer);
		//const auto value = MiniMax(chessboard, depth - 1, !maximizingPlayer);

		chessboard.Rollback();

		if (value >= bestValue && maximizingPlayer || value <= bestValue && !maximizingPlayer) {
			bestValue = value;
			bestMovement = &movement;
		}
	}

	if (!bestMovement) throw exception("No movements");

	//std::cout << "Best value: " << bestValue << endl;
	return *bestMovement;
}


