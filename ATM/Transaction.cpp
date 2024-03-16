#include "Transaction.h" 

// Konstruktor klasy Transaction
Transaction::Transaction(std::string& type, double amount)
{
    this->type = type;     
    this->amount = amount;  
}

// Metoda klasy Transaction do tworzenia nowej transakcji
bool Transaction::makeNewTransaction(std::string login, double amount, std::string type)
{
    sqlite3* db;
    sqlite3_open("bankDB.db", &db); // Otwarcie po³¹czenia z baz¹ danych

    // Tworzenie tabeli 'transactions' jeœli nie istnieje
    std::string query = "CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, type TEXT, amount FLOAT);";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        return false; 
    }

    // Jeœli kwota jest ró¿na od zera, dodajemy now¹ transakcjê do bazy danych
    if (amount != 0) {
        query = "INSERT INTO transactions (user_id, type, amount) VALUES ((SELECT id FROM accounts WHERE name = '" + login + "'), '" + type + "', " + std::to_string(amount) + ")";
        int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            return false; 
        }
    }

    sqlite3_close(db); // Zamykanie po³¹czenia z baz¹ danych
    return true; 
}

// Metoda klasy Transaction do usuwania transakcji u¿ytkownika
bool Transaction::deleteUserTransactions(const std::string login)
{
    sqlite3* db;
    sqlite3_open("bankDB.db", &db); // Otwarcie po³¹czenia z baz¹ danych

    // Tworzenie zapytania SQL usuwaj¹cego transakcje u¿ytkownika o danym loginie
    std::string query = "DELETE FROM transactions WHERE user_id = (SELECT id FROM accounts WHERE name = '" + login + "')";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        return false; // Zwracamy false w przypadku b³êdu
    }

    sqlite3_close(db); // Zamykanie po³¹czenia z baz¹ danych
    return true; // Zwracamy true w przypadku poprawnego wykonania operacji
}

// Metoda zwracaj¹ca typ transakcji
std::string Transaction::getType()
{
    return this->type; 
}

// Metoda zwracaj¹ca kwotê transakcji
double Transaction::getAmount()
{
    return this->amount; 
}
