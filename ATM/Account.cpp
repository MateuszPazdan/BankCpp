#include "Account.h"

// Konstruktor klasy Account
Account::Account(std::string& login, double initialBalance, std::string& password)
{
    this->login = login;         
    this->balance = initialBalance; 
    this->password = password;    
    this->role = "user";          
}

// Konstruktor klasy Account
Account::Account(int id, std::string& login, double initialBalance, std::string& password, std::string& role)
{
    this->id = id;                
    this->login = login;          
    this->balance = initialBalance;
    this->password = password;   
    this->role = role;         
}

// Metoda klasy Account do tworzenia nowego konta w bazie danych
bool Account::createAccount()
{
    sqlite3* db;
    sqlite3_open("bankDB.db", &db); // Otwarcie po³¹czenia z baz¹ danych

    // Tworzenie tabeli 'accounts' jeœli nie istnieje
    int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE, password TEXT, role TEXT, balance FLOAT)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        return false; 
    }
    else {
        // Dodanie nowego rekordu do tabeli 'accounts'
        std::string query = "INSERT INTO accounts (name, password, role, balance) VALUES ('" + this->login + "', '" + this->password + "', '" + this->role + "', " + std::to_string(this->balance) + ")";
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            return false; 
        }

        // Dodanie transakcji "wplata" do tabeli 'transactions'
        Transaction transaction;
        transaction.makeNewTransaction(this->login, this->balance, "wplata");
    }

    sqlite3_close(db); // Zamykanie po³¹czenia z baz¹ danych
    return true; 
}

// Metoda klasy Account zwracaj¹ca wektor wszystkich kont z bazy danych
std::vector<Account> Account::returnVectorOfAccounts()
{
    std::vector<Account> accounts;
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_open("bankDB.db", &db); // Otwarcie po³¹czenia z baz¹ danych

    // Przygotowanie zapytania SQL do zwracania wszystkich u¿ytkowników
    int rc = sqlite3_prepare_v2(db, "SELECT id, name, password, role, balance FROM accounts", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return accounts; // Zwracamy pusty wektor w przypadku b³êdu
    }

    // Pobieranie danych z bazy danych
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        std::string loginString(reinterpret_cast<const char*>(name));

        const unsigned char* password = sqlite3_column_text(stmt, 2);
        std::string passwordString(reinterpret_cast<const char*>(password));

        const unsigned char* role = sqlite3_column_text(stmt, 3);
        std::string roleString(reinterpret_cast<const char*>(role));

        double balance = sqlite3_column_double(stmt, 4);

        // Tworzenie obiektu Account i dodawanie go do wektora
        Account account(id, loginString, balance, passwordString, roleString);
        accounts.push_back(account);
    }

    sqlite3_finalize(stmt); // Finalizacja zapytania
    return accounts; // Zwracamy wektor kont
}

// Metoda klasy Account sprawdzaj¹ca poprawnoœæ danych logowania
bool Account::returnAccount(std::string login, std::string password)
{
    std::vector<Account> accounts = returnVectorOfAccounts(); // Pobranie wszystkich kont z bazy danych

    for (Account& account : accounts) {
        // Sprawdzenie czy dane logowania siê zgadzaj¹
        if (account.getLogin() == login && account.getPassword() == password) {
            // Przypisanie danych zgodnego konta do bie¿¹cego obiektu
            this->id = account.getId();
            this->login = account.getLogin();
            this->password = account.getPassword();
            this->role = account.getRole();
            this->balance = account.getBalance();
            return true; 
        }
    }

    return false; // Zwracamy false jeœli nie znaleziono pasuj¹cego konta
}

// Metoda klasy Account zwracaj¹ca wektor wszystkich transakcji zwi¹zanych z danym kontem
std::vector<Transaction> Account::returnVectorOfTransactions()
{
    std::vector<Transaction> transactions;
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_open("bankDB.db", &db); // Otwarcie po³¹czenia z baz¹ danych

    // Przygotowanie zapytania SQL
    int rc = sqlite3_prepare_v2(db, "SELECT * FROM transactions WHERE user_id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, this->id); // Ustawienie wartoœci parametru w zapytaniu
    if (rc != SQLITE_OK) {
        return transactions; // Zwracamy pusty wektor w przypadku b³êdu
    }

    // Pobieranie danych z bazy danych
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* typeOfTransaction = sqlite3_column_text(stmt, 2);
        std::string typeString(reinterpret_cast<const char*>(typeOfTransaction));

        double balance = sqlite3_column_double(stmt, 3);

        // Tworzenie obiektu Transaction i dodawanie go do wektora
        Transaction transaction(typeString, balance);
        transactions.push_back(transaction);
    }

    sqlite3_finalize(stmt); 
    return transactions; // Zwracamy wektor transakcji
}

