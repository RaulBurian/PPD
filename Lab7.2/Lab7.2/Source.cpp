#include<vector>
#include<iostream>
#include<string>
#include<thread>
#include<queue>
#include<stack>
#include<algorithm>
#include"blockingQueue.h"
#include<Windows.h>
#define LEN 8

using namespace std;

mutex mtx;

vector<string> numbers;
vector<blockingQueue<int>> queues{ LEN+1 };

void sumLast(int index1, int index2, int output) {
	blockingQueue<int>* q = &(queues[output]);
	string nr1 = numbers[index1];
	string nr2 = numbers[index2];
	int i = 0;
	int carry = 0;
	int remainder = 0;
	while (i < nr1.size()) {
		int number1 = nr1[i] - '0';
		int number2 = nr2[i] - '0';
		/*carry = (number1+number2+carry) / 10;
		remainder = (number1 + number2 + carry) % 10;*/
		remainder = number1 + number2;
		q->push(remainder);
		i++;
		//Sleep(1000);
	}
	q->push(-1);
}

void sum(int input1, int input2, int output) {
	blockingQueue<int>* outQueue = &(queues[output]);
	blockingQueue<int>* inQueue1 = &(queues[input1]);
	blockingQueue<int>* inQueue2 = &(queues[input2]);
	int in1 = 0;
	int in2 = 0;
	int carry = 0;
	int remainder = 0;
	do {
		in1 = inQueue1->pop();
		in2 = inQueue2->pop();
		if (in1 != -1 && in2 != -1) {
			/*carry = (in1 + in2 + carry) / 10;
			remainder = (in1 + in2 + carry) % 10;*/
			remainder = in1 + in2;
			outQueue->push(remainder);
			//cout << in1 << " " << in2 << "\n";
		}
	} while (in1 != -1 && in2 != -1);
	outQueue->push(carry);
	outQueue->push(-1);
}


int main() {

	vector<thread> threads;

	for (size_t i = 0; i < LEN; i++)
	{
		numbers.push_back("22222222");
	}

	for (size_t i = 1; i < LEN; i++)
	{
		if (i < LEN / 2) {
			threads.emplace_back(sum, 2 * i, 2 * i + 1, i);
		}
		else {
			threads.emplace_back(sumLast, 2 * i - LEN, 2 * i + 1 - LEN, i);
		}
	}

	for (size_t i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}
	int carry = 0;
	stack<int> st;
	for (size_t i = 0;queues[1].size(); i++)
	{
		
		int number = queues[1].pop();
		if (number != -1) {
			number += carry;
			st.push(number % 10);
			carry = (number + carry) / 10;
		}
	}
	while (st.size() > 0) {
		int number = st.top();
		st.pop();
		if (number != 0)
			cout << number;
	}
	return 0;
}