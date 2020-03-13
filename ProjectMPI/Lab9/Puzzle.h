#pragma once

#include<random>
#include<array>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <iomanip> 
#include <stdlib.h>
#include <vector>
#include "Pair.h"


class Puzzle
{
private:
	int puzzle[4][4] = { 0 };
	Pair freeSpace;
	int moves;
public:
	
	
	Puzzle() {
		std::array<int, 16> nrs{ 1,2,3,7,4,5,6,11,8,9,10,15,0,12,13,14};
		//std::array<int, 16> nrs{ 1,2,3,0,4,5,6,7,8,9,10,11,12,13,14,15};
		//int seed= std::chrono::system_clock::now().time_since_epoch().count();

		//std::shuffle(nrs.begin(), nrs.end(), std::default_random_engine(seed));

		int k = 0;
		for(int i=0;i<4;i++)
			for (int j = 0; j < 4; j++) {
				if (nrs[k] == 0) {
					freeSpace = Pair(i, j);
				}
				puzzle[i][j] = nrs[k++];
				
			}
		moves = 0;
	}


	Puzzle(std::vector<int> puzz, int fI, int fJ, int moves) {
		this->moves = moves;
		this->freeSpace = Pair(fI, fJ);
		int k = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				puzzle[i][j] = puzz[k++];
			}
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
	}

	std::vector<int> toMpiSend() {
		std::vector<int> f;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				f.push_back(puzzle[i][j]);
			}
		return f;
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

