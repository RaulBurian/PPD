#include<iostream>
#include<functional>
#include<mutex>
#include<Windows.h>
#include "Puzzle.h"
#include "Pair.h"
#include "ThreadPool.h"
#include "PPQueue.h"


using namespace std;

#define NRT 16

mutex mtx;
Puzzle bestSolution;
ThreadPool tp(NRT);
bool found=false;
PPQueue q;

void search(Puzzle p) {
	if (found && p.getMoves() >= bestSolution.getMoves())
		return;

	int sol = p.isSolution();
	if (sol != -1) {
		mtx.lock();
		if (!found) {
			bestSolution = p;
			found = true;
			mtx.unlock();
			return;
		}
		else if (found && sol < bestSolution.getMoves()) {
			cout <<"BestMoves:"<< bestSolution.getMoves() << '\n';
			cout <<"NewMoves:"<< p.getMoves() << '\n';
			bestSolution = p;
			found = true;
			mtx.unlock();
			return;
		}
		mtx.unlock();
	}
	vector<Pair> moves = p.getPossibleMoves();
	for (Pair move : moves) {
		Puzzle movedPuzzle(p);
		movedPuzzle.makeMove(move);
		q.push(movedPuzzle);
	}
	for (Pair move : moves) {
		Puzzle toSearch = q.pop();
		tp.enqueue([toSearch]() {search(toSearch); });
	}

}


int main() {

	Puzzle p= bestSolution;
	p.printPuzzle();
	cout << '\n';

	search(p);

	tp.close();

	bestSolution.printPuzzle();
	cout << bestSolution.getMoves()<<'\n';
	bestSolution.printMoves();
	return 0;
}