#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "Account.h"
#include "Transaction.h"
#include "sqlite/sqlite3.h"

Account::Account(std::string& login, double initialBalance, std::string& password)
{
	this->login = login;
	this->balance = initialBalance;
	this->password = password;
	this->role = "user";
}

Account::Account(int id, std::string& login, double initialBalance, std::string& password, std::string& role)
{
	this->id = id;
	this->login = login;
	this->balance = initialBalance;
	this->password = password;
	this->role = role;
}

bool Account::createAccount()
{
	char* err;
	sqlite3* db;
	sqlite3_open("bankDB.db", &db);

	int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE, password TEXT, role TEXT, balance FLOAT)", NULL, NULL, &err);
	if (rc != SQLITE_OK) {
		return false;
	}
	else {
		std::string query = "INSERT INTO accounts (name, password, role, balance) VALUES ('" + this->login + "', '" + this->password + "', '" + this->role + "', " + std::to_string(this->balance) + ")";
		int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
		if (rc != SQLITE_OK) {
			return false;
		}

		Transaction transaction;
		transaction.makeNewTransaction(this->login, this->balance, "wplata");
	}
	
	return true;
}

std::vector<Account> Account::returnVectorOfAccounts()
{
	std::vector<Account> accounts;
	char* err;
	sqlite3* db;
	sqlite3_stmt* stmt;
	sqlite3_open("bankDB.db", &db);

	int rc = sqlite3_prepare_v2(db, "SELECT id, name, password, role, balance FROM accounts", -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		return accounts;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		const unsigned char* name = sqlite3_column_text(stmt, 1);
		std::string loginString(reinterpret_cast<const char*>(name));

		const unsigned char* password = sqlite3_column_text(stmt, 2);
		std::string passwordString(reinterpret_cast<const char*>(password));

		const unsigned char* role = sqlite3_column_text(stmt, 3);
		std::string roleString(reinterpret_cast<const char*>(role));

		double balance = sqlite3_column_double(stmt, 4);

		Account account(id, loginString, balance, passwordString, roleString);
		accounts.push_back(account);
	}

	sqlite3_finalize(stmt);
	return accounts;

}


bool Account::returnAccount(std::string login, std::string password)
{
	std::vector<Account> accounts = returnVectorOfAccounts();

	for ( Account& account : accounts) {
		if (account.getLogin() == login && account.getPassword() == password) { 
			this->id = account.getId();
			this->login = account.getLogin();
			this->password = account.getPassword();
			this->role = account.getRole();
			this->balance = account.getBalance();
			return true; 
		}
	}

	return false;
}

std::vector<Transaction> Account::returnVectorOfTransactions()
{
	std::vector<Transaction> transactions;

	char* err;
	sqlite3* db;
	sqlite3_stmt* stmt;
	sqlite3_open("bankDB.db", &db);

	int rc = sqlite3_prepare_v2(db, "SELECT * FROM transactions WHERE user_id = ?", -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, this->id);
	if (rc != SQLITE_OK) {
		return transactions;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {

		const unsigned char* typeOfTransaction = sqlite3_column_text(stmt, 2);
		std::string typeString(reinterpret_cast<const char*>(typeOfTransaction));

		double balance = sqlite3_column_double(stmt, 3);

		Transaction transaction(typeString, balance);
		transactions.push_back(transaction);
	}

	sqlite3_finalize(stmt);

	return transactions;
}

bool Account::writeTransactionToDB(std::string type, double amount)
{
	Transaction transaction;
	if (transaction.makeNewTransaction(this->login, amount, type)) return true;
	return false;
}

bool Account::deleteAccount(std::string login)
{
	Transaction transaction;
	transaction.deleteUserTransactions(login);
	sqlite3* db;
	sqlite3_stmt* stmt;
	if (sqlite3_open("bankDB.db", &db) != SQLITE_OK) {
		return false;
	}

	const char* query = "DELETE FROM accounts WHERE name = ?";
	int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return false;
	}

	sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return false;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return true;
}


bool Account::updateBalance(double newBalance)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	sqlite3_open("bankDB.db", &db);
	const char* query = "UPDATE accounts SET balance = ? WHERE id = ?";
	int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		return false;
	}

	sqlite3_bind_double(stmt, 1, newBalance); 
	sqlite3_bind_int(stmt, 2, this->id);       

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		return false;
	}
	this->balance = newBalance;

	sqlite3_finalize(stmt);
	return true;
}


int Account::getId()
{
	return this->id;
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

std::string Account::getRole()
{
	return this->role;
}

void Account::setId(int id)
{
	this->id = id;
}

void Account::setLogin(std::string login)
{
	this->login = login;
}

void Account::setPassword(std::string password)
{
	this->password = password;
}

void Account::setBalance(double balance)
{
	this->balance = balance;
}

void Account::setRole(std::string role)
{
	this->role = role;
}


