#include "Transfer.h"


Transfer::Transfer(Account* owner, Account* recipient, int money,bool status)
{
	this->serial = generateSerial();
	this->recipient = recipient;
	this->owner = owner;
	this->money = money;
	this->status = status;
}

Transfer::~Transfer()
{
}
