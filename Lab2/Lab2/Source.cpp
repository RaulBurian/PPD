#include <vector>
#include <thread>
#include <iostream>
#include <chrono>

#define SIZE 50
using namespace std;

void printMatrix(int a[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			cout << a[i][j] <<"  ";
		}
		cout << '\n';
	}
}

void multiplyOneThread(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			result[i][j] = 0;
			for (int k = 0; k < SIZE; k++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
}

void addOneThread(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++) {
			result[i][j] = a[i][j] + b[i][j];
		}
}

void multiplyNThread(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE],int lineNr) {
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][i] = 0;
		for (int j = 0; j < SIZE; j++) {
			result[lineNr][i] += a[lineNr][j] * b[j][i];
		}
	}
}

void addNThread(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE], int lineNr) {
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][i] = a[lineNr][i] + b[lineNr][i];
	}
}

void addElemThread(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE], int lineNr, int colNr) {
	result[lineNr][colNr] = a[lineNr][colNr] + b[lineNr][colNr];
}

void multiplyElemThread(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE], int lineNr, int colNr) {
	result[lineNr][colNr] = 0;
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][colNr] += a[lineNr][i] * b[i][colNr];
	}
}

void startNThreadsAdd(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]) {
	vector<thread> threads;
	for (int i = 0; i < SIZE; i++) {
		threads.push_back(thread(addNThread, a, b, result, i));
	}

	for (int i = 0; i < SIZE; i++) {
		threads[i].join();
	}
}
void startNThreadsMultiply(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]) {
	vector<thread> threads;
	for (int i = 0; i < SIZE; i++) {
		threads.push_back(thread(multiplyNThread, a, b, result, i));
	}

	for (int i = 0; i < SIZE; i++) {
		threads[i].join();
	}
}

void startElemThreadsAdd(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]) {
	vector<thread> threads;
	for (int i = 0; i < SIZE; i++) {
		for(int j=0;j<SIZE;j++)
			threads.push_back(thread(addElemThread, a, b, result, i,j));
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}

void startElemThreadsMultiply(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]) {
	vector<thread> threads;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++)
			threads.push_back(thread(multiplyElemThread, a, b, result, i, j));
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}

int main() {
	std::chrono::steady_clock::time_point begin, end;
	int a[SIZE][SIZE], b[SIZE][SIZE], result[SIZE][SIZE];
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++) {
			a[i][j] = i+j;
			b[i][j] = SIZE-i+j;
		}
	int command;
	while (true) {
		cin >> command;
		switch (command) {
		case 1:
			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					a[i][j] = i + j;
					b[i][j] = SIZE - i + j;
				}
			 begin = chrono::high_resolution_clock::now();
			multiplyOneThread(a, b, result);
			 end = chrono::high_resolution_clock::now();
			cout << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";
			//printMatrix(result);
			break;
		case 2:
			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					a[i][j] = i + j;
					b[i][j] = SIZE - i + j;
				}
			 begin = chrono::high_resolution_clock::now();
			startNThreadsMultiply(a, b, result);
			 end = chrono::high_resolution_clock::now();
			cout << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";
			//printMatrix(result);
			break;
		case 3:
			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					a[i][j] = i + j;
					b[i][j] = SIZE - i + j;
				}
			 begin = chrono::high_resolution_clock::now();
			startElemThreadsMultiply(a, b, result);
			 end = chrono::high_resolution_clock::now();
			cout << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";
			//printMatrix(result);
			break;
		case 4:
			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					a[i][j] = i + j;
					b[i][j] = SIZE - i + j;
				}
			 begin = chrono::high_resolution_clock::now();
			addOneThread(a, b, result);
			 end = chrono::high_resolution_clock::now();
			cout << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";
			//printMatrix(result);
			break;
		case 5:
			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					a[i][j] = i + j;
					b[i][j] = SIZE - i + j;
				}
			 begin = chrono::high_resolution_clock::now();
			startNThreadsAdd(a, b, result);
			 end = chrono::high_resolution_clock::now();
			cout << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";
			//printMatrix(result);
			break;
		case 6:
			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					a[i][j] = i + j;
					b[i][j] = SIZE - i + j;
				}
			 begin = chrono::high_resolution_clock::now();
			startElemThreadsMultiply(a, b, result);
			 end = chrono::high_resolution_clock::now();
			cout << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";
			//printMatrix(result);
			break;

		}
	}
	/*startElemThreadsMultiply(a, b, result);
	printMatrix(result);*/


	//startNThreadsMultiply(a, b, result);
	//printMatrix(result);


	multiplyOneThread(a, b, result);
	printMatrix(result);
	return 0;
}