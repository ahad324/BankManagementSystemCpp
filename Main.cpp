#include <iostream>      // Input/output operations
#include <sstream>       // String stream processing
#include <string>        // String manipulation
#include <vector>        // Dynamic array container
#include <fstream>       // File stream input/output operations
#include <cstdlib>       // For system("cls") to clear console
#include <conio.h>       // For _getch() to read a character from console
#include <regex>         // Regular expressions
#include <ctime>         // Date and time functions
#include <windows.h>     // Windows-specific functions
#include <iomanip>       // Input/output manipulators
#include <unordered_set> // Unordered set container

using namespace std;
// <===================== 🎨 COLOR CODES FOR DIFFERENT STATEMENTS 🌈 =====================>
const int SuccessMessagesColorCode = 10;
const int WaitingMessagesColorCode = 11;
const int ErrorMessagesColorCode = 12;
const int TittleColorCode = 6;
const int PressKeyColorCode = 8;
const int DefaultColorCode = 9;
const int HighlightColorCode = 5;

// <===================== 🚧 Keeping everything Centered from just one place 😎 =====================>
const string Spaces_to_be_Centered = "\t\t\t\t\t\t\t\t\t\t";

// <===================== 🎨 FUNCTIONS TO SET COLORS FOR DIFFERENT STATEMENTS 🌈 =====================>
void setConsoleForegroundColor(int colorCode)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}
void PrintColoredText(const string &text, int color)
{
    setConsoleForegroundColor(color);
    cout << Spaces_to_be_Centered << text << endl;
    setConsoleForegroundColor(DefaultColorCode);
}
void PrintColoredTittle(const string &text, int color)
{
    setConsoleForegroundColor(color);
    cout << "\t\t\t\t\t\t" << text << endl;
    setConsoleForegroundColor(DefaultColorCode);
}
void PrintErrorsORSucess(const string &text, int color)
{
    setConsoleForegroundColor(color);
    cout << Spaces_to_be_Centered;
    for (char c : text)
    {
        cout << c;
        Sleep(10); // Sleep for 10 milliseconds
    }
    cout << endl;
    setConsoleForegroundColor(DefaultColorCode);
}
void ShowMenuOptions(int num, string &option)
{
    setConsoleForegroundColor(TittleColorCode);
    cout << Spaces_to_be_Centered << "[" << num << "]" << "- ";
    setConsoleForegroundColor(DefaultColorCode);
    cout << option << endl;
}
void InputTaking(const string &text)
{
    setConsoleForegroundColor(TittleColorCode);
    cout << Spaces_to_be_Centered << text << ": ";
    setConsoleForegroundColor(DefaultColorCode);
}
// <===================== 🛠️ CHECKING & INSTALLING DEPENDENCIES 📦 =====================>
bool checkDependency(const string &command, const string &name)
{
    PrintColoredText("Checking if " + name + " is installed...", WaitingMessagesColorCode);
    bool isInstalled = system(command.c_str()) == 0;
    if (isInstalled)
    {
        PrintColoredText(name + " is installed.", SuccessMessagesColorCode);
    }
    else
    {
        PrintColoredText(name + " is not installed.", ErrorMessagesColorCode);
    }
    return isInstalled;
}

void installDependency(const string &installCommand, const string &name)
{
    PrintColoredText("Installing " + name + "...", WaitingMessagesColorCode);
    system(installCommand.c_str());
}
bool checkAndInstallDependencies()
{
    bool npmInstalled = checkDependency("npm -v > nul 2>&1", "npm");
    bool nodeInstalled = checkDependency("node -v > nul 2>&1", "Node.js");
    bool appwriteInstalled = checkDependency("npm list appwrite > nul 2>&1", "Appwrite");
    bool argon2Installed = checkDependency("npm list argon2 > nul 2>&1", "Argon2");

    if (!nodeInstalled && !npmInstalled)
    {
        PrintErrorsORSucess("Node.js or npm is required to run this program. Please download and install Node.js from https://nodejs.org/en/.", ErrorMessagesColorCode);
        return false;
    }

    if (!appwriteInstalled || !argon2Installed)
    {
        char choice;

        InputTaking("Some packages are missing. Would you like to install them? (y/n)");
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            if (!appwriteInstalled)
            {
                installDependency("npm install -g appwrite", "Appwrite");
            }
            if (!argon2Installed)
            {
                installDependency("npm install argon2", "Argon2");
            }
            return true;
        }
        else
        {
            if (!appwriteInstalled)
            {
                PrintColoredText("Appwrite is required to run this program. Exiting...", ErrorMessagesColorCode);
            }
            if (!argon2Installed)
            {
                PrintColoredText("Argon2 is required to run this program. Exiting...", ErrorMessagesColorCode);
            }
            return false;
        }
    }

    PrintColoredText("All dependencies are installed.", SuccessMessagesColorCode);
    return true;
}
// <===================== 🌐 FUNCTIONS FOR FETCHING DATA FROM BACKEND 📡 =====================>
string callJavaScript(const string &command)
{
    string cmd = "node Server.js " + command + " > output.txt";
    system(cmd.c_str());

    ifstream file("output.txt");
    string result;
    getline(file, result);
    file.close();

    ofstream clearFile("output.txt", ios::trunc);
    clearFile.close();
    return result;
}
string FetchAllLargeDataFromJavaScript(const string &command)
{
    string cmd = "node Server.js " + command + " > output.txt";
    system(cmd.c_str());

    ifstream file("output.txt");
    stringstream buffer;
    buffer << file.rdbuf();
    string result = buffer.str();
    file.close();

    ofstream clearFile("output.txt", ios::trunc);
    clearFile.close();

    return result;
}
// <===================== ✨🔰 MAIN CODE START 🌟🔰 =====================>

