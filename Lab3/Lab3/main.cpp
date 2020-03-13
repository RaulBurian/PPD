#include<iostream>
#include<vector>
#include"Operations.h"
#include"ThreadPool.h"

using namespace std;

void startNThreadsAdd(int **a, int **b, int **result) {
	ThreadPool t(12);
	for (int i = 0; i < SIZE; i++) {
		t.enqueue([i, a, b, result]() {addNThread(a, b, result, i); });
	}

	t.close();
}
void startNThreadsMultiply(int **a, int **b, int **result) {
	
	ThreadPool t(12);

	for (int i = 0; i < SIZE; i++) {
		t.enqueue([i, a, b, result]() {multiplyNThread(a, b, result, i); });
	}

	t.close();
}

void startElemThreadsAdd(int** a, int** b, int** result) {
	
	ThreadPool t(12);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++)
			t.enqueue([i,j, a, b, result]() {addElemThread(a, b, result, i,j); });
	}

	t.close();
}

void startElemThreadsMultiply(int** a, int** b, int** result) {
	ThreadPool t(12);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++)
			t.enqueue([i, j, a, b, result]() {multiplyElemThread(a, b, result, i, j); });
	}

	t.close();
}

void menu() {
	cout << "0.Exit\n";
	cout << "1.Add one task\n";
	cout << "2. Add N tasks\n";
	cout << "3. Add N*N tasks\n";
	cout << "4. Mult one tasks\n";
	cout << "5. Mult N tasks\n";
	cout << "6. Mult N*N tasks\n";
	cout << "Enter command:";
}

int main() {

	std::chrono::steady_clock::time_point begin, end;
	//int a**, b**, result**;
	int** a, ** b, ** result;
	a = new int* [SIZE];
	b = new int* [SIZE];
	result = new int* [SIZE];
	for (int i = 0; i < SIZE; i++) {
		a[i] = new int[SIZE];
		b[i] = new int[SIZE];
		result[i] = new int[SIZE];
	}

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++) {
			a[i][j] = i + j;
			b[i][j] = SIZE - i + j;
		}
	int command;
	while (true) {
		menu();
		cin >> command;
		switch (command) {
		case 0:
			return 0;
		case 1:
			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					a[i][j] = i + j;
					b[i][j] = SIZE - i + j;
				}
			begin = chrono::high_resolution_clock::now();
			//multiplyOneThread(a, b, result);
			addOneThread(a, b, result);
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
			//startNThreadsMultiply(a, b, result);
			startNThreadsAdd(a, b, result);
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
			//startElemThreadsMultiply(a, b, result);
			startElemThreadsAdd(a, b, result);
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
			//addOneThread(a, b, result);
			multiplyOneThread(a, b, result);
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
			//startNThreadsAdd(a, b, result);
			startNThreadsMultiply(a, b, result);
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

	return 0;
}
//Mult and Add on NxN tasks never worth
//Mult on N tasks > Mult on one task from SIZE~=200
//Add on N tasks > Add on one task from SIZE~=8000