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

bool Atm::withdrawCash(int amount)
{
    std::unordered_map<int, int> withdrawnBills;
    if (withdrawBills(amount, withdrawnBills)) {
        std::cout << "Wydane banknoty:" << std::endl;
        for (const auto& para : withdrawnBills) {
            std::cout << para.second << " x " << para.first << std::endl;
        }
        return true;
    }
    else {
        return false;
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