// Struct to represent a transaction
struct Transaction
{
    string timestamp;
    string username;
    string type; // "Deposit", "Withdrawal" or "Transfer"
    double amount;
};

// Base class to represent a bank account
class Account
{
protected:
    string cnic;
    string username;
    string password;
    string email;
    double balance;
    vector<Transaction> transactions; // Transaction history

public:
    // ⌚⏳ Function to get current timestamp ⌚⏳
    string getCurrentTimestamp()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << 1900 + ltm->tm_year << "-"
           << 1 + ltm->tm_mon << "-"
           << ltm->tm_mday << "_"
           << ltm->tm_hour << ":"
           << ltm->tm_min << ":"
           << ltm->tm_sec;
        return ss.str();
    }
    // Parameterized Constructor
    Account(string _cnic, string _username, string _password, string _email, double _balance = 0)
        : cnic(_cnic), username(_username), password(_password), email(_email), balance(_balance) {}

    // Virtual destructor
    virtual ~Account() {}

    // Getters
    string getUsername() const { return username; }

    string getCNIC() const { return cnic; }
    string getPassword() const { return password; }

    string getEmail() const { return email; }

    double getBalance() const { return balance; }
    void setBalance(double newBalance) { balance = newBalance; }

    // Log transaction to backend
    void logTransaction(const string &cnic, const string &timestamp, const string &type, double amount, const string &recipientCnic = "")
    {
        string command = "LogTransaction " + cnic + " " + timestamp + " " + type + " " + to_string(amount);
        if (!recipientCnic.empty())
        {
            command += " " + recipientCnic;
        }
        string result = callJavaScript(command);
    }
    // Update balance in backend
    void UpdateBalanceInBackend()
    {
        string command = "BalanceUpdate " + cnic + " " + to_string(balance);
        string result = callJavaScript(command);
    }

    // Deposit money into account
    void deposit(double amount, string type = "", string acctype = "")
    {
        if (balance + amount > 1000000)
        {
            PrintErrorsORSucess("Deposit failed: Balance cannot exceed $1,000,000.", ErrorMessagesColorCode);
            return;
        }
        else if (balance + amount < 0)
        {
            PrintErrorsORSucess("Deposit failed: Balance cannot be negative.", ErrorMessagesColorCode);
            return;
        }
        // Perform deposit
        balance += amount;
        string timestamp = getCurrentTimestamp();
        transactions.push_back({timestamp, username, type, amount});

        // Log transaction if not received or transfer
        if (type != "Received" && type != "Transfer")
        {
            logTransaction(cnic, timestamp, type, amount); // Log transaction
        }
        // Update balance in backend
        UpdateBalanceInBackend();

        // Calculate and apply interest for savings account
        if (acctype == "Savings")
        {
            double annualInterestRate = 0.01;                                    // Example: 1% per annum
            double interest = amount * (annualInterestRate / 12);                // Monthly interest calculation
            balance += interest;                                                 // Add interest to balance
            transactions.push_back({timestamp, username, "Interest", interest}); // Log interest transaction
            logTransaction(cnic, timestamp, "Interest", interest);               // Log interest in backend
            UpdateBalanceInBackend();                                            // Update balance in backend again after adding interest
            PrintErrorsORSucess("Interest earned on deposit: " + to_string(interest), SuccessMessagesColorCode);
        }
        PrintErrorsORSucess("Deposit successful.\n", SuccessMessagesColorCode);
    }
    // Withdraw money from account
    bool withdraw(double amount, string type)
    {
        if (balance >= amount)
        {
            balance -= amount;
            string timestamp = getCurrentTimestamp();
            transactions.push_back({timestamp, username, type, amount});
            if (type != "Received" && type != "Transfer")
            {
                logTransaction(cnic, timestamp, type, amount); // Log transaction
            }
            UpdateBalanceInBackend(); // Update balance in backend
            return true;
        }
        return false;
    }
    // Change username
    void changeUsername(string new_username)
    {
        username = new_username;
    }

    // Display user menu (virtual function)
    virtual void displayMenu()
    {
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                  User Dashoard                                      |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

        cout << Spaces_to_be_Centered << "Welcome, ";
        setConsoleForegroundColor(HighlightColorCode);
        cout << getUsername() << endl;
        setConsoleForegroundColor(DefaultColorCode);
        cout << Spaces_to_be_Centered << "Your balance is: $";
        setConsoleForegroundColor(HighlightColorCode);
        cout << getBalance() << endl;
        setConsoleForegroundColor(DefaultColorCode);
        cout << "\n";
        string options[] = {
            "My Account",
            "Deposit",
            "Withdraw",
            "Change Password",
            "Change Username",
            "View Transaction History",
            "Transfer Money",
            "Update Balance",
            "Logout"};
        int size = sizeof(options) / sizeof(options[0]);
        for (int i = 0; i < size; i++)
        {
            ShowMenuOptions(i + 1, options[i]);
        }
        cout << "\n";
    }
    // Display user details
    void displayUserDetails()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      My Account                                     |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        InputTaking("Username");
        cout << username << endl;
        InputTaking("CNIC");
        cout << cnic << endl;
        InputTaking("Email");
        cout << email << endl;
        InputTaking("Account Type");
        cout << getAccountType() << endl;
        InputTaking("Balance");
        cout << balance << endl;
        cout << "\n";
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }
    // View transaction history
    void viewTransactionHistory(string cnic) const
    {
        PrintErrorsORSucess("Hold on getting your transactions...", WaitingMessagesColorCode);
        string command = "FetchTransactions " + cnic;
        string result = FetchAllLargeDataFromJavaScript(command);
        system("cls");
        cout << "\n\n";
        PrintColoredText("Transaction History for User " + getUsername() + " :", TittleColorCode);

        if (result.empty())
        {
            PrintErrorsORSucess("No transactions found.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << result << endl;
        }

        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    // Abstract function to get account type (pure virtual function)
    virtual string getAccountType() const = 0;
};

// Derived class for Savings Account
class SavingsAccount : public Account
{
public:
    // Parameterized Constructor
    SavingsAccount(string _cnic, string _username, string _password, string _email, double _balance = 0)
        : Account(_cnic, _username, _password, _email, _balance) {}

    // Get account type
    string getAccountType() const override
    {
        return "Savings";
    }
};

// Derived class for Current Account
class CurrentAccount : public Account
{
public:
    // Parameterized Constructor
    CurrentAccount(string _cnic, string _username, string _password, string _email, double _balance = 0)
        : Account(_cnic, _username, _password, _email, _balance) {}

    // Get account type
    string getAccountType() const override
    {
        return "Current";
    }
};

// Class to manage accounts
class Bank
{
private:
    vector<Account *> accounts;
    vector<Account *> pendingAccounts;

public:
    Bank() {}

    ~Bank()
    {
        for (auto account : accounts)
        {
            delete account;
        }
        for (auto account : pendingAccounts)
        {
            delete account;
        }
    }
    // <===================== ✨🔃 Data Retrieving from backend to the vectors 🔃🌟 =====================>
    void LoadAllAccountsFromBackend()
    {
        accounts.clear();
        pendingAccounts.clear();
        loadAccountsFromBackend();
        loadPendingAccountsFromBackend();
    }
    void loadAccountsFromBackend()
    {
        string command = "FetchAccounts"; // Command to fetch all user details
        string result = FetchAllLargeDataFromJavaScript(command);

        if (result.empty())
        {
            PrintErrorsORSucess("Failed to Connect to Database. Make sure you have a strong internet connection.Please try again.", ErrorMessagesColorCode);
            PrintColoredText("Press any key to exit...", PressKeyColorCode);
            _getch(); // Wait for user input
            exit(0);  // Exit the program
        }
        // Split the result string into lines
        stringstream ss(result);
        string line;

        while (getline(ss, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            // Split each line into account details
            stringstream lineStream(line);
            string username, email, accountType, cnic;
            double balance;

            lineStream >> username >> email >> accountType >> balance >> cnic;

            // Create account objects based on the account type
            Account *account = nullptr;
            if (accountType == "Savings")
            {
                account = new SavingsAccount(cnic, username, "", email, balance);
            }
            else
            {
                account = new CurrentAccount(cnic, username, "", email, balance);
            }

            // Add the account object to the accounts vector
            accounts.push_back(account);
        }

        // Displaying the fetched result for verification
        // cout << "Press any key to continue...";
        // _getch();
    }

    void loadPendingAccountsFromBackend()
    {
        string command = "FetchPendingAccounts"; // Command to fetch all user details
        string result = FetchAllLargeDataFromJavaScript(command);

        // Split the result string into lines
        stringstream ss(result);
        string line;

        while (getline(ss, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            // Split each line into account details
            stringstream lineStream(line);
            string username, email, accountType, cnic;
            double balance;

            lineStream >> username >> email >> accountType >> cnic;

            // Create account objects based on the account type
            Account *pendingaccount = nullptr;
            if (accountType == "Savings")
            {
                pendingaccount = new SavingsAccount(cnic, username, "", email, 0);
                ;
            }
            else
            {
                pendingaccount = new CurrentAccount(cnic, username, "", email, 0);
            }

            // Add the account object to the accounts vector
            pendingAccounts.push_back(pendingaccount);
        }
    }
    // <===================== 🔐👥 User and Admin AUTH 🔐👥 =====================>
    bool authenticateAdmin(string username, string password)
    {
        PrintErrorsORSucess("Proccessing your request...", WaitingMessagesColorCode);
        string command = "AdminLogin " + username + " " + password;
        string result = callJavaScript(command);
        return (result == "SUCCESS" ? true : false);
    }

    Account *authenticateUser(string cnic, string password)
    {
        PrintErrorsORSucess("Proccessing your request...", WaitingMessagesColorCode);
        string command = "UserLogin " + cnic + " " + password;
        string result = callJavaScript(command);
        if (result == "SUCCESS")
        {
            PrintErrorsORSucess("Logging in...", WaitingMessagesColorCode);
            accounts.clear();
            loadAccountsFromBackend();
            PrintErrorsORSucess("Getting your dashboard ready...", WaitingMessagesColorCode);
            // Search for the user in the loaded accounts vector
            for (auto account : accounts)
            {
                if (account->getCNIC() == cnic)
                {
                    // If CNIC match, return the account object
                    return account;
                }
            }
        }

        // If user not found , return nullptr
        return nullptr;
    }
    void createAccount(Account *account, bool isPending = false)
    {
        if (isPending)
        {
            pendingAccounts.push_back(account);
        }
        else
        {
            accounts.push_back(account);
        }
    }

    void approvePendingAccounts()
    {
        pendingAccounts.clear();
        cout << "\n";
        PrintErrorsORSucess("Fetching Users...", WaitingMessagesColorCode);
        loadPendingAccountsFromBackend();
        system("cls");

        if (pendingAccounts.empty())
        {
            PrintErrorsORSucess("No pending accounts to approve.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                                 Pending Accounts                                    |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            cout << "\n";

            // Print table header
            cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+\n";
            cout << "\t| No |         CNIC          |       Username       |         Email        |    Account Type   |\n";
            cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+\n";

            // Store CNICs and Emails to detect duplicates
            unordered_set<string> cnicSet;
            unordered_set<string> emailSet;
            // Iterate over pendingAccounts and print details in table format
            for (size_t i = 0; i < pendingAccounts.size(); ++i)
            {
                string cnic = pendingAccounts[i]->getCNIC();
                string email = pendingAccounts[i]->getEmail();

                // Check for duplicates
                bool isDuplicate = false;
                if (cnicSet.count(cnic) > 0 || emailSet.count(email) > 0)
                {
                    isDuplicate = true;
                }

                // Print row in red if duplicate found
                if (isDuplicate)
                {
                    setConsoleForegroundColor(ErrorMessagesColorCode);
                }

                // Print account details
                cout << "\t| " << setw(2) << i + 1 << " | "
                     << setw(21) << cnic << " | "
                     << setw(20) << pendingAccounts[i]->getUsername() << " | "
                     << setw(20) << email << " | "
                     << setw(17) << pendingAccounts[i]->getAccountType() << " |\n";

                // Reset color if printed in red
                if (isDuplicate)
                {
                    setConsoleForegroundColor(DefaultColorCode);
                }

                // Add CNIC and Email to sets for future checks
                cnicSet.insert(cnic);
                emailSet.insert(email);
                cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+\n";
            }
            cout << "\n";

            // Ask user to approve an account
            int choice;
            InputTaking("Enter the number of the account to approve (0 to exit)");
            cin >> choice;

            if (choice > 0 && choice <= pendingAccounts.size())
            {
                string cnic = pendingAccounts[choice - 1]->getCNIC();

                string command = "MoveToAccounts " + cnic;
                string result = callJavaScript(command);

                if (result == "ACCOUNT MOVED TO ACCOUNTS SUCCESSFULLY")
                {
                    // Move the account from pendingAccounts to accounts
                    accounts.push_back(pendingAccounts[choice - 1]);
                    pendingAccounts.erase(pendingAccounts.begin() + choice - 1);
                    PrintErrorsORSucess("Account approved and moved to main accounts.", SuccessMessagesColorCode);
                }
                else
                {
                    PrintErrorsORSucess("Failed to move user to main accounts.", ErrorMessagesColorCode);
                }
            }
            else
            {
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
            }
        }

        cout << "\n";
        PrintErrorsORSucess("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void changeUserPassword(string cnic, string new_password)
    {
        PrintErrorsORSucess("Hold on changing your Password...", WaitingMessagesColorCode);
        string command = "ChangeUserPassword " + cnic + " " + new_password;
        string result = callJavaScript(command);

        PrintErrorsORSucess("Password Changed Successfully\n", SuccessMessagesColorCode);
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }
    void changeUserUsername(string cnic, string new_username, Account *account)
    {
        PrintErrorsORSucess("Hold on changing your Username...", WaitingMessagesColorCode);
        string command = "ChangeUserUsername " + cnic + " " + new_username;
        string result = callJavaScript(command);

        if (result == "USERNAME_UPDATED")
        {
            account->changeUsername(new_username);
            PrintErrorsORSucess("Username changed successfully.", SuccessMessagesColorCode);
        }
        else
        {
            PrintErrorsORSucess("Username change failed.", ErrorMessagesColorCode);
        }
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void viewAllAccounts()
    {
        accounts.clear();
        PrintErrorsORSucess("Fetching Users...", WaitingMessagesColorCode);
        loadAccountsFromBackend();
        system("cls");

        if (accounts.empty())
        {
            PrintErrorsORSucess("No accounts found.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                                   All Accounts                                      |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            cout << "\n";

            // Print table header
            cout << "\t+---------------------+----------------------+---------------------------+--------------------+------------+\n";
            cout << "\t|        CNIC         |       Username       |           Email           |      Balance       |    Type    |\n";
            cout << "\t+---------------------+----------------------+---------------------------+--------------------+------------+\n";

            // Print account details
            for (const auto &account : accounts)
            {
                cout << "\t| " << setw(19) << left << account->getCNIC()
                     << " | " << setw(20) << left << account->getUsername()
                     << " | " << setw(25) << left << account->getEmail()
                     << " | " << setw(18) << left << "$" + to_string(account->getBalance())
                     << " | " << setw(10) << left << account->getAccountType()
                     << " |\n";
            }

            // Print table footer
            cout << "\t+---------------------+----------------------+---------------------------+--------------------+------------+\n";
        }

        cout << "\n";
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }
    void removeAccount()
    {
        accounts.clear();
        cout << "\n";
        PrintErrorsORSucess("Fetching Users...", WaitingMessagesColorCode);
        loadAccountsFromBackend();
        system("cls");

        if (accounts.empty())
        {
            PrintErrorsORSucess("No accounts to remove.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                                 Remove Account                                      |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            cout << "\n";

            // Print table header
            cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+-------------------+\n";
            cout << "\t| No |         CNIC          |       Username       |         Email        |      Balance      |    Account Type   |\n";
            cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+-------------------+\n";

            // Iterate over accounts and print details in table format
            for (size_t i = 0; i < accounts.size(); ++i)
            {
                cout << "\t| " << setw(2) << i + 1 << " | "
                     << setw(21) << accounts[i]->getCNIC() << " | "
                     << setw(20) << accounts[i]->getUsername() << " | "
                     << setw(20) << accounts[i]->getEmail() << " | "
                     << setw(17) << fixed << setprecision(2) << accounts[i]->getBalance() << " | "
                     << setw(17) << accounts[i]->getAccountType() << " |\n";
                cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+-------------------+\n";
            }

            cout << "\n";
            int choice;
            InputTaking("Enter the number of the account to remove (0 to exit)");
            cin >> choice;

            if (choice > 0 && choice <= accounts.size())
            {
                string command = "DeleteAccount " + accounts[choice - 1]->getCNIC();
                string result = callJavaScript(command);
                if (result == "ACCOUNT_DELETED")
                {
                    delete accounts[choice - 1];
                    accounts.erase(accounts.begin() + choice - 1);
                    PrintErrorsORSucess("Account removed.\n", SuccessMessagesColorCode);
                }
                else
                {
                    PrintErrorsORSucess("Failed to delete account. Make sure you have an active internet connection.", ErrorMessagesColorCode);
                }
            }
            else
            {
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
            }
        }

        cout << "\n";
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }
    void WriteEmail()
    {
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                  Write Email                                        |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        cout << "\n";

        string email, message, command, result;
        InputTaking("Enter Recipient's Email");
        cin >> email;
        cin.ignore(); // Ignore the newline character left in the buffer by cin

        if (!regex_match(email, regex(".+@.+\\..+")))
        {
            PrintErrorsORSucess("Invalid email format. Please enter a valid email.", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
        }
        else
        {
            InputTaking("Write Message");
            getline(cin, message); // Using getline to read the entire line
            if (!message.empty())
            {

                PrintErrorsORSucess("Sending Message...", WaitingMessagesColorCode);
                command = "sendEmail " + email + " " + message;
                result = callJavaScript(command);
                if (result == "EMAIL_SENT")
                {
                    PrintErrorsORSucess("Email sent successfully.\n", SuccessMessagesColorCode);
                    PrintColoredText("Press any key to continue...", PressKeyColorCode);
                    _getch(); // Wait for user input
                }
                else
                {
                    PrintErrorsORSucess("Oops! Something went wrong...", ErrorMessagesColorCode);
                    PrintColoredText("Press any key to continue...", PressKeyColorCode);
                    _getch(); // Wait for user input
                }
            }
            else
            {
                PrintErrorsORSucess("Please write something to send ", ErrorMessagesColorCode);
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
            }
        }
    }

    bool isNumeric(const string &str)
    {
        for (char const &c : str)
        {
            if (isdigit(c) == 0)
                return false;
        }
        return true;
    }

    double getValidatedAmount()
    {
        double amount;
        string input;
        while (true)
        {
            InputTaking("Enter amount");
            cin >> input;
            if (isNumeric(input))
            {
                stringstream ss(input);
                ss >> amount;
                if (amount > 0)
                {
                    break;
                }
                else
                {
                    PrintErrorsORSucess("Amount should be positive.\n", ErrorMessagesColorCode);
                }
            }
            else
            {
                PrintErrorsORSucess("Invalid amount. Please enter a numeric value.\n", ErrorMessagesColorCode);
            }
        }
        return amount;
    }

    string getPasswordInput()
    {
        string password;
        char ch;
        while ((ch = _getch()) != '\r')
        { // '\r' is the Enter key
            if (ch == '\b')
            { // '\b' is the Backspace key
                if (!password.empty())
                {
                    password.pop_back();
                    cout << "\b \b"; // Erase character from console
                }
            }
            else
            {
                password += ch;
                cout << '*'; // Print asterisk instead of character
            }
        }
        cout << endl;
        return password;
    }

    void transferMoney(Account *from_account)
    {
        accounts.clear();
        loadAccountsFromBackend();
        string recipient_CNIC;
        InputTaking("Enter recipient's CNIC");
        cin >> recipient_CNIC;

        // Check if the recipient CNIC is the same as the sender's CNIC
        if (from_account->getCNIC() == recipient_CNIC)
        {
            PrintErrorsORSucess("You cannot transfer money to the your account.\n", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
            return;
        }

        Account *recipient_account = nullptr;
        for (auto account : accounts)
        {
            if (account->getCNIC() == recipient_CNIC)
            {
                recipient_account = account;
                break;
            }
        }

        if (recipient_account)
        {
            double amount = getValidatedAmount();
            PrintErrorsORSucess("Initiating transfer process...", WaitingMessagesColorCode);

            // cout << "Attempting to withdraw amount: " << amount << " from " << from_account->getUsername() << endl;

            if (from_account->withdraw(amount, "Transfer"))
            {
                PrintErrorsORSucess("Amount deducted successfully from " + from_account->getUsername() + "'s account.", SuccessMessagesColorCode);
                stringstream stream;
                stream << std::fixed;
                stream.precision(2);
                stream << amount;
                string amountStr = stream.str();
                PrintErrorsORSucess("Transferring $" + amountStr + " to " + recipient_account->getUsername() + "...", WaitingMessagesColorCode);
                // cout << "Withdrawal successful. Depositing amount to " << recipient_account->getUsername() << endl;
                recipient_account->deposit(amount, "Received", "");

                string timestamp = from_account->getCurrentTimestamp();
                // cout << "Logging transfer transaction for sender.\n";
                from_account->logTransaction(from_account->getCNIC(), timestamp, "Transfer", amount, recipient_CNIC);

                // cout << "Logging transfer transaction for recipient.\n";
                recipient_account->logTransaction(recipient_account->getCNIC(), timestamp, "Received", amount, from_account->getCNIC());

                PrintErrorsORSucess("Transfer completed successfully!", SuccessMessagesColorCode);
                PrintErrorsORSucess("The amount has been transferred.", SuccessMessagesColorCode);
            }

            else
            {
                PrintErrorsORSucess("Insufficient balance for transfer.\n", ErrorMessagesColorCode);
            }
        }
        else
        {
            PrintErrorsORSucess("Recipient account not found.\n", ErrorMessagesColorCode);
        }
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void start()
    {
        while (true)
        {
            system("cls");
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                              Welcome to AUT Finance Bank                            |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            cout << "\n";
            string options[] = {"Login",
                                "Forgot Password",
                                "Create Account",
                                "Admin Login",
                                "About Us",
                                "Exit"};
            int size = sizeof(options) / sizeof(options[0]);
            for (int i = 0; i < size; i++)
            {
                ShowMenuOptions(i + 1, options[i]);
            }
            cout << "\n";
            int choice;
            InputTaking("Enter your choice");
            cin >> choice;

            switch (choice)
            {
            case 1:
                login();
                break;
            case 2:
                forgotPassword();
                break;
            case 3:
                createAccount();
                break;
            case 4:
                adminLogin();
                break;
            case 5:
                About();
                break;
            case 6:
                return;
            default:
                PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
                _getch();                                            // Wait for user input
                break;
            }
        }
    }

    void login()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      User Login                                     |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

        string cnic, password;
        InputTaking("Enter CNIC");
        cin >> cnic;
        InputTaking("Enter password");
        password = getPasswordInput();

        Account *account = authenticateUser(cnic, password);
        if (account)
        {
            userMenu(account);
        }
        else
        {
            PrintErrorsORSucess("Invalid CNIC or password.", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
        }
    }
    void forgotPassword()
    {
        while (true)
        {

            system("cls");
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                                  Forgot Password                                    |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            cout << "\n";
            string options[] = {"Recover password via CNIC and Key", "Recover password via Email", "Go back"};
            int size = sizeof(options) / sizeof(options[0]);
            for (int i = 0; i < size; i++)
            {
                ShowMenuOptions(i + 1, options[i]);
            }

            cout << "\n";
            int choice;
            InputTaking("Enter your choice");
            cin >> choice;

            switch (choice)
            {
            case 1:
                RecoverPasswordWithCNICAndKey();
                break;
            case 2:
                RecoverPasswordWithEmailAndOTP();
                break;
            case 3:
                return;
            default:
                PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
                _getch();
                break;
            }
        }
    }
    void RecoverPasswordWithCNICAndKey()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                 Password Recovery                                   |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        string cnic, key, command, result;
        InputTaking("Enter CNIC");
        cin >> cnic;
        InputTaking("Enter Key");
        cin >> key;

        PrintErrorsORSucess("Processing your request...", WaitingMessagesColorCode);
        command = "forgot_password " + cnic + " " + key;
        result = callJavaScript(command);

        if (result == "SUCCESS")
        {
            PrintErrorsORSucess("Preparing for change password...", WaitingMessagesColorCode);
            accounts.clear();
            loadAccountsFromBackend();
            for (const auto &account : accounts)
            {
                if (account->getCNIC() == cnic)
                {
                    while (true)
                    {
                        string new_password;
                        InputTaking("Enter new password");
                        cin >> new_password;

                        if (new_password.length() < 8)
                        {
                            PrintErrorsORSucess("Password must be at least 8 characters", ErrorMessagesColorCode);
                        }
                        else
                        {
                            changeUserPassword(account->getCNIC(), new_password);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else
        {
            PrintErrorsORSucess("Invalid CNIC or Key.", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
        }
    }
    void RecoverPasswordWithEmailAndOTP()
    {
        while (true)
        {
            system("cls");

            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                                 Password Recovery                                   |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            string email, command, result;
            InputTaking("Enter Email");
            cin >> email;

            PrintErrorsORSucess("Processing your request...", WaitingMessagesColorCode);

            if (CheckEmail(email))
            {
                PrintErrorsORSucess("Email not found. Please enter a registered email.", ErrorMessagesColorCode);
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
                continue;
            }

            if (!verifyOtp(email, "Password Recovery"))
            {
                PrintErrorsORSucess("Invalid OTP. Please try again.", ErrorMessagesColorCode);
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
                continue;
            }

            accounts.clear();
            loadAccountsFromBackend();
            for (const auto &account : accounts)
            {
                if (account->getEmail() == email)
                {
                    while (true)
                    {
                        string new_password;
                        InputTaking("Enter new password");
                        cin >> new_password;

                        if (new_password.length() < 8)
                        {
                            PrintErrorsORSucess("Password must be at least 8 characters", ErrorMessagesColorCode);
                        }
                        else
                        {
                            changeUserPassword(account->getCNIC(), new_password);
                            break;
                        }
                    }
                    break;
                }
            }
            break; // Exit the while loop after successful password change
        }
    }

    bool CheckCNIC(string cnic)
    {
        // Assume CNIC is not in use initially
        bool cnicAvailable = true;
        PrintErrorsORSucess("Please wait while your CNIC is being verified...", WaitingMessagesColorCode);
        LoadAllAccountsFromBackend();
        // Check if CNIC already exists in loaded accounts
        for (const auto &account : accounts)
        {
            if (account->getCNIC() == cnic)
            {
                cnicAvailable = false;
                break;
            }
        }

        // Check if CNIC already exists in pending accounts
        if (cnicAvailable)
        {
            for (const auto &account : pendingAccounts)
            {
                if (account->getCNIC() == cnic)
                {
                    cnicAvailable = false;
                    break;
                }
            }
        }
        return cnicAvailable; // Return whether CNIC is available or not
    }
    bool CheckEmail(string email)
    {
        // Assume email is not in use initially
        bool emailAvailable = true;
        PrintErrorsORSucess("Please wait while your EMAIL is being verified...", WaitingMessagesColorCode);
        LoadAllAccountsFromBackend();
        // Check if email already exists in loaded accounts
        for (const auto &account : accounts)
        {
            if (account->getEmail() == email)
            {
                emailAvailable = false;
                break;
            }
        }

        // Check if email already exists in pending accounts
        if (emailAvailable)
        {
            for (const auto &account : pendingAccounts)
            {
                if (account->getEmail() == email)
                {
                    emailAvailable = false;
                    break;
                }
            }
        }
        return emailAvailable; // Return whether email is available or not
    }
    int SendOTP(string email, string reason)
    {
        srand(time(0)); // Seed the random number generator with current time
        int code = rand() % 9000 + 1000;
        string scode = to_string(code);
        PrintErrorsORSucess("Please wait we are sending you OTP for " + reason + "...", WaitingMessagesColorCode);
        string command = "sendOTPEmail " + email + " " + scode + " " + reason;
        callJavaScript(command);

        int temp_code;
        InputTaking("Enter OTP Sent On your Email");

        cin >> temp_code;

        if (temp_code == code)
        {
            return 0; // OTP is correct
        }
        else
        {
            return 1; // OTP is incorrect
        }
    }

    bool verifyOtp(string email, string reason)
    {
        int result = SendOTP(email, reason);
        return result == 0;
    }
    string generateRandomKey(int length)
    {
        const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const int max_index = sizeof(charset) - 1;

        string random_key;
        for (int i = 0; i < length; ++i)
        {
            random_key += charset[rand() % max_index];
        }
        return random_key;
    }

    void createAccount()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      User Registration                              |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        string command;
        string cnic, username, password, email, account_type;
        double initial_deposit = 0;

        while (true)
        {
            InputTaking("Enter CNIC (xxxxx-xxxxxxx-x)");
            cin >> cnic;

            if (!regex_match(cnic, regex(R"(^\d{5}-\d{7}-\d{1}$)")))
            {
                PrintErrorsORSucess("Invalid CNIC. Please enter a valid 13-digit CNIC including dashes (\"-\").", ErrorMessagesColorCode);
            }
            else if (!CheckCNIC(cnic))
            {
                PrintErrorsORSucess("CNIC is already in use. Please enter a different CNIC.", ErrorMessagesColorCode);
            }
            else
            {
                break; // Email is unique, exit the loop
            }
        }

        while (true)
        {
            InputTaking("Enter username");
            cin >> username;
            if (username.length() < 3)
            {
                PrintErrorsORSucess("Username must be at least 3 characters", ErrorMessagesColorCode);
            }
            else
            {
                break;
            }
        }

        while (true)
        {
            InputTaking("Enter password");
            cin >> password;
            if (password.length() < 8)
            {
                PrintErrorsORSucess("Password must be at least 8 characters", ErrorMessagesColorCode);
            }
            else
            {
                break;
            }
        }

        while (true)
        {
            InputTaking("Enter email");
            cin >> email;

            if (!regex_match(email, regex(".+@.+\\..+")))
            {
                PrintErrorsORSucess("Invalid email format. Please enter a valid email.", ErrorMessagesColorCode);
            }
            else if (!CheckEmail(email))
            {
                PrintErrorsORSucess("Email is already in use. Please enter a different email.", ErrorMessagesColorCode);
            }
            else if (!verifyOtp(email, "Email Verification"))
            {
                PrintErrorsORSucess("Otp is not valid. Please enter a valid otp.", ErrorMessagesColorCode);
            }
            else
            {
                break; // Email is unique, exit the loop
            }
        }
        while (true)
        {
            InputTaking("Enter account type (Savings 'S' /Current 'C')");
            cin >> account_type;
            if (account_type == "S" || account_type == "s")
            {
                createAccount(new SavingsAccount(cnic, username, password, email, initial_deposit), true);
                break;
            }
            else if (account_type == "C" || account_type == "c")
            {
                createAccount(new CurrentAccount(cnic, username, password, email, initial_deposit), true);
                break;
            }
            else
            {
                PrintErrorsORSucess("Invalid account type. Please try again.", ErrorMessagesColorCode);
            }
        }
        srand(time(0));     // Seed the random number generator
        int key_length = 5; // Length of the random key
        string random_key = generateRandomKey(key_length);
        cout << "\n";
        PrintColoredTittle("-------------------------------------------------------------------------------------", ErrorMessagesColorCode);
        PrintColoredTittle("|     Save this secret key. You'll need it if you forget your password : " + random_key + "      |", SuccessMessagesColorCode);
        PrintColoredTittle("-------------------------------------------------------------------------------------", ErrorMessagesColorCode);
        cout << "\n";
        command = "forgot_password " + cnic + " " + random_key + " " + "create";
        callJavaScript(command);

        PrintErrorsORSucess("Hold on creating your account.", WaitingMessagesColorCode);

        command = "Register " + username + " " + password + " " + email + " " + account_type + " " + cnic + " " + random_key;
        callJavaScript(command);
        PrintErrorsORSucess("Account created successfully. Pending admin approval.", SuccessMessagesColorCode);
        cout << "\n";
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void adminLogin()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      Admin Login                                    |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        string username, password;
        InputTaking("Enter admin username");
        cin >> username;
        InputTaking("Enter admin password");
        password = getPasswordInput();

        if (authenticateAdmin(username, password))
        {
            PrintErrorsORSucess("Logging in...", WaitingMessagesColorCode);
            adminMenu();
        }
        else
        {
            PrintErrorsORSucess("Invalid admin username or password.\n", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
        }
    }

    void adminMenu()
    {
        while (true)
        {
            system("cls");
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                                  Admin Dashoard                                     |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            string options[] = {"Approve Pending Accounts",
                                "View All Accounts",
                                "Remove Accounts",
                                "Write Email",
                                "Logout"};
            int size = sizeof(options) / sizeof(options[0]);
            for (int i = 0; i < size; i++)
            {
                ShowMenuOptions(i + 1, options[i]);
            }
            cout << "\n";
            int choice;
            InputTaking("Enter your choice");
            cin >> choice;

            switch (choice)
            {
            case 1:
                system("cls");
                approvePendingAccounts();
                break;
            case 2:
                system("cls");
                viewAllAccounts();
                break;
            case 3:
                system("cls");
                removeAccount();
                break;
            case 4:
                system("cls");
                WriteEmail();
                break;
            case 5:
                return;
            default:
                PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
                _getch();
                break;
            }
        }
    }

    void userMenu(Account *account)
    {
        while (true)
        {
            system("cls");
            cout << "\n\n";
            account->displayMenu();
            int choice;
            InputTaking("Enter your choice");
            cin >> choice;

            switch (choice)
            {
            case 1:
                account->displayUserDetails();
                break;
            case 2:
            {
                double amount = getValidatedAmount();
                PrintErrorsORSucess("Initiating Deposit process...", WaitingMessagesColorCode);
                account->deposit(amount, "Deposit", account->getAccountType());
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
                break;
            }
            case 3:
            {
                double amount = getValidatedAmount();
                PrintErrorsORSucess("Initiating Withdraw process...", WaitingMessagesColorCode);
                if (account->withdraw(amount, "Withdrawal"))
                {
                    PrintErrorsORSucess("Withdrawal successful.\n", SuccessMessagesColorCode);
                }
                else
                {
                    PrintErrorsORSucess("Insufficient balance.\n", ErrorMessagesColorCode);
                }
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
                break;
            }
            case 4:
            {
                while (true)
                {
                    string new_password;
                    InputTaking("Enter new password");
                    cin >> new_password;

                    if (new_password.length() < 8)
                    {
                        PrintErrorsORSucess("Password must be at least 8 characters", ErrorMessagesColorCode);
                    }
                    else
                    {
                        changeUserPassword(account->getCNIC(), new_password);
                        break;
                    }
                }
                break;
            }
            case 5:
            {
                while (true)
                {
                    string new_username;
                    InputTaking("Enter new username");
                    cin >> new_username;

                    if (new_username.length() < 3)
                    {
                        PrintErrorsORSucess("Username must be at least 3 characters", ErrorMessagesColorCode);
                    }
                    else
                    {
                        changeUserUsername(account->getCNIC(), new_username, account);
                        break;
                    }
                }
                break;
            }
            case 6:
                system("cls");
                account->viewTransactionHistory(account->getCNIC());
                break;
            case 7:
                transferMoney(account);
                break;
            case 8:
                PrintErrorsORSucess("Proccessing your request...", WaitingMessagesColorCode);
                accounts.clear();
                loadAccountsFromBackend();
                for (auto &acc : accounts)
                {
                    if (acc->getCNIC() == account->getCNIC())
                    {
                        account->setBalance(acc->getBalance());
                    }
                }
                PrintErrorsORSucess("Your balance is updated now !!", SuccessMessagesColorCode);
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
                break;
            case 9:
                return;
            default:
                PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
                _getch();
                break;
            }
        }
    }
    void About()
    {
        system("cls");

        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      About Us                                       |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        cout << "\n";

        PrintColoredText("Meet Our Team", TittleColorCode);
        cout << "\n";

        string Names[] = {
            " [.] AbdulAhad Sadiq",
            " [.] Taimoor Hassan",
            " [.] Usman Ali",
        };
        int size = sizeof(Names) / sizeof(Names[0]);
        for (int i = 0; i < size; i++)
        {
            PrintErrorsORSucess(Names[i], HighlightColorCode);
        }
        setConsoleForegroundColor(DefaultColorCode);
        cout << "\n\n";

        PrintColoredText("Project Overview", TittleColorCode);
        cout << "\n";
        PrintErrorsORSucess(" Using Object-Oriented Programming (OOP) principles in C++ for the frontend", DefaultColorCode);
        PrintErrorsORSucess(" and Node.js for the backend,", DefaultColorCode);
        PrintErrorsORSucess(" we've crafted an application that seamlessly integrates functionality and performance.", DefaultColorCode);
        cout << "\n";

        PrintErrorsORSucess(" Utilizing Appwrite, a robust Cloud-based Database,", DefaultColorCode);
        PrintErrorsORSucess(" we ensure global connectivity and data security for all our users.", DefaultColorCode);
        cout << "\n\n";

        PrintErrorsORSucess("Thank you for using our application!", SuccessMessagesColorCode);
        cout << "\n\n";

        PrintErrorsORSucess("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }
};

void setConsoleTitle(const char *title)
{
    SetConsoleTitleA(title);
}
int main()
{
    if (!checkAndInstallDependencies())
    {
        return 1; // Exit if dependencies are not installed
    }
    setConsoleTitle("AUT Finance Bank");

    Bank bank;
    bank.start();

    return 0;
}
