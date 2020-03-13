#pragma once

#include <atomic>
#include "Account.h"
#include "Utils.h"

class Account;

/*
	Fields:
	serial - the identifier for a transfer
	owner - the sender account of the transfer
	recipient - the reciever account of the transfer
	money - the amount transfered
	status - shows is the transfer was successfull or not
*/
class Transfer
{

private:
	int serial;
	Account* owner;
	Account* recipient;
	int money;
	bool status;
public:
	Transfer(Account* owner, Account* recipient, int money, bool status);
	~Transfer();

	/*
		@returns - a pointer to the sender account of the transfer
	*/
	Account* getOwner() { return owner; }

	/*
		@returns - a pointer to the reciever account of the transfer
	*/
	Account* getRecipient() { return recipient; }

	/*
		@returns - the amount of money transfered
	*/
	int getMoney() { return money; }

	/*
		@returns - the status of the transfer
	*/
	bool getStatus() { return status; }

	int getSerial() { return serial; }

};

