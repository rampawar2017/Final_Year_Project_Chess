#pragma once

//Structure, which defines a coordinate in chessboard.
struct Coord
{
	const int x, y;

	Coord(int x, int y);

	bool operator==(const Coord& other) const
	{
		return x == other.x && y == other.y;
	}
};