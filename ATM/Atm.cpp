#include <iostream>
#include <unordered_map>
#include "Atm.h"

bool Atm::withdrawBills(int amount, std::unordered_map<int, int>& withdrawnBills)
{
    int bills[] = { 500, 200, 100, 50, 20, 10 };

    for (int bill : bills) {
        int amountOfBills = std::min(amount / bill, this->availableBills[bill]);
        if (amountOfBills > 0) {
            withdrawnBills[bill] = amountOfBills;
            this->availableBills[bill] -= amountOfBills;
            amount -= amountOfBills * bill;
        }
    }

    return amount == 0;
}

std::unordered_map<int, int> Atm::withdrawCash(int amount)
{
    std::unordered_map<int, int> withdrawnBills;
    if (withdrawBills(amount, withdrawnBills)) {
        
        return withdrawnBills;
    }
    else {
        return std::unordered_map<int, int>();
    }
}

void Atm::depositCash(const std::unordered_map<int, int>& depositedBills)
{
    for (const auto& pair : depositedBills) {
        this->availableBills[pair.first] += pair.second;
    }
}

void Atm::setAvailableBills(const std::unordered_map<int, int>& availableBills)
{
    this->availableBills = availableBills;
}

std::unordered_map<int, int> Atm::getAvailableBills() const
{
    return this->availableBills;
}
