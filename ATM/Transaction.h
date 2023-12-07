#pragma once
#include <string>
class Transaction
{
private:
	std::string type;
	double amount;
public:
	Transaction(std::string& type, double amount);

	std::string getType();
	double getAmount();
};

