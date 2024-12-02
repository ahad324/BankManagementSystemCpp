#include <iostream>
#include <string>
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

bool Account::isBalanceValid(double testBalance)
{
  const int MaxAmount = 100000;
  if (testBalance > MaxAmount)
  {
    PrintErrorsORSucess("Deposit failed: Balance cannot exceed $100,000.", ErrorMessagesColorCode);
    return false;
  }
  if (testBalance < 0)
  {
    PrintErrorsORSucess("Deposit failed: Balance cannot be negative.", ErrorMessagesColorCode);
    return false;
  }
  return true;
}

// Deposit money into account
void Account::deposit(double amount, string type, string acctype)
{
  double newBalance = balance;

  // Validate deposit
  newBalance += amount;
  if (!isBalanceValid(newBalance))
    return; // Exit the function if the balance is invalid

  // If it's a savings account, calculate interest
  if (acctype == "Savings")
  {
    double annualInterestRate = 0.01;                     // Example: 1% per annum
    double interest = amount * (annualInterestRate / 12); // Monthly interest calculation
    newBalance += interest;

    if (!isBalanceValid(newBalance))
      return; // Exit the function if the balance is invalid after interest calculation

    // Apply interest
    transactions.push_back({getCurrentTimestamp(), username, "Interest", interest});
    logTransaction(cnic, getCurrentTimestamp(), "Interest", interest);
    PrintErrorsORSucess("Interest earned on deposit: " + to_string(interest), SuccessMessagesColorCode);
  }

  // Update the actual balance after all checks pass
  balance = newBalance;

  // Log the deposit transaction
  string timestamp = getCurrentTimestamp();
  transactions.push_back({timestamp, username, type, amount});
  if (type != "Received" && type != "Transfer")
  {
    logTransaction(cnic, timestamp, type, amount);
  }

  // Update balance in the backend
  UpdateBalanceInBackend();
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