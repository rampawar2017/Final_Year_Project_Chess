#pragma once
#include "Piece.h"
class Pawn : public Piece
{
public:
	char Symbol() override;
	vector<Coord> GetMoves(const Chessboard& chessboard) override;
	bool IsValid(const Chessboard& chessboard, const Coord& coord) override;
	Pawn(Color color);
};

