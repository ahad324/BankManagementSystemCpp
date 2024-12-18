#ifndef BANK_H
#define BANK_H

#include <unordered_map>
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Bank
{
private:
  unordered_map<string, Account *> accounts;
  unordered_map<string, Account *> pendingAccounts;
  unordered_map<string, string> userCache;  // Cache for user CNICs and passwords
  unordered_map<string, string> adminCache; // Cache for admin usernames and passwords

public:
  Bank();
  ~Bank();

  void LoadAllAccountsFromBackend();
  void loadAccountsFromBackend();
  void loadPendingAccountsFromBackend();
  bool authenticateAdmin(string username, string password);
  Account *authenticateUser(string cnic, string password);
  void createAccount(Account *account, bool isPending = false);
  void changeUserPassword(string cnic, string new_password);
  void changeUserUsername(string cnic, string new_username, Account *account);
  void displayAccountsInTable(const unordered_map<string, Account *> &accountMap);
  void displaySortedDataInTable(const vector<pair<string, Account *>> &accountVec);
  void sortAccounts(unordered_map<string, Account *> &accountMap, const string &sortBy);
  void searchAccount(unordered_map<string, Account *> &accountMap, const string &searchBy, const string &value);
  void approveSelectedAccount();
  void removeSelectedAccount();
  void approvePendingAccounts();
  void viewAllAccounts();
  void removeAccount();
  bool isNumeric(const string &str);
  double getValidatedAmount();
  string getPasswordInput();
  void transferMoney(Account *from_account);
  Account *handleSuccessfulLogin(const string &cnic);
  void login();
  void forgotPassword();
  void RecoverPasswordWithCNICAndKey();
  void RecoverPasswordWithEmailAndOTP();
  bool CheckCNIC(string cnic);
  bool CheckEmail(string email);
  int SendOTP(string email, string reason);
  bool verifyOtp(string email, string reason);
  string generateRandomKey(int length);
  bool checkExit(const string &input);
  void createAccount();
  void adminLogin();
  void adminMenu();
  void userMenu(Account *account);
  void About();
};

extern Bank bank;

#endif