#pragma once

#include<random>
#include<array>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <iomanip> 
#include <stdlib.h>
#include "Pair.h"
#include "MovePair.h"

class Puzzle
{
private:
	int puzzle[4][4] = { 0 };
	Pair freeSpace;
	int moves;
	std::vector<MovePair> mvs;
public:
	
	
	Puzzle() {
		std::array<int, 16> nrs{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
		

		int k = 0;
		for(int i=0;i<4;i++)
			for (int j = 0; j < 4; j++) {
				if (nrs[k] == 0) {
					freeSpace = Pair(i, j);
				}
				puzzle[i][j] = nrs[k++];
				
			}
		
		shuffle(20);
		moves = 0;
		mvs.clear();
	}

	void shuffle(int shuffles) {
		std::vector<Pair> moves;
		printPuzzle();
		for (int i = 0; i < shuffles; i++) {
			moves = getPossibleMoves();
			int seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::shuffle(moves.begin(), moves.end(), std::default_random_engine(seed));
			makeMove(moves[0]);
			printPuzzle();
		}
		std::cout << "Print puzzle ended\n";
	}

	std::vector<Pair> getPossibleMoves() {
		std::vector<Pair> moves;
		if (freeSpace.line == 0) {
			if (freeSpace.col == 0) {
				moves.emplace_back(freeSpace.line + 1,freeSpace.col);
				moves.emplace_back(freeSpace.line, freeSpace.col + 1);
			}
			else if (freeSpace.col == 3) {
				moves.emplace_back(freeSpace.line + 1, freeSpace.col);
				moves.emplace_back(freeSpace.line, freeSpace.col - 1);
			}
			else {
				moves.emplace_back(freeSpace.line + 1, freeSpace.col);
				moves.emplace_back(freeSpace.line, freeSpace.col + 1);
				moves.emplace_back(freeSpace.line, freeSpace.col - 1);
			}
		}
		else if (freeSpace.line == 3) {
			if (freeSpace.col == 0) {
				moves.emplace_back(freeSpace.line - 1, freeSpace.col);
				moves.emplace_back(freeSpace.line, freeSpace.col + 1);
			}
			else if (freeSpace.col == 3) {
				moves.emplace_back(freeSpace.line - 1, freeSpace.col);
				moves.emplace_back(freeSpace.line, freeSpace.col - 1);
			}
			else {
				moves.emplace_back(freeSpace.line - 1, freeSpace.col);
				moves.emplace_back(freeSpace.line, freeSpace.col + 1);
				moves.emplace_back(freeSpace.line, freeSpace.col - 1);
			}
		}
		else if (freeSpace.col == 0) {
			moves.emplace_back(freeSpace.line - 1, freeSpace.col);
			moves.emplace_back(freeSpace.line + 1, freeSpace.col);
			moves.emplace_back(freeSpace.line, freeSpace.col + 1);
		}
		else if (freeSpace.col == 3) {
			moves.emplace_back(freeSpace.line - 1, freeSpace.col);
			moves.emplace_back(freeSpace.line + 1, freeSpace.col);
			moves.emplace_back(freeSpace.line, freeSpace.col - 1);
		}
		else {
			moves.emplace_back(freeSpace.line - 1, freeSpace.col);
			moves.emplace_back(freeSpace.line + 1, freeSpace.col);
			moves.emplace_back(freeSpace.line, freeSpace.col - 1);
			moves.emplace_back(freeSpace.line, freeSpace.col + 1);
		}

		int seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::shuffle(moves.begin(), moves.end(), std::default_random_engine(seed));
		return moves;
	}


	void makeMove(Pair toMove) {
		int aux = puzzle[freeSpace.line][freeSpace.col];
		puzzle[freeSpace.line][freeSpace.col] = puzzle[toMove.line][toMove.col];
		puzzle[toMove.line][toMove.col] = aux;
		mvs.emplace_back(freeSpace, toMove);
		freeSpace = toMove;
		moves++;
	}

	Pair getFreeSpace() {
		return freeSpace;
	}

	int isSolution() {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				if (puzzle[i][j] != i * 4 + j)
					return -1;
			}
		return moves;
	}

	int getMoves() {
		return moves;
	}


	int getHeuristic() const {
		int sum = 0;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				sum += std::abs(i * 4 + j - puzzle[i][j]);
			}

		return sum;
	}

	void printPuzzle() {
		
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::cout << std::setw(3);
				std::cout << puzzle[i][j] << " ";
			}
			std::cout << std::setw(2);
			std::cout << '\n';
		}
		std::cout << '\n';
	}

	void printMoves() {
		for (auto m : mvs) {
			std::cout << m.from.line << " : " << m.from.col << " -> " << m.to.line << " : " << m.to.col << '\n';
		}
	}

	friend bool operator < (Puzzle const& lhs, Puzzle const& rhs) {
		return lhs.getHeuristic() < rhs.getHeuristic();
	}
	friend bool operator > (const Puzzle& lhs, const Puzzle& rhs) {
		return lhs.getHeuristic() > rhs.getHeuristic();
	}	

	~Puzzle() {

	}
	
};

