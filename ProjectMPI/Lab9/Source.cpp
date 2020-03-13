#include <iostream>
#include <mpi.h>
#include <vector>
#include <fstream>
#include "Puzzle.h"
#include "PPQueue.h"
#include <set>

using namespace std;

#define PUZZLE -1
#define SHUTDOWN -2
#define BEGIN_SEND -3

#define TAG_METADATA 1
#define TAG_PUZZLE_CONTENT 2
#define TAG_FREE_SPACE 3
#define TAG_MOVES 4
#define TAG_SHUTDOWN 10

ofstream fout("out.txt");

Puzzle search(Puzzle p, int nrProcs) {

	Puzzle bestSolution, puzzle, recvPuzzle;
	bool found = false;
	bool done = false;
	PPQueue q;
	int toSend = 0;
	int metadata;
	int free[2];
	vector<int> puzz;
	Pair freeSpace;
	int m, nrMoves;
	MPI_Status status;
	set<int> receivers;


	vector<Pair> moves = p.getPossibleMoves();
	for (Pair move : moves) {
		Puzzle movedPuzzle(p);
		movedPuzzle.makeMove(move);
		q.push(movedPuzzle);
	}
	while (!done) {

		receivers.clear();

		while (!q.empty()) {
			puzzle = q.pop();

			if (!found && puzzle.isSolution() != -1) {
				bestSolution = puzzle;
				found = true;
			}
			else if (puzzle.isSolution() != -1 && puzzle.getMoves() < bestSolution.getMoves()) {
				bestSolution = puzzle;
			}
			else {
				toSend++;
				toSend = toSend % nrProcs;
				if (toSend == 0)
					toSend++;

				metadata = PUZZLE;

				//Send metadata for worker to know it expects a puzzle
				MPI_Ssend(&metadata, 1, MPI_INT, toSend, TAG_METADATA, MPI_COMM_WORLD);


				//Prepare the puzzle to be sent
				puzz = puzzle.toMpiSend();
				freeSpace = puzzle.getFreeSpace();
				free[0] = freeSpace.line;
				free[1] = freeSpace.col;
				m = puzzle.getMoves();


				//Send the puzzle
				MPI_Ssend(puzz.data(), 16, MPI_INT, toSend, TAG_PUZZLE_CONTENT, MPI_COMM_WORLD);
				MPI_Ssend(free, 2, MPI_INT, toSend, TAG_FREE_SPACE, MPI_COMM_WORLD);
				MPI_Ssend(&m, 1, MPI_INT, toSend, TAG_MOVES, MPI_COMM_WORLD);

				receivers.insert(toSend);
			}

		}

		for (int r : receivers) {
			metadata = BEGIN_SEND;
			MPI_Ssend(&metadata, 1, MPI_INT, r, TAG_METADATA, MPI_COMM_WORLD);

			MPI_Recv(&nrMoves, 1, MPI_INT, r, TAG_METADATA, MPI_COMM_WORLD, &status);
			for (int i = 0; i < nrMoves; i++) {
				MPI_Recv(puzz.data(), 16, MPI_INT, r, TAG_PUZZLE_CONTENT, MPI_COMM_WORLD, &status);
				MPI_Recv(free, 2, MPI_INT, r, TAG_FREE_SPACE, MPI_COMM_WORLD, &status);
				MPI_Recv(&m, 1, MPI_INT, r, TAG_MOVES, MPI_COMM_WORLD, &status);

				recvPuzzle = Puzzle(puzz, free[0], free[1], m);

				if (!found || (found && recvPuzzle.getMoves() < bestSolution.getMoves())) {
					q.push(recvPuzzle);
				}
			}
		}

		if (q.empty()) {
			done = true;
		}
	}

	for (int i = 1; i < nrProcs; i++) {
		int metadata = SHUTDOWN;
		MPI_Ssend(&metadata, 1, MPI_INT, i, TAG_METADATA, MPI_COMM_WORLD);
	}

	return bestSolution;

}

void worker(int me) {

	bool done = false;
	int metadata;
	int free[2];
	vector<int> puzz(16);
	Pair freeSpace;
	int m, nrMoves;
	MPI_Status status;
	Puzzle puzzle, movedPuzzle;
	vector<Pair> moves;
	vector<Puzzle> puzzles;

	while (!done) {

		MPI_Recv(&metadata, 1, MPI_INT, 0, TAG_METADATA, MPI_COMM_WORLD, &status);

		switch (metadata) {
		case PUZZLE:

			MPI_Recv(puzz.data(), 16, MPI_INT, 0, TAG_PUZZLE_CONTENT, MPI_COMM_WORLD, &status);
			MPI_Recv(free, 2, MPI_INT, 0, TAG_FREE_SPACE, MPI_COMM_WORLD, &status);
			MPI_Recv(&m, 1, MPI_INT, 0, TAG_MOVES, MPI_COMM_WORLD, &status);
			puzzle = Puzzle(puzz, free[0], free[1], m);

			moves = puzzle.getPossibleMoves();

			for (Pair move : moves) {
				movedPuzzle = puzzle;
				movedPuzzle.makeMove(move);

				puzzles.push_back(movedPuzzle);

			}

			break;
		case SHUTDOWN:
			done = true;
			break;
		case BEGIN_SEND:
			nrMoves = puzzles.size();
			MPI_Ssend(&nrMoves, 1, MPI_INT, 0, TAG_METADATA, MPI_COMM_WORLD);
			for (auto movedPuzzle : puzzles) {
				puzz = movedPuzzle.toMpiSend();
				freeSpace = movedPuzzle.getFreeSpace();
				free[0] = freeSpace.line;
				free[1] = freeSpace.col;
				m = movedPuzzle.getMoves();

				MPI_Ssend(puzz.data(), 16, MPI_INT, 0, TAG_PUZZLE_CONTENT, MPI_COMM_WORLD);
				MPI_Ssend(free, 2, MPI_INT, 0, TAG_FREE_SPACE, MPI_COMM_WORLD);
				MPI_Ssend(&m, 1, MPI_INT, 0, TAG_MOVES, MPI_COMM_WORLD);
			}
			puzzles.clear();
			break;
		}
	}
}


int main(int argc, char** argv)
{
	MPI_Init(0, 0);
	int me;
	int nrProcs;
	MPI_Comm_size(MPI_COMM_WORLD, &nrProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	if (me == 0) {
		Puzzle p;
		p.printPuzzle();
		p = search(p, nrProcs);
		p.printPuzzle();
		cout << p.getMoves() << '\n';
	}
	else {
		worker(me);
	}

	MPI_Finalize();
}
