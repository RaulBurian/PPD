#pragma once

#include <atomic>
#include <vector>
#include <iostream>
#include <shared_mutex>
#include "Transfer.h"
#include "Account.h"



class Account;
class Transfer;

/*
	@returns - an integer representing a unique serial number
*/

int generateSerial();
/*
	@returns - an integer representing a unique id
*/

int generateId();
/*
	@param owner - a pointer to the source account for the transfer
	@param recipient - a pointer to the destination account for the trasnfer
	@param money - an integer representing the sum of money to be trasnfered

	@returns - true if the trasnfer is successfull, false otherwise
*/

bool transfer(Account* owner, Account* recipient, int money);

/*
	Checks the consistency of the transactions
	@param accounts - the vector of accounts
*/
void consistencyCheck(std::vector<Account*> accounts);

/*
	The task to be executed by each thread
	@param ops - the number of operations for the thread to perform
	@param accounts - the accounts to perform the task upon
*/
void task(int ops,std::vector<Account*> accounts);

void check(std::vector<Account*> accounts, bool* done);