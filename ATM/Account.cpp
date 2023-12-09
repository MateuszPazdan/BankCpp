#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "Account.h"
#include "Transaction.h"

Account::Account(std::string& login, double initialBalance, std::string& password)
{
	this->login = login;
	this->balance = initialBalance;
	this->password = password;
}

std::vector<Account> Account::returnVectorOfAccounts()
{
	std::ifstream fileIn("accounts.txt");
	std::vector<Account> accounts;
	std::string log;
	std::string pass;
	
	if (fileIn.good()) {
		while (fileIn >> log >> pass) {
			Account account(log, 0, pass);
			accounts.push_back(account);
		}
			fileIn.close();
	}
	else {
		fileIn.close();
		std::ofstream fileOut("accounts.txt");
		fileOut.close();
	}
	return accounts;
}

void Account::writeAccountToFile()
{
	std::ofstream fileOut("accounts.txt", std::ios::app);
	std::ofstream balanceOut("users\\" + this->getLogin() + ".txt");
	balanceOut << this->getBalance() << "\n";
	if (this->getBalance() != 0) {
		balanceOut << "wplata " << this->getBalance() << '\n';
	}
	fileOut << this->getLogin() << " " << this->getPassword() << '\n';
	
	fileOut.close();
	balanceOut.close();
}


bool Account::createAccount()
{
	std::vector<Account> accounts = returnVectorOfAccounts();
	for (Account account : accounts) {
		if (account.getLogin() == this->getLogin()) {
			return false;
		}
	}

	writeAccountToFile();
	return true;
}

bool Account::returnAccount(std::string login, std::string password)
{
	std::vector<Account> accounts = returnVectorOfAccounts();
	for (Account account : accounts) {
		if (login == account.getLogin() && password== account.getPassword()) {
			std::ifstream balanceIn("users\\" + login + ".txt");
			double tempBalance;
			balanceIn >> tempBalance;
			this->setBalance(tempBalance);
			balanceIn.close();
			return true;
		}
	}
	return false;
}

std::vector<Transaction> Account::returnVectorOfTransactions()
{
	std::ifstream fileIn("users\\" + this->getLogin() + ".txt");
	std::vector<Transaction> transactions;
	std::string type;
	double value;
	double balance;
	fileIn >> balance;
	while (fileIn >> type >> value) {
		Transaction* transaction = new Transaction(type, value);
		transactions.push_back(*transaction);
		delete(transaction);
	}
	fileIn.close();
	return transactions;
}


bool Account::writeTransactionToFile(std::string type, double amount)
{
	std::ofstream file("users\\" + this->getLogin() + ".txt", std::ios::app);
	file << type << " " << amount << "\n";
	file.close();
	return true;
}

bool Account::updateBalance(double newBalance)
{
	std::ifstream file("users\\" + this->getLogin() + ".txt");
	if (file.is_open()) {
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line) ){
			lines.push_back(line);
		}
		if (!lines.empty()) {
			lines.erase(lines.begin());
			lines.insert(lines.begin(), std::to_string(newBalance));
		}
		file.close();
		std::ofstream file("users\\" + this->getLogin() + ".txt");
		for (std::string line : lines) {
			file << line << '\n';
		}
		file.close();
		this->setBalance(newBalance);
		return true;
	}
	else {
		return false;
	}
}



std::string Account::getLogin()
{
	return this->login;
}

std::string Account::getPassword()
{
	return this->password;
}

double Account::getBalance()
{
	return this->balance;
}

void Account::setBalance(double balance)
{
	this->balance = balance;
}

void Account::setLogin(std::string login)
{
	this->login = login;
}

void Account::setPassword(std::string password)
{
	this->password = password;
}



