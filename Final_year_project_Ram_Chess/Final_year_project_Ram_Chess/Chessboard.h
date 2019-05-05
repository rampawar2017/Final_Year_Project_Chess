#pragma once
#include <vector>
#include "Movement.h"
#include <stack>
using namespace std;

struct Coord;

//Types of cells (empty and pieces of two colors).
enum Cell {
	Empty,
	WhitePawn, WhiteKnight, WhiteBishop, WhiteRook, WhiteQueen, WhiteKing,
	BlackPawn, BlackKnight, BlackBishop, BlackRook, BlackQueen, BlackKing
};

//Structure, used to store change in state of chessboard after a movement.
struct State
{
	Cell prevSrcCell;
	Cell prevDestCell;
	Movement movement;
};

//Main class, where chess game rules are implemented.
class Chessboard
{
public:
	static const int size = 8;
	Cell cells[size][size];
private:
	bool isBlack = false;
	bool isCheck = false;
	bool isMate = false;
	bool isStalemate = false;
	stack<State> states;

	static bool IsInside(const Coord& coord);
	vector<Movement> GetAllMovements(bool isBlack) const;
	vector<Coord> GetMoves(const Coord& srcCoord) const;
	bool IsCheck(bool isBlack);
public:
	bool is_check() const;
	bool is_mate() const;
	bool is_stalemate() const;
	bool is_black() const;
	void Init();
	char CellToSymbol(Cell cell) const;
	void UpdateState(bool full);
	void Show() const;

	Chessboard();
	static bool IsBlack(Cell cell);
	bool IsValid(const Movement& movement) const;
	void Move(const Movement& movement);
	void Rollback();
	vector<Movement> GetCorrectMovements();
};

