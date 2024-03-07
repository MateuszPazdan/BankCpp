#include "Transaction.h"



Transaction::Transaction(std::string& type, double amount)
{
	this->type = type;
	this->amount = amount;
}

bool Transaction::makeNewTransaction(std::string login, double amount, std::string type)
{
	char* err;
	sqlite3* db;
	sqlite3_stmt* stmt;
	sqlite3_open("bankDB.db", &db);

	std::string query = "CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, type TEXT, amount FLOAT, FOREIGN KEY (user_id) REFERENCES accounts(id))";
	int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
	if (rc != SQLITE_OK) {
		return false;
	}

	if (amount != 0) {
		query = "INSERT INTO transactions (user_id, type, amount) VALUES ((SELECT id FROM accounts WHERE name = '" + login + "'), 'wplata', " + std::to_string(amount) + ")";
		int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
		if (rc != SQLITE_OK) {
			return false;
		}
	}
	return true;
}

std::string Transaction::getType()
{
	return this->type;
}

double Transaction::getAmount()
{
	return this->amount;
}