// Metoda klasy Account do zapisywania transakcji do bazy danych
bool Account::writeTransactionToDB(std::string type, double amount)
{
    Transaction transaction;
    if (transaction.makeNewTransaction(this->login, amount, type)) return true; // Wywo³anie metody makeNewTransaction i zwrócenie wyniku
    return false; // Zwracamy false w przypadku b³êdu
}

// Metoda klasy Account do usuwania konta z bazy danych
bool Account::deleteAccount(std::string login)
{
    Transaction transaction;
    transaction.deleteUserTransactions(login); // Usuniêcie transakcji powi¹zanych z kontem
    sqlite3* db;
    sqlite3_stmt* stmt;
    if (sqlite3_open("bankDB.db", &db) != SQLITE_OK) {
        return false; 
    }

    const char* query = "DELETE FROM accounts WHERE name = ?"; // Zapytanie SQL do usuwania konta
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false; 
    }

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC); // Ustawienie wartoœci parametru w zapytaniu
    rc = sqlite3_step(stmt); // Wykonanie zapytania
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false; 
    }

    sqlite3_finalize(stmt); 
    sqlite3_close(db); // Zamykanie po³¹czenia z baz¹ danych

    return true; 
}

// Metoda klasy Account do edycji danych u¿ytkownika
bool Account::editUser(std::string login, std::string newLogin, double newBalance)
{
    char* err;
    sqlite3* db;
    sqlite3_open("bankDB.db", &db); // Otwarcie po³¹czenia z baz¹ danych

    // Aktualizacja danych u¿ytkownika w tabeli 'accounts'
    std::string query = "UPDATE accounts SET name = '" + newLogin + "', balance = " + std::to_string(newBalance) + " WHERE name = '" + login + "'";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        return false; 
    }

    // Aktualizacja danych u¿ytkownika w tabeli 'transactions'
    query = "UPDATE transactions SET user_id = (SELECT id FROM accounts WHERE name = '" + newLogin + "') WHERE user_id = (SELECT id FROM accounts WHERE name = '" + login + "')";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        return false; 
    }

    sqlite3_close(db); // Zamykanie po³¹czenia z baz¹ danych

    return true; // Zwracamy true jeœli operacja zakoñczy³a siê sukcesem
}

// Metoda klasy Account do aktualizacji salda u¿ytkownika
bool Account::updateBalance(double newBalance)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_open("bankDB.db", &db); // Otwarcie po³¹czenia z baz¹ danych
    const char* query = "UPDATE accounts SET balance = ? WHERE id = ?"; // Zapytanie SQL do aktualizacji salda
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false; 
    }

    sqlite3_bind_double(stmt, 1, newBalance); // Ustawienie wartoœci parametrów w zapytaniu
    sqlite3_bind_int(stmt, 2, this->id);

    rc = sqlite3_step(stmt); // Wykonanie zapytania
    if (rc != SQLITE_DONE) {
        return false;
    }
    this->balance = newBalance; // Aktualizacja salda w obiekcie

    sqlite3_finalize(stmt); 
    return true; 
}

// Metoda zwracaj¹ca ID konta
int Account::getId()
{
    return this->id; 
}

// Metoda zwracaj¹ca login konta
std::string Account::getLogin()
{
    return this->login; 
}

// Metoda zwracaj¹ca has³o konta
std::string Account::getPassword()
{
    return this->password; 
}

// Metoda zwracaj¹ca saldo konta
double Account::getBalance()
{
    return this->balance; 
}

// Metoda zwracaj¹ca rolê konta
std::string Account::getRole()
{
    return this->role; 
}

// Metoda ustawiaj¹ca ID konta
void Account::setId(int id)
{
    this->id = id;
}

// Metoda ustawiaj¹ca login konta
void Account::setLogin(std::string login)
{
    this->login = login; 
}

// Metoda ustawiaj¹ca has³o konta
void Account::setPassword(std::string password)
{
    this->password = password; 
}

// Metoda ustawiaj¹ca saldo konta
void Account::setBalance(double balance)
{
    this->balance = balance; 
}

// Metoda ustawiaj¹ca rolê konta
void Account::setRole(std::string role)
{
    this->role = role; 
}
