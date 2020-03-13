#include <iostream>
#include <chrono>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include "ThreadPool.h"
#include <functional>

#define SIZE 100
#define NRT 12

using namespace std;

void printMatrix(int** a) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			cout << a[i][j] << "  ";
		}
		cout << '\n';
	}
}

void multiplyFirst(int** a, int** b, int** result, int lineNr,mutex* mtx,condition_variable* cond,bool* done) {
	unique_lock<mutex> l(*mtx);
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][i] = 0;
		for (int j = 0; j < SIZE; j++) {
			result[lineNr][i] += a[lineNr][j] * b[j][i];
		}
	}
	*done = true;
	cond->notify_one();
	//cond->notify_all();
}

void multiplySecond(int** a, int** b, int** result, int lineNr, mutex* mtx, condition_variable* cond,bool* done) {
	unique_lock<mutex> l(*mtx);
	while (!(*done))
		cond->wait(l);
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][i] = 0;
		for (int j = 0; j < SIZE; j++) {
			result[lineNr][i] += a[lineNr][j] * b[j][i];
		}
	}
	
}


void multiplyElemThread(int** a, int** b, int** result, int lineNr, int colNr,mutex* mtx,condition_variable* cond,bool* done) {
	unique_lock<mutex> l(*mtx);
	while (!(*done))
		cond->wait(l);
	result[lineNr][colNr] = 0;
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][colNr] += a[lineNr][i] * b[i][colNr];
	}
}

int main() {

	std::chrono::steady_clock::time_point begin, end;
	mutex* mutexes;
	condition_variable* conditionals;
	bool* done;
	int** a, ** b,**ab,**c, ** result;
	a = new int* [SIZE];
	b = new int* [SIZE];
	c = new int* [SIZE];
	ab = new int* [SIZE];
	mutexes = new mutex[SIZE];
	conditionals = new condition_variable[SIZE];
	done = new bool[SIZE];
	result = new int* [SIZE];
	for (int i = 0; i < SIZE; i++) {
		a[i] = new int[SIZE];
		b[i] = new int[SIZE];
		c[i] = new int[SIZE];
		ab[i] = new int[SIZE];
		result[i] = new int[SIZE];
	}

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++) {
			a[i][j] = i + j;
			b[i][j] = SIZE - i + j;
			c[i][j] = i + j;
		}
	/*printMatrix(a);
	printMatrix(b);
	printMatrix(c);*/
	/*vector<future<void>> tasks;
	for (unsigned int i = 0; i < SIZE; i++)
	{
		tasks.push_back(async(launch::async,multiplyFirst,a,b,ab,i,&mutexes[i],&conditionals[i],&done[i]));
		tasks.push_back(async(launch::async,multiplySecond,ab,c,result,i,&mutexes[i],&conditionals[i],&done[i]));
	}

	for (unsigned int i = 0; i < tasks.size(); i++)
	{
		tasks[i].wait();
	}*/

	//printMatrix(result);

	begin = chrono::high_resolution_clock::now();
	//vector<thread> threads;
	//for (unsigned int i = 0; i < SIZE; i++)
	//{
	//	threads.push_back(thread{multiplyFirst,a,b,ab,i,&mutexes[i],&conditionals[i],&done[i]});
	//	threads.push_back(thread{multiplySecond,ab,c,result,i,&mutexes[i],&conditionals[i],&done[i]});

	//	//for(unsigned int j=0;j<SIZE;j++)
	//	//	threads.push_back(thread{multiplyElemThread,ab,c,result,i,j,&mutexes[i],&conditionals[i],&done[i]});
	//}

	//for (size_t i = 0; i < threads.size(); i++)
	//{
	//	threads[i].join();
	//}


	ThreadPool tp{ NRT };

	for (unsigned int i = 0; i < SIZE; i++)
	{
		tp.enqueue([i, a, b, ab, mutexes, conditionals, done]() {multiplyFirst(a, b, ab, i, &mutexes[i], &conditionals[i], &done[i]); });
		tp.enqueue([i, ab, c, result, mutexes, conditionals, done]() {multiplySecond(ab, c, result, i, &mutexes[i], &conditionals[i], &done[i]); });
	}

	
	tp.close();
	end = chrono::high_resolution_clock::now();
	cout <<"The multiplications took:"<< std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";

	delete[] mutexes;
	delete[] conditionals;
	delete[] done;
	return 0;
}