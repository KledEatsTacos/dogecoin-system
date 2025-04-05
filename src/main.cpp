#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
using namespace std;

const int maxAccounts = 100;

class Account {
private:
    int accountNo;
    double balance;
    double dogecoin;
    bool isGeneralAccount;
    int lastUpdatedDay;
    double dogecoinValue;
    int lastChangeDay;
    int lastChangeMonth;
    int lastChangeYear;
    bool firstPurchaseMade;

public:
    Account() : dogecoin(0), dogecoinValue(0), lastUpdatedDay(0), firstPurchaseMade(false) {}
    
    Account(int accountNo, double balance, bool isGeneralAccount) 
        : accountNo(accountNo), balance(balance), isGeneralAccount(isGeneralAccount),
          dogecoin(0), dogecoinValue(0), firstPurchaseMade(false) {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        if (localTime != nullptr) {
            lastChangeDay = localTime->tm_mday;
            lastChangeMonth = localTime->tm_mon + 1;
            lastChangeYear = localTime->tm_year + 1900;
            lastUpdatedDay = lastChangeDay;
        }
    }

    void setLastChangeDate(int day, int month, int year) {
        lastChangeDay = day;
        lastChangeMonth = month;
        lastChangeYear = year;
    }

    int daysInMonth(int month, int year) {
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                return 29;
            }
            return 28;
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11) {
            return 30;
        }
        return 31;
    }

    int calculateDaysSinceLastChange(int currentDay, int currentMonth, int currentYear) {
        int daysPassed = 0;
        while (!(currentYear == lastChangeYear && currentMonth == lastChangeMonth && currentDay == lastChangeDay)) {
            int daysInCurrentMonth = daysInMonth(lastChangeMonth, lastChangeYear);
            daysPassed++;
            lastChangeDay++;

            if (lastChangeDay > daysInCurrentMonth) {
                lastChangeDay = 1;
                lastChangeMonth++;

                if (lastChangeMonth > 12) {
                    lastChangeMonth = 1;
                    lastChangeYear++;
                }
            }

            currentDay = lastChangeDay;
            currentMonth = lastChangeMonth;
            currentYear = lastChangeYear;
        }
        return daysPassed;
    }

    void updateGeneralAccount(int currentDay) {
        int daysPassed = currentDay - lastUpdatedDay;
        double increaseRate = 0.01;
        double decreaseRate = 0.05;

        if (daysPassed >= 15) {
            int updateCount = daysPassed / 15;
            for (int i = 0; i < updateCount; ++i) {
                dogecoinValue -= (dogecoinValue * decreaseRate);
                lastUpdatedDay += 15;
            }
        }
        else {
            dogecoinValue += (dogecoinValue * increaseRate * daysPassed);
            lastUpdatedDay = currentDay;
        }
    }

    void updateSpecialAccount(int currentDay) {
        int daysPassed = currentDay - lastUpdatedDay;
        double increaseRate = 0.05;
        double decreaseRate = 0.1;
        int randomDays[3] = { 5, 15, 25 };

        if (daysPassed >= 30) {
            int updateCount = daysPassed / 30;
            for (int i = 0; i < updateCount; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (lastUpdatedDay + 30 == randomDays[j]) {
                        dogecoinValue -= (dogecoinValue * decreaseRate);
                        lastUpdatedDay += 30;
                        break;
                    }
                }
            }
        }
        else {
            dogecoinValue += (dogecoinValue * increaseRate * daysPassed);
            lastUpdatedDay = currentDay;
        }
    }

    void firstPurchase(double investedMoney) {
        if (dogecoin == 0) {
            dogecoin = investedMoney;
            dogecoinValue = investedMoney;
            balance -= investedMoney;
            cout << investedMoney << " TL worth of " << dogecoin << " Dogecoin added to account " << accountNo << endl;
        }
        else {
            double additionalDogecoin = investedMoney;
            dogecoin += additionalDogecoin;
            dogecoinValue += investedMoney;
            balance -= investedMoney;
            cout << investedMoney << " TL worth of " << additionalDogecoin << " Dogecoin added to account " << accountNo << endl;
        }
        firstPurchaseMade = true;
    }

    bool accountExists(Account accounts[], int accountCount, int accountNo) {
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].accountNo == accountNo) {
                return true;
            }
        }
        return false;
    }

    bool checkAccountType() { return isGeneralAccount; }
    int getAccountNumber() { return accountNo; }
    double getDogecoinBalance() { return dogecoin; }
    double getBalance() { return balance; }
    void setDogecoinValue(double value) { dogecoin = value; }

    void addToAccountList(Account accounts[], int& accountCount) {
        accounts[accountCount] = *this;
        accountCount++;
    }

    void list() {
        cout << "Account number: " << accountNo << endl
             << "Balance: " << balance << endl
             << "Dogecoin: " << dogecoin << endl
             << "Account type: " << (isGeneralAccount ? "General" : "Special") << endl;
    }

    void deposit(double amount) { balance += amount; }
    void withdraw(double amount) { balance -= amount; }
    void checkBalance() { cout << "Your balance: " << balance << endl; }

    double profit(int currentDay, int currentMonth, int currentYear) {
        int daysPassed = calculateDaysSinceLastChange(currentDay, currentMonth, currentYear);
        double initialDogecoinValue = 0;
        
        if (firstPurchaseMade) {
            initialDogecoinValue = dogecoin / (1 + (isGeneralAccount ? 0.01 : 0.05) * daysPassed);
        }

        return dogecoinValue - initialDogecoinValue;
    }
};

