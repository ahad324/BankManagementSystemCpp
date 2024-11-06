#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <conio.h>
#include "../include/Account.h"
#include "../include/JavascriptUtils.h"
#include "../include/ConsoleUtils.h"
#include "../include/Constants.h"
#include "../include/Menu.h"
using namespace std;

Account::Account(string _cnic, string _username, string _password, string _email, double _balance, string _forgot_password_key)
    : cnic(_cnic), username(_username), password(_password), email(_email), balance(_balance), forgot_password_key(_forgot_password_key) {}

Account::~Account() {}

// ⌚⏳ Function to get current timestamp ⌚⏳
string Account::getCurrentTimestamp()
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

// Getters
string Account::getUsername() const { return username; }

string Account::getCNIC() const { return cnic; }
string Account::getPassword() const { return password; }

string Account::getEmail() const
{
  return email;
}

double Account::getBalance() const { return balance; }
void Account::setBalance(double newBalance) { balance = newBalance; }

// Log transaction to backend
void Account::logTransaction(const string &cnic, const string &timestamp, const string &type, double amount, const string &recipientCnic)
{
  string command = "LogTransaction " + cnic + " " + timestamp + " " + type + " " + to_string(amount);
  if (!recipientCnic.empty())
  {
    command += " " + recipientCnic;
  }
  string result = callJavaScript(command);
}
// Update balance in backend
void Account::UpdateBalanceInBackend()
{
  string command = "BalanceUpdate " + cnic + " " + to_string(balance);
  string result = callJavaScript(command);
}

// Deposit money into account
void Account::deposit(double amount, string type, string acctype)
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
bool Account::withdraw(double amount, string type)
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
void Account::changeUsername(string new_username)
{
  username = new_username;
}

// Display user menu (virtual function)
void Account::displayMenu()
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
void Account::displayUserDetails()
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
  InputTaking("Forgot Password key");
  cout << forgot_password_key << endl;
  cout << "\n";
  PrintColoredText("Press any key to continue...", PressKeyColorCode);
  _getch(); // Wait for user input
}
// View transaction history
void Account::viewTransactionHistory(string cnic) const
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

SavingsAccount::SavingsAccount(string _cnic, string _username, string _password, string _email, double _balance, string _forgot_password_key)
    : Account(_cnic, _username, _password, _email, _balance, _forgot_password_key) {}

// Get account type
string SavingsAccount::getAccountType() const
{
  return "Savings";
}

CurrentAccount::CurrentAccount(string _cnic, string _username, string _password, string _email, double _balance, string _forgot_password_key)
    : Account(_cnic, _username, _password, _email, _balance, _forgot_password_key) {}

// Get account type
string CurrentAccount::getAccountType() const
{
  return "Current";
}