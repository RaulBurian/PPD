#include "Account.h"

Account::Account(int  money, std::string owner)
{
	this->id = generateId();
	this->money = money;
	this->owner = owner;
	this->lastCheckedSent = 0;
	this->lastCheckedRec = 0;
	this->lastMoney = money;
}

Account::~Account()
{
}

void Account::addTransferSent(Transfer transfer)
{
	
	this->transfersSent.push_back(transfer);

}

void Account::addTransferReceived(Transfer transfer)
{
	
	this->transfersReceived.push_back(transfer);
	
}
