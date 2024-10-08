#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class Account {
public:
    int accNo;
    char accHolder[25];
    float balance;

    void addAcc();
    void displayAccount() const;
    void getAllAccounts() const;
    void updateAcc();
    void deleteAcc();
    bool isAccountExists(int) const;
};

class Customer : public Account {
public:
    void deposit();
    void withdraw();
    void viewStatement() const;
};

bool Account::isAccountExists(int id) const {
    ifstream checkFile("account.dat", ios::binary);
    Account a;
    while (checkFile.read((char*)&a, sizeof(a))) {
        if (id == a.accNo) {
            checkFile.close();
            return true;
        }
    }
    checkFile.close();
    return false;
}

void Account::addAcc() {
    cout << "\nEnter account holder name: ";
    cin.ignore();
    cin.getline(accHolder, 25);

    cout << "Enter account number: ";
    while (!(cin >> accNo)) {
        cout << "Enter number only: ";
        cin.clear();
        cin.ignore(123, '\n');
    }

    if (isAccountExists(accNo)) {
        cout << "Error: Account with number " << accNo << " already exists\n";
    } else {
        cout << "Enter initial balance: ";
        while (!(cin >> balance)) {
            cout << "Enter number only: ";
            cin.clear();
            cin.ignore(123, '\n');
        }

        ofstream file("account.dat", ios::binary | ios::app);
        if (file) {
            file.write((char*)this, sizeof(*this));
            file.close();
            cout << "\nAccount created successfully...\n";
        } else {
            cout << "Error: Could not open file to save account data.\n";
        }
    }
}

void Account::displayAccount() const {
    cout << setw(5) << accNo << setw(25) << accHolder << setw(25) << balance << endl;
}

void Account::getAllAccounts() const {
    ifstream file("account.dat", ios::binary);
    if (!file) {
        cerr << "Error: Could not open file.\n";
        return;
    }

    Account a;
    bool found = false; // To check if any account exists
    while (file.read((char*)&a, sizeof(a))) {
        found = true;
        a.displayAccount();
    }

    if (!found) {
        cout << "There is no account.\n";
    }

    file.close();
}

void Account::updateAcc() {
    int id, pos;
    bool found = false;
    cout << "\nEnter account number to update: ";
    cin >> id;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    Account a;
    while (file.read((char*)&a, sizeof(a))) {
        if (a.accNo == id) {
            cout << "Enter new account holder name: ";
            cin.ignore();
            cin.getline(a.accHolder, 25);
            cout << "Enter new balance: ";
            while (!(cin >> a.balance)) {
                cout << "Enter number only: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            pos = -1 * static_cast<int>(sizeof(a));
            file.seekp(pos, ios::cur);
            file.write((char*)&a, sizeof(a));
            found = true;
            cout << "\nAccount updated successfully...\n";
            break;
        }
    }
    if (!found) {
        cout << "Invalid Account Number\n";
    }
    file.close();
}

void Account::deleteAcc() {
    int id;
    bool found = false;
    cout << "\nEnter account number to delete: ";
    cin >> id;
    ifstream file("account.dat", ios::binary);
    ofstream tempFile("temp.dat", ios::binary);
    Account a;
    while (file.read((char*)&a, sizeof(a))) {
        if (a.accNo != id) {
            tempFile.write((char*)&a, sizeof(a));
        } else {
            found = true;
        }
    }
    file.close();
    tempFile.close();
    remove("account.dat");
    rename("temp.dat", "account.dat");
    if (found) {
        cout << "\nAccount deleted successfully...\n";
    } else {
        cout << "Invalid Account Number\n";
    }
}

void Customer::deposit() {
    int id;
    bool found = false;
    cout << "Enter account number: ";
    cin >> id;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    Account a;
    while (file.read((char*)&a, sizeof(a))) {
        if (a.accNo == id) {
            found = true;
            float amount;
            cout << "Enter amount to deposit: ";
            while (!(cin >> amount)) {
                cout << "Enter number only: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            a.balance += amount;
            int pos = -1 * static_cast<int>(sizeof(a));
            file.seekp(pos, ios::cur);
            file.write((char*)&a, sizeof(a));
            cout << "Deposit successful. New balance: " << a.balance << endl;
            break;
        }
    }
    if (!found) {
        cout << "Invalid Account Number\n";
    }
    file.close();
}

void Customer::withdraw() {
    int id;
    bool found = false;
    cout << "Enter account number: ";
    cin >> id;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    Account a;
    while (file.read((char*)&a, sizeof(a))) {
        if (a.accNo == id) {
            found = true;
            float amount;
            cout << "Enter amount to withdraw: ";
            while (!(cin >> amount)) {
                cout << "Enter number only: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            if (a.balance >= amount) {
                a.balance -= amount;
                int pos = -1 * static_cast<int>(sizeof(a));
                file.seekp(pos, ios::cur);
                file.write((char*)&a, sizeof(a));
                cout << "Withdrawal successful. New balance: " << a.balance << endl;
            } else {
                cout << "Insufficient balance\n";
            }
            break;
        }
    }
    if (!found) {
        cout << "Invalid Account Number\n";
    }
    file.close();
}

void Customer::viewStatement() const {
    int id;
    bool found = false;
    cout << "Enter account number: ";
    cin >> id;
    ifstream file("account.dat", ios::binary);
    Account a;
    while (file.read((char*)&a, sizeof(a))) {
        if (a.accNo == id) {
            found = true;
            cout << "Account Statement:\n";
            cout << "Account Number: " << a.accNo << endl;
            cout << "Account Holder: " << a.accHolder << endl;
            cout << "Balance: " << a.balance << endl;
            break;
        }
    }
    if (!found) {
        cout << "Invalid Account Number\n";
    }
    file.close();
}

void adminMenu() {
    int choice;
    Account account;
    do {
        cout << "\n***********|| ADMIN MENU ||***********\n";
        cout << "1. Create Account\n";
        cout << "2. Update Account\n";
        cout << "3. Delete Account\n";
        cout << "4. View All Accounts\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                account.addAcc();
                break;
            case 2:
                account.updateAcc();
                break;
            case 3:
                account.deleteAcc();
                break;
            case 4:
                account.getAllAccounts();
                break;
            case 5:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice\n";
        }
    } while (choice != 5);
}

void customerMenu() {
    int choice;
    Customer customer;
    do {
        cout << "\n***********|| CUSTOMER MENU ||***********\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. View Statement\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                customer.deposit();
                break;
            case 2:
                customer.withdraw();
                break;
            case 3:
                customer.viewStatement();
                break;
            case 4:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice\n";
        }
    } while (choice != 4);
}

void login() {
    int choice;
    string password;
    do {
        cout << "\n***********|| BANK MANAGEMENT SYSTEM ||***********\n";
        cout << "1. Admin Login\n";
        cout << "2. Customer Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: // Admin Login
                cout << "Enter Admin Password: ";
                cin >> password;
                if (password == "dheeraj167") {
                    adminMenu();
                } else {
                    cout << "Invalid password\n";
                }
                break;
            case 2: // Customer Login
                customerMenu();
                break;
            case 3: // Exit
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice\n";
        }
    } while (choice != 3);
}

int main() {
    login(); // Start with the login screen
    return 0;
}
