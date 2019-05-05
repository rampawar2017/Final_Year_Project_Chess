#pragma once
#include "Coord.h"
#include "Chessboard.h"
#include <vector>
#include "State.h"
using namespace std;

class Chessboard;

enum Color { Black, White };

class Piece
{
	Color _color;
public:
	Color color() const
	{
		return _color;
	}

	Piece(Color color);
	virtual bool IsValid(const Chessboard& chessboard, const Coord& coord) = 0;
	virtual vector<Coord> GetMoves(const Chessboard& chessboard) = 0;
	virtual char Symbol() = 0;
};

