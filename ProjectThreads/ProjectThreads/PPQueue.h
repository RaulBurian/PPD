#pragma once
#include <queue>
#include <mutex>
#include <functional>
#include "Puzzle.h"

using namespace std;

class PPQueue
{

private:
	std::priority_queue<Puzzle,std::vector<Puzzle>, std::greater<std::vector<Puzzle>::value_type>> queue;
	std::mutex mtx;

public:

	void push(Puzzle p) {
        std::unique_lock<std::mutex> lkc(mtx);
        queue.push(p);
	}

    Puzzle pop() {
        std::unique_lock<std::mutex> lkc(mtx);
        Puzzle p = queue.top();
        queue.pop();
        return p;
    }

    bool empty() {
        std::unique_lock<std::mutex> lkc(mtx);
        return queue.empty();
    }
};

