#include<vector>
#include<iostream>
#include<future>
#include<random>
#include<Windows.h>

#define LEN 5

using namespace std;

vector<int> a,b;


vector<int> randomVector() {
	vector<int> c;
	int x;
	for (size_t i = 0; i < LEN; i++)
	{
		x = rand() % 10;
		c.push_back(x);
	}
	return c;
}


void partialCompute1(int k) {

	vector<future<void>> tasks1;
	for (size_t i = 2*k-1; i < LEN; i+=2*k)
	{
		tasks1.push_back(async([=]() {b[i] += b[i - k]; }));
		//b[i] += b[i - k];
	}

	for (size_t i = 0; i < tasks1.size(); i++)
	{
		tasks1[i].wait();
	}
}

void partialCompute2(int k) {

	vector<future<void>> tasks2;

	for (size_t i = 3 * k - 1; i < LEN; i += 2 * k)
	{
		tasks2.push_back(async([=]() {b[i] += b[i - k]; }));
		//b[i] += b[i - k];
	}

	for (size_t i = 0; i < tasks2.size(); i++)
	{
		tasks2[i].wait();
	}
}


int main() {

	a = vector<int>{ 4,5,2,4,4 };

	b = a;

	vector<future<void>> tasks1;
	vector<future<void>> tasks2;

	int k;

	for (k = 1; k < LEN; k=k*2)
	{
		partialCompute1(k);
	}


	int x=k / 4;

	while (x > 0) {

		partialCompute2(x);

		x /= 2;
	}



	for (auto x : b) {
		cout << x << " ";
	}

	return 0;
}