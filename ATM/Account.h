#pragma once
#include <string>
#include <vector>
#include "Transaction.h"


class Account {
    private:
        int id;
        std::string login;
        std::string password;
        double balance;
        std::string role;
    public:
        Account(std::string& login, double initialBalance, std::string& password);
        Account(int id, std::string& login, double initialBalance, std::string& password, std::string& role);

        bool createAccount();
        std::vector<Account> returnVectorOfAccounts();
        bool returnAccount(std::string login, std::string password);
        std::vector<Transaction> returnVectorOfTransactions();
        bool updateBalance(double newBalance);
        bool writeTransactionToDB(std::string type, double amount);
        bool deleteAccount(std::string login);


        int getId();
        std::string getLogin();
        std::string getPassword();
        double getBalance();
        std::string getRole();
        void setId(int id);
        void setLogin(std::string login);
        void setPassword(std::string password);
        void setBalance(double balance);
        void setRole(std::string role);
};
