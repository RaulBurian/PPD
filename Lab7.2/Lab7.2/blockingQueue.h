#pragma once
#include<queue>
#include<condition_variable>
#include<mutex>

template <typename T>
class blockingQueue
{
private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cond;

public:

	void push(T const value) {
		std::unique_lock<std::mutex> lck(mutex);
		queue.push(value);
		cond.notify_one();
	}

	T pop() {
		std::unique_lock<std::mutex> lck(mutex);
		while (queue.empty()) {
			cond.wait(lck);
		}
		T top = queue.front();
		queue.pop();
		return top;
	}

	int size() {
		return queue.size();
	}
};

