#include<iostream>
#include "Operations.h"


using namespace std;

void printMatrix(int **a) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			cout << a[i][j] << "  ";
		}
		cout << '\n';
	}
}

void multiplyOneThread(int** a, int** b, int** result) {
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			result[i][j] = 0;
			for (int k = 0; k < SIZE; k++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
}

void addOneThread(int** a, int** b, int** result) {
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++) {
			result[i][j] = a[i][j] + b[i][j];
		}
}

void multiplyNThread(int** a, int** b, int** result, int lineNr) {
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][i] = 0;
		for (int j = 0; j < SIZE; j++) {
			result[lineNr][i] += a[lineNr][j] * b[j][i];
		}
	}
}

void addNThread(int** a, int** b, int** result, int lineNr) {
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][i] = a[lineNr][i] + b[lineNr][i];
	}
}

void addElemThread(int** a, int** b, int** result, int lineNr, int colNr) {
	result[lineNr][colNr] = a[lineNr][colNr] + b[lineNr][colNr];
}

void multiplyElemThread(int** a, int** b, int** result, int lineNr, int colNr) {
	result[lineNr][colNr] = 0;
	for (int i = 0; i < SIZE; i++) {
		result[lineNr][colNr] += a[lineNr][i] * b[i][colNr];
	}
}