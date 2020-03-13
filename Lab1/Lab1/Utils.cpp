#include "Utils.h"
#include <random>
#include <chrono>
#include <Windows.h>
#include <iostream>
#include <utility> 

std::shared_mutex centralLock{};

int generateSerial()
{
	static std::atomic<int> serial = 0;
	return serial++;
}

int generateId() {
	static std::atomic<int> id = 0;
	return id++;
}



bool transfer(Account* owner, Account* recipient, int money)
{
	
	centralLock.lock_shared();
	if (owner->getId() < recipient->getId()) {
		owner->getMutex().lock();
		recipient->getMutex().lock();
		if (owner->getMoney() < money) {
			Transfer t{ owner,recipient,money,false };
			owner->addTransferSent(t);
			recipient->addTransferReceived(t);
			owner->getMutex().unlock();
			recipient->getMutex().unlock();
			centralLock.unlock_shared();
			return false;
		}

		owner->setMoney(owner->getMoney() - money);
		recipient->setMoney(recipient->getMoney() + money);
		Transfer t{ owner,recipient,money,true };
		owner->addTransferSent(t);
		recipient->addTransferReceived(t);
		owner->getMutex().unlock();
		recipient->getMutex().unlock();
		centralLock.unlock_shared();
		return true;
	}
	else {
		recipient->getMutex().lock();
		owner->getMutex().lock();

		if (owner->getMoney() < money) {
			Transfer t{ owner,recipient,money,false };
			owner->addTransferSent(t);
			recipient->addTransferReceived(t);
			recipient->getMutex().unlock();
			owner->getMutex().unlock();
			centralLock.unlock_shared();
			return false;
		}
		owner->setMoney(owner->getMoney() - money);
		recipient->setMoney(recipient->getMoney() + money);
		Transfer t{ owner,recipient,money,true };
		owner->addTransferSent(t);
		recipient->addTransferReceived(t);
		recipient->getMutex().unlock();
		owner->getMutex().unlock();
		centralLock.unlock_shared();
		return true;
	}
	
}

void consistencyCheck(std::vector<Account*> accounts) {
	std::vector<std::pair<Account*, int>> sentSizes;
	std::vector<std::pair<Account*, int>> receivedSizes;
	std::vector<std::pair<Account*, int>> money;
	std::vector<std::pair<Account*, int>> moneyNow;
	int s1 = 0, s2 = 0;
	int delta = 0;
	centralLock.lock();
	for (Account* account : accounts) {
		sentSizes.push_back(std::pair<Account*, int>{account,account->getTransfersSent().size()});
		s1 += account->getTransfersSent().size();
		s2 += account->getTransfersReceived().size();
		receivedSizes.push_back(std::pair<Account*, int>{account,account->getTransfersReceived().size()});
		money.push_back(std::pair<Account*, int>{account, account->getMoney()});
		moneyNow.push_back(std::pair<Account*, int>{account, account->getMoney()});
	}
	centralLock.unlock();

	/*for (auto pair : sentSizes) {
		std::cout << pair.first->getId() << " sentSize:" << pair.second<<'\n';
	}

	for (auto pair : receivedSizes) {
		std::cout << pair.first->getId() << " recSize:" << pair.second<<'\n';
	}*/
	int i = 0;
	for (auto sentPair : sentSizes) {
		int lastMoney = money[i].first->getLastMoney();
		std::vector<Transfer> sentTransfers = sentPair.first->getTransfersSent();
		for (int i = sentPair.first->getLastCheckedSent(); i < sentPair.second; i++) {
			int ok = 0;
			Account* recipient = sentTransfers[i].getRecipient();
			std::vector<Transfer> receivedTransfers =recipient->getTransfersReceived();
			delta += sentTransfers[i].getMoney();
			int x = -1;
			lastMoney -= sentTransfers[i].getMoney();
			for (int k = 0; k < receivedSizes.size(); k++)
				if (receivedSizes[k].first->getId() == recipient->getId()) {
					x = k;
					break;
				}
			int receivedSize = receivedSizes[x].second;
			for (int j = receivedSizes[x].first->getLastCheckedRec(); j < receivedSize;j++) {
				if (receivedTransfers[j].getSerial() == sentTransfers[i].getSerial()) {
					ok = 1;
					delta -= receivedTransfers[j].getMoney();
					lastMoney += receivedTransfers[j].getMoney();
					break;
					
				}
			}
			if (!ok && lastMoney!=moneyNow[i].second) {
				std::cout << "Inconsistent Data\n"<<x<<'\n'<<receivedSizes[x].second<<'\n'<<s2-s1<<'\n';
				return;
			}
		}
		i++;
	}
	if (delta)
		std::cout << "Inconsistent data'\n";
	else
		std::cout << "Consistent Data\n"<<s2-s1<<'\n';
	for (auto pair : sentSizes) {
		//std::cout<<pair.first->getId() << " LastcheckedSent:" << pair.first->getLastCheckedSent() << " new:" << pair.second << '\n';
		pair.first->setLastCheckedSent(pair.second);
		
	}

	for (auto pair : receivedSizes) {
		//std::cout << pair.first->getId() << " LastcheckedRec:" << pair.first->getLastCheckedSent() << " new:" << pair.second << '\n';
		pair.first->setLastCheckedRec(pair.second);

	}
	i = 0;
	for (auto pair : money) {
		pair.first->setLastMoney(moneyNow[i].second);
		i++;
	}
	
}

void task(int ops,std::vector<Account*> accounts)
{
	std::default_random_engine e((int)std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> f(0,1000);
	for (int i = 0; i < ops; i++)
	{
		int x = f(e) % 10;
		int y = f(e) % 10;
		while (x == y) {
			y = f(e) % 10;
		}
		int z = f(e) % 1000;
		//std::cout << x << " " << y << '\n';
		bool result=transfer(accounts[x], accounts[y], z);
		/*if (result) {
			Transfer t{ accounts[x],accounts[y],z,true };
			accounts[x]->addTransferSent(t);
			accounts[y]->addTransferReceived(t);
		}*/
		//Sleep(1);
	}
}

void showMenu() {
	std::cout << "1.Check total money"<<'\n';
	std::cout << "2.Show logs for owner"<<'\n';
	std::cout << "0.Exit"<<'\n';
	std::cout << "Enter your option:";
}

void check(std::vector<Account*> accounts,bool* done) {
	while (!(*done)) {
		consistencyCheck(accounts);
		//Sleep(1);
	}
}

//void console(std::vector<Account*> accounts)
//{
//	bool end = false;
//	int x;
//	std::string owner;
//	Account* a = nullptr;
//	while (!end) {
//		showMenu();
//		std::cin >> x;
//		switch (x) {
//		case 1:
//			std::cout<<"Total money in bank:"<<checkMoney(accounts)<<'\n';
//			break;
//		case 2:
//			std::cout << "\nEnter account owner:";
//			std::cin >> owner;
//			std::cout << owner;
//			for (auto account : accounts) {
//				if (account->getOwner() == owner) {
//					a = account;
//					std::cout << "gasit";
//					break;
//				}
//			}
//			checkTransfers(a);
//			break;
//		case 0:
//			end = true;
//			break;
//		default:
//			std::cout << "\nInvalid option!\n";
//		}
//	}
//}
