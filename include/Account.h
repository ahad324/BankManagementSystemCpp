#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
using namespace std;

// Struct to represent a transaction
struct Transaction
{
  string timestamp;
  string username;
  string type; // "Deposit", "Withdrawal" or "Transfer"
  double amount;
};

class Account
{
protected:
  string cnic;
  string username;
  string password;
  string email;
  string forgot_password_key;
  double balance;
  vector<Transaction> transactions; // Transaction history

public:
  Account(string _cnic, string _username, string _password, string _email, double _balance = 0, string _forgot_password_key = "");
  virtual ~Account();
  string getUsername() const;
  string getCNIC() const;
  string getPassword() const;
  string getEmail() const;
  double getBalance() const;
  void setBalance(double newBalance);
  void logTransaction(const string &cnic, const string &timestamp, const string &type, double amount, const string &recipientCnic = "");
  void UpdateBalanceInBackend();
  void deposit(double amount, string type = "", string acctype = "");
  bool withdraw(double amount, string type);
  void changeUsername(string new_username);
  virtual void displayMenu();
  void displayUserDetails();
  void viewTransactionHistory(string cnic) const;
  virtual string getAccountType() const = 0;
  string getCurrentTimestamp();
  bool isBalanceValid(double testBalance);
};

class SavingsAccount : public Account
{
public:
  SavingsAccount(string _cnic, string _username, string _password, string _email, double _balance = 0, string _forgot_password_key = "");
  string getAccountType() const override;
};

class CurrentAccount : public Account
{
public:
  CurrentAccount(string _cnic, string _username, string _password, string _email, double _balance = 0, string _forgot_password_key = "");
  string getAccountType() const override;
};

#endif