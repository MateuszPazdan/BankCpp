#pragma once
#include <string>
#include <vector>
#include "Transaction.h"

class Account {
    private:
        std::string login;
        std::string password;
        double balance;
    public:
        Account(std::string& login, double initialBalance,  std::string& password);

        std::vector<Account> returnVectorOfAccounts();
        void writeAccountToFile();
        bool returnAccount(std::string login, std::string password);
        bool createAccount();
        bool updateBalance(double newBalance);
        std::vector<Transaction> returnVectorOfTransactions();
        bool writeTransactionToFile(std::string type, double amount);

        std::string getLogin();
        std::string getPassword();
        double getBalance();
        void setLogin(std::string login);
        void setPassword(std::string password);
        void setBalance(double balance);
};
