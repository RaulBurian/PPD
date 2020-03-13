#pragma once

#include <string>
#include <vector>
#include <mutex>
#include "Transfer.h"
#include "Utils.h"

class Transfer;

/*
	Fields:
	id - the unique ID of an account
	money - the amount transfered
	owner - the owner of the account
	transfers - a "log" with all the trasnfers to or from that account
	mtx - a mutex, to provide exclusive access to the account information
*/
class Account
{
private:
	int id;
	int money;
	std::string owner;
	std::vector<Transfer> transfersSent;
	std::vector<Transfer> transfersReceived;
	std::mutex mtx;
	int lastCheckedSent;
	int lastCheckedRec;
	int lastMoney;
public:
	Account(int money, std::string owner);
	~Account();

	/*
		@returns - an integer, representing the ID of the account
	*/
	int getId() { return id; };

	/*
		@returns - an integer, representing the money available in the account
	*/
	int getMoney() { return money; };

	/*
		@returns - a string, representing the owner of the account
	*/
	const std::string& getOwner() { return owner; };

	/*
		@returns - the mutex, used to secure that account
	*/
	std::mutex& getMutex() { return mtx; };

	/*
		@returns - a vector of transfers, representing the transfers sent of that account
	*/
	const std::vector<Transfer> getTransfersSent() {
		mtx.lock();
		std::vector<Transfer> t = transfersSent;
		mtx.unlock();
		return t; };

	/*
		@returns - a vector of transfers, representing the transfers received of that account
	*/
	const std::vector<Transfer> getTransfersReceived() {
		mtx.lock();
		std::vector<Transfer> t = transfersReceived;
		mtx.unlock();
		return t; 
	};

	/*
		Changes the available amount of money to the new value
		@param amouont - the new amount of money of the account
	*/
	void setMoney(int amount) { money = amount; };

	/*
		Adds a new transfer for that account
		@param t - a transfer from that account to be "logged"
	*/
	void addTransferSent(Transfer t);

	/*
		Adds a new transfer for that account
		@param t - a transfer to that account to be "logged"
	*/
	void addTransferReceived(Transfer t);

	int getLastCheckedSent() { return lastCheckedSent; }

	void setLastCheckedSent(int newLast) { lastCheckedSent = newLast; }

	int getLastCheckedRec() { return lastCheckedRec; }

	void setLastCheckedRec(int newLast) { lastCheckedRec = newLast; }

	int getLastMoney() { return lastMoney; }

	void setLastMoney(int newlast) { lastMoney = newlast; }
};

