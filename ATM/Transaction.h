#pragma once
#include <string>
#include "sqlite/sqlite3.h"

class Transaction
{
private:
	std::string type;
	double amount;
public:
	Transaction() = default;
	Transaction(std::string& type, double amount);

	bool makeNewTransaction(const std::string login, const double amount, const std::string type);
	
	std::string getType();
	double getAmount();

};

