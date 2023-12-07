#include "Transaction.h"

Transaction::Transaction(std::string& type, double amount)
{
	this->type = type;
	this->amount = amount;
}

std::string Transaction::getType()
{
	return this->type;
}

double Transaction::getAmount()
{
	return this->amount;
}

