    #pragma once
    #include <iostream>
    #include <unordered_map>

    class Atm {
    private:
        std::unordered_map<int, int> availableBills;
        bool withdrawBills(int kwota, std::unordered_map<int, int>& wydaneBanknoty);
    public:
        Atm(std::unordered_map<int, int> banknoty) : availableBills(banknoty) {};
        std::unordered_map<int, int> withdrawCash(int kwota);
        void depositCash(const std::unordered_map<int, int>& depositedBills);
        void setAvailableBills(const std::unordered_map<int, int>& noweDostepneBanknoty);
        std::unordered_map<int, int> getAvailableBills() const;
    };