void getCurrentDate(int& day, int& month, int& year) {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    if (localTime != nullptr) {
        day = localTime->tm_mday;
        month = localTime->tm_mon + 1;
        year = localTime->tm_year + 1900;
    }
    else {
        cout << "Error: Could not get current date" << endl;
    }
}

void advanceDate(int& day, int& month, int& year, int daysToAdd) {
    const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    while (daysToAdd > 0) {
        int currentMonthDays = daysInMonth[month - 1];
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
            currentMonthDays = 29;
        }

        if (day + daysToAdd > currentMonthDays) {
            daysToAdd -= (currentMonthDays - day + 1);
            day = 1;
            if (++month > 12) {
                month = 1;
                ++year;
            }
        }
        else {
            day += daysToAdd;
            daysToAdd = 0;
        }
    }
}

int main() {
    srand(time(0));
    int day, month, year;
    getCurrentDate(day, month, year);

    Account accounts[maxAccounts];
    int accountCount = 0;
    int choice;

    do {
        cout << "What would you like to do?: " << endl
             << "\t 1. Open General Account" << endl
             << "\t 2. Open Special Account" << endl
             << "\t 3. List Accounts" << endl
             << "\t 4. Deposit Money" << endl
             << "\t 5. Withdraw Money" << endl
             << "\t 6. Buy Dogecoin" << endl
             << "\t 7. Sell Dogecoin" << endl
             << "\t 8. Check Date" << endl
             << "\t 9. Advance Time" << endl
             << "\t 10. Calculate Profit/Loss" << endl
             << "\t 11. Exit Program" << endl;
        cin >> choice;

        switch (choice) {
        case 1: {
            int accountNo;
            double amount;
            cout << "Enter account number: "; cin >> accountNo;
            cout << "Enter amount: "; cin >> amount;

            if (accounts[0].accountExists(accounts, accountCount, accountNo))
                cout << "An account with this number already exists!" << endl;
            else {
                Account newGeneral(accountNo, amount, true);
                newGeneral.addToAccountList(accounts, accountCount);
            }
            break;
        }
        case 2: {
            int accountNo;
            double amount;
            cout << "Enter account number: "; cin >> accountNo;
            cout << "Enter amount: "; cin >> amount;

            if (accounts[0].accountExists(accounts, accountCount, accountNo))
                cout << "An account with this number already exists!" << endl;
            else {
                Account newSpecial(accountNo, amount, false);
                newSpecial.addToAccountList(accounts, accountCount);
            }
            break;
        }
        case 3: {
            if (accountCount == 0)
                cout << "No accounts found!" << endl;
            else {
                cout << "Accounts: " << endl;
                for (int i = 0; i < accountCount; i++) {
                    accounts[i].list();
                    cout << endl;
                }
            }
            break;
        }
        case 4: {
            int accountNo;
            double amount;
            bool accountFound = false;

            cout << "Enter account number: "; cin >> accountNo;
            cout << "Enter amount to deposit: "; cin >> amount;
            
            for (int i = 0; i < accountCount; i++) {
                if (accounts[i].getAccountNumber() == accountNo) {
                    accounts[i].deposit(amount);
                    cout << amount << " TL deposited to account " << accountNo << endl;
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound) cout << "Account not found!" << endl;
            break;
        }
        case 5: {
            int accountNo;
            double amount;
            bool accountFound = false;
            
            cout << "Enter account number: "; cin >> accountNo;
            cout << "Enter amount to withdraw: "; cin >> amount;
            
            for (int i = 0; i < accountCount; i++) {
                if (accounts[i].getAccountNumber() == accountNo) {
                    accounts[i].withdraw(amount);
                    cout << amount << " TL withdrawn from account " << accountNo << endl;
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound) cout << "Account not found!" << endl;
            break;
        }
        case 6: {
            int accountNo;
            double investAmount;
            bool accountFound = false;

            cout << "Enter account number: "; cin >> accountNo;
            cout << "Enter amount to invest: "; cin >> investAmount;

            for (int i = 0; i < accountCount; i++) {
                if (accounts[i].getAccountNumber() == accountNo) {
                    getCurrentDate(day, month, year);
                    bool isGeneral = accounts[i].checkAccountType();
                    
                    if (isGeneral) accounts[i].updateGeneralAccount(day);
                    else accounts[i].updateSpecialAccount(day);

                    accounts[i].firstPurchase(investAmount);
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound) cout << "Account not found!" << endl;
            break;
        }
        case 7: {
            int accountNo;
            double amount;
            bool accountFound = false;

            cout << "Enter account number: "; cin >> accountNo;
            cout << "Enter amount of Dogecoin to sell: "; cin >> amount;

            for (int i = 0; i < accountCount; i++) {
                if (accounts[i].getAccountNumber() == accountNo) {
                    getCurrentDate(day, month, year);
                    bool isGeneral = accounts[i].checkAccountType();

                    if (isGeneral) accounts[i].updateGeneralAccount(day);
                    else accounts[i].updateSpecialAccount(day);

                    double currentDogecoinBalance = accounts[i].getDogecoinBalance();

                    if (currentDogecoinBalance >= amount) {
                        accounts[i].setDogecoinValue(currentDogecoinBalance - amount);
                        accounts[i].deposit(amount);
                        cout << amount << " Dogecoin sold from account " << accountNo << endl;
                    }
                    else {
                        cout << "Insufficient Dogecoin balance!" << endl;
                    }
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound) cout << "Account not found!" << endl;
            break;
        }
        case 8: {
            cout << "Date: " << setw(2) << setfill('0') << day << "/" 
                 << setw(2) << setfill('0') << month << "/" << year << endl;
            break;
        }
        case 9: {
            int daysToAdvance;
            cout << "Enter number of days to advance: "; cin >> daysToAdvance;
            advanceDate(day, month, year, daysToAdvance);

            for (int i = 0; i < accountCount; i++) {
                getCurrentDate(day, month, year);
                if (accounts[i].checkAccountType()) accounts[i].updateGeneralAccount(day);
                else accounts[i].updateSpecialAccount(day);
            }
            break;
        }
        case 10: {
            int accountNo;
            bool accountFound = false;

            cout << "Enter account number: "; cin >> accountNo;

            for (int i = 0; i < accountCount; i++) {
                if (accounts[i].getAccountNumber() == accountNo) {
                    getCurrentDate(day, month, year);
                    double accountProfit = accounts[i].profit(day, month, year);
                    cout << "Profit/Loss for account " << accountNo << ": " << accountProfit << endl;
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound) cout << "Account not found!" << endl;
            break;
        }
        case 11: {
            cout << "Exiting program..." << endl;
            break;
        }
        }
    } while (choice != 11);
    
    system("pause");
    return 0;
}