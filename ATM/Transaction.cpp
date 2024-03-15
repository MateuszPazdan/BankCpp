#include "Transaction.h"

Transaction::Transaction(std::string& type, double amount)
{
	this->type = type;
	this->amount = amount;
}

bool Transaction::makeNewTransaction(std::string login, double amount, std::string type)
{
  
    sqlite3* db;
    sqlite3_open("bankDB.db", &db);

    std::string query = "CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, type TEXT, amount FLOAT);";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        return false;
    }

    if (amount != 0) {
        query = "INSERT INTO transactions (user_id, type, amount) VALUES ((SELECT id FROM accounts WHERE name = '" + login + "'), '" + type + "', " + std::to_string(amount) + ")";
        int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            return false;
        }
    }

    return true;
}

bool Transaction::deleteUserTransactions(const std::string login)
{
    sqlite3* db;
    sqlite3_open("bankDB.db", &db);

    // Tworzymy zapytanie SQL usuwaj¹ce transakcje u¿ytkownika o danym loginie
    std::string query = "DELETE FROM transactions WHERE user_id = (SELECT id FROM accounts WHERE name = '" + login + "')";

    // Wykonujemy zapytanie
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        return false;
    }

    // Zamykamy po³¹czenie z baz¹ danych
    sqlite3_close(db);

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

