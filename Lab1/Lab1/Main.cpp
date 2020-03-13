#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include "Account.h"
#include "Transfer.h"
#include "Utils.h"


#define T 100000
#define TR 4
#define OPS T/TR

using namespace std;

vector<Account*> createAccounts(){
	vector<Account*> accounts;
	accounts.push_back(new Account(10000,"A"));
	accounts.push_back(new Account(11000,"B"));
	accounts.push_back(new Account(12000,"C"));
	accounts.push_back(new Account(8000,"D"));
	accounts.push_back(new Account(50000,"E"));
	accounts.push_back(new Account(70000,"F"));
	accounts.push_back(new Account(20000,"G"));
	accounts.push_back(new Account(30000,"H"));
	accounts.push_back(new Account(2000,"I"));
	accounts.push_back(new Account(40000,"J"));

	return accounts;
}

void freeAccounts(vector<Account*> accounts) {
	for (auto a : accounts) {
		delete a;
	}
}

int main() {

	bool* done = new bool(false);
	vector<thread> threads;
	vector<Account*> accounts = createAccounts();
	auto begin = chrono::high_resolution_clock::now();
	
	for (int i = 0; i < TR; i++) {
		threads.push_back(thread(task,OPS,accounts));
	}
	thread t{ check ,accounts,done };
	for (int i = 0; i < TR; i++) {
		threads[i].join();
	}
	*done = true;
	auto end = chrono::high_resolution_clock::now();
	cout << "\nTransfers took:" << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms\n";
	t.join();
	consistencyCheck(accounts);
	freeAccounts(accounts);
	delete done;

	return 0;
}

/*
	T=100000 ops, TR=2 => ~255 ms
	T=100000 ops, TR=6 => ~210 ms
	T=100000 ops, TR=12 => ~255 ms 
	T=100000 ops, TR=100 => ~290 ms
	T=10000000 ops, TR=100 => 20 sec
	T=10000000 ops, TR=12 => 20 sec

*/