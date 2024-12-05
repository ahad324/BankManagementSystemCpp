#include <iostream>
#include <sstream>
#include <map>
#include <unordered_set>
#include <string>
#include <conio.h>
#include <regex>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include "../include/Bank.h"
#include "../include/Account.h"
#include "../include/ConsoleUtils.h"
#include "../include/Constants.h"
#include "../include/JavascriptUtils.h"
#include "../include/EmailUtils.h"
#include "../include/Menu.h"

using namespace std;

Bank bank;
Bank::Bank() {}

Bank::~Bank()
{
  for (auto &pair : accounts)
  {
    delete pair.second;
  }
  for (auto &pair : pendingAccounts)
  {
    delete pair.second;
  }
}
// <===================== ✨🔃 Data Retrieving from backend 🔃🌟 =====================>
void Bank::LoadAllAccountsFromBackend()
{
  accounts.clear();
  pendingAccounts.clear();
  loadAccountsFromBackend();
  loadPendingAccountsFromBackend();
}
void Bank::loadAccountsFromBackend()
{
  string command = "FetchAccounts"; // Command to fetch all user details
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
    string username, email, accountType, cnic, key;
    double balance;

    lineStream >> username >> email >> accountType >> balance >> cnic >> key;

    // Create account objects based on the account type
    Account *account = nullptr;
    if (accountType == "Savings")
    {
      account = new SavingsAccount(cnic, username, "", email, balance, key);
    }
    else
    {
      account = new CurrentAccount(cnic, username, "", email, balance, key);
    }

    // Store account in map
    accounts[cnic] = account;
  }

  // Displaying the fetched result for verification
  // cout << "Press any key to continue...";
  // _getch();
}

void Bank::loadPendingAccountsFromBackend()
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
      pendingaccount = new SavingsAccount(cnic, username, "", email);
      ;
    }
    else
    {
      pendingaccount = new CurrentAccount(cnic, username, "", email);
    }

    // Store pending account in map
    pendingAccounts[cnic] = pendingaccount;
  }
}
// <===================== 🔐👥 User and Admin AUTH 🔐👥 =====================>
bool Bank::authenticateAdmin(string username, string password)
{
  PrintErrorsORSucess("Proccessing your request...", WaitingMessagesColorCode);
  // Check if the admin is in the cache
  auto cachedPassword = adminCache.find(username);
  if (cachedPassword != adminCache.end() && cachedPassword->second == password)
  {
    return true;
  }

  // If not in cache, call JavaScript function
  string command = "AdminLogin " + username + " " + password;
  string result = callJavaScript(command);
  if (result == "SUCCESS")
  {
    adminCache[username] = password; // Cache the admin's password
    return true;
  }
  return false;
}

Account *Bank::handleSuccessfulLogin(const string &cnic)
{
  PrintErrorsORSucess("Logging in...", WaitingMessagesColorCode);
  accounts.clear();
  loadAccountsFromBackend();
  PrintErrorsORSucess("Getting your dashboard ready...", WaitingMessagesColorCode);
  auto it = accounts.find(cnic);
  if (it != accounts.end())
  {
    return it->second;
  }
  return nullptr;
}

Account *Bank::authenticateUser(string cnic, string password)
{
  PrintErrorsORSucess("Proccessing your request...", WaitingMessagesColorCode);
  // Check if the user is in the cache
  auto cachedPassword = userCache.find(cnic);
  if (cachedPassword != userCache.end() && cachedPassword->second == password)
  {
    // cout << "Using Caching\n"; // Debugging line
    return handleSuccessfulLogin(cnic);
  }
  // If not in cache, call JavaScript function
  string command = "UserLogin " + cnic + " " + password;
  string result = callJavaScript(command);

  if (result == "SUCCESS")
  {
    userCache[cnic] = password; // Cache the user's password
    return handleSuccessfulLogin(cnic);
  }
  // If user not found , return nullptr
  return nullptr;
}
void Bank::createAccount(Account *account, bool isPending)
{
  if (isPending)
  {
    pendingAccounts[account->getCNIC()] = account;
  }
  else
  {
    accounts[account->getCNIC()] = account;
  }
}

void Bank::changeUserPassword(string cnic, string new_password)
{
  PrintErrorsORSucess("Hold on changing your Password...", WaitingMessagesColorCode);
  string command = "ChangeUserPassword " + cnic + " " + new_password;
  string result = callJavaScript(command);

  PrintErrorsORSucess("Password Changed Successfully\n", SuccessMessagesColorCode);
  PrintColoredText("Press any key to continue...", PressKeyColorCode);
  _getch(); // Wait for user input
}
void Bank::changeUserUsername(string cnic, string new_username, Account *account)
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
// Function to display accounts in table format
void Bank::displayAccountsInTable(const map<string, Account *> &accountMap)
{
  // Print table header
  cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+-------------------+\n";
  cout << "\t| No |         CNIC          |       Username       |         Email        |      Balance      |    Account Type   |\n";
  cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+-------------------+\n";

  int i = 1;
  // Print account details
  for (const auto &pair : accountMap)
  {
    cout << "\t| " << setw(2) << i++ << " | "
         << setw(19) << pair.second->getCNIC() << " | "
         << setw(20) << pair.second->getUsername() << " | "
         << setw(25) << pair.second->getEmail() << " | "
         << setw(10) << left << "$" + to_string(pair.second->getBalance()) << " | "
         << setw(10) << pair.second->getAccountType() << " |\n";
    cout << "\t+----+-----------------------+----------------------+----------------------+-------------------+-------------------+\n";
  }
}
void Bank::displaySortedDataInTable(const vector<pair<string, Account *>> &accountVec)
{
  // Print table header
  cout << "\t+---------------------+----------------------+---------------------------+--------------------+----------------------+\n";
  cout << "\t|        CNIC         |       Username       |           Email           |      Balance       |    Account Type    |\n";
  cout << "\t+---------------------+----------------------+---------------------------+--------------------+--------------------+\n";

  // Print account details
  for (const auto &pair : accountVec)
  {
    cout << "\t| " << setw(19) << left << pair.second->getCNIC()
         << " | " << setw(20) << left << pair.second->getUsername()
         << " | " << setw(25) << left << pair.second->getEmail()
         << " | " << setw(10) << left << "$" + to_string(pair.second->getBalance())
         << " | " << setw(10) << left << pair.second->getAccountType()
         << " |\n";
    cout << "\t+---------------------+----------------------+---------------------------+--------------------+------------+\n";
  }
}

void Bank::sortAccounts(map<string, Account *> &accountMap, const string &sortBy)
{
  // Create a vector of pairs to hold the sorted keys and their corresponding Account objects
  vector<pair<string, Account *>> accountVec;
  for (const auto &pair : accountMap)
  {
    accountVec.push_back(pair); // Add the CNIC (key) and the Account object to the vector
  }

  // Sort the vector based on the specified criterion
  if (sortBy == "Balance")
  {
    sort(accountVec.begin(), accountVec.end(),
         [](const pair<string, Account *> &a, const pair<string, Account *> &b)
         {
           return a.second->getBalance() > b.second->getBalance(); // Sort by balance
         });
  }
  else if (sortBy == "CNIC")
  {
    sort(accountVec.begin(), accountVec.end(),
         [](const pair<string, Account *> &a, const pair<string, Account *> &b)
         {
           return a.first < b.first; // Sort by CNIC
         });
  }
  else if (sortBy == "Name")
  {
    sort(accountVec.begin(), accountVec.end(),
         [](const pair<string, Account *> &a, const pair<string, Account *> &b)
         {
           return a.second->getUsername() < b.second->getUsername(); // Sort by name
         });
  }

  // Pass the sorted vector to the display function
  displaySortedDataInTable(accountVec);
}

// Generalized search function (works for both accounts and pending accounts)
void Bank::searchAccount(map<string, Account *> &accountMap, const string &searchBy, const string &value)
{
  bool found = false;

  for (const auto &pair : accountMap)
  {
    if ((searchBy == "CNIC" && pair.second->getCNIC() == value) ||
        (searchBy == "Name" && pair.second->getUsername() == value))
    {
      found = true;
      // Display account
      map<string, Account *> matchingAccounts;
      matchingAccounts[pair.first] = pair.second;
      displayAccountsInTable(matchingAccounts);
      break;
    }
  }

  if (!found)
  {
    PrintErrorsORSucess("No account found with that " + searchBy + ".", ErrorMessagesColorCode);
  }
}

void Bank::approveSelectedAccount()
{
  int choice;
  InputTaking("Enter the number of the account to approve (0 to exit)");
  cin >> choice;

  if (choice > 0 && choice <= pendingAccounts.size())
  {
    auto it = pendingAccounts.begin();
    advance(it, choice - 1); // Move iterator to the selected position
    string cnic = it->second->getCNIC();

    // Move account to the main accounts list
    string command = "MoveToAccounts " + cnic;
    string result = callJavaScript(command);

    if (result == "SUCCESS")
    {
      // Add the account to the main accounts map and remove it from pending accounts
      accounts[cnic] = it->second;
      pendingAccounts.erase(it);
      PrintErrorsORSucess("Account approved and moved to main accounts.", SuccessMessagesColorCode);
    }
    else
    {
      PrintErrorsORSucess("Failed to approve account.", ErrorMessagesColorCode);
    }
  }
  else
  {
    PrintErrorsORSucess("Invalid account number. Exiting...", ErrorMessagesColorCode);
  }
}
void Bank::removeSelectedAccount()
{
  int choice;
  InputTaking("Enter the number of the account to approve (0 to exit)");
  cin >> choice;
  if (choice > 0 && choice <= accounts.size())
  {
    auto it = accounts.begin();
    advance(it, choice - 1); // Move iterator to the selected position
    string command = "DeleteAccount " + it->second->getCNIC();
    string result = callJavaScript(command);

    if (result == "SUCCESS")
    {
      delete it->second;  // Free memory for the account object
      accounts.erase(it); // Remove from the map
      PrintErrorsORSucess("Account removed.\n", SuccessMessagesColorCode);
    }
    else
    {
      PrintErrorsORSucess("Failed to delete account. Make sure you have an active internet connection.", ErrorMessagesColorCode);
    }
  }
  else
  {
    PrintErrorsORSucess("Invalid account number. Exiting...", ErrorMessagesColorCode);
  }
  cout << "\n";
  PrintColoredText("Press any key to continue...", PressKeyColorCode);
  _getch(); // Wait for user input
}

void Bank::approvePendingAccounts()
{
  pendingAccounts.clear();
  cout << "\n";
  PrintErrorsORSucess("Fetching Accounts...", WaitingMessagesColorCode);
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

    displayAccountsInTable(pendingAccounts);
    cout << "\n";

    // Show options after displaying pending accounts
    vector<string> options = {"Search by CNIC", "Search by Name", "Approve Account", "Clear Screen", "Exit"};
    int choice;
    while (true)
    {
      InputTaking("Enter your choice");
      for (auto i = 0; i < options.size(); i++)
      {
        ShowMenuOptions(i + 1, options[i]);
      }
      cin >> choice;
      switch (choice)
      {
      case 1:
      {
        string cnic;
        InputTaking("Enter CNIC to search for");
        cin >> cnic;
        searchAccount(pendingAccounts, "CNIC", cnic);
        break;
      }
      break;
      case 2:
      {
        string name;
        InputTaking("Enter Name to search for");
        cin >> name;
        searchAccount(pendingAccounts, "Name", name);
        break;
      }
      case 3:
        approveSelectedAccount();
        break;
      case 4:
        system("cls");
        break;
      case 5:
        return;
      default:
        PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
        break;
      }
    }
  }
}
void Bank::viewAllAccounts()
{
  accounts.clear();
  PrintErrorsORSucess("Fetching Accounts...", WaitingMessagesColorCode);
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

    // Call the function to display accounts
    displayAccountsInTable(accounts);

    // Show options after displaying accounts
    vector<string> options = {"Sort by Balance", "Sort by CNIC", "Sort by Name", "Search by CNIC", "Search by Name", "Clear Screen", "Exit"};
    int choice;
    while (true)
    {
      for (auto i = 0; i < options.size(); i++)
      {
        ShowMenuOptions(i + 1, options[i]);
      }
      InputTaking("Enter your choice");
      cin >> choice;

      switch (choice)
      {
      case 1:
        sortAccounts(accounts, "Balance");
        break;
      case 2:
        sortAccounts(accounts, "CNIC");
        break;
      case 3:
        sortAccounts(accounts, "Name");
        break;
      case 4:
      {
        string cnic;
        InputTaking("Enter CNIC to search for");
        cin >> cnic;
        searchAccount(accounts, "CNIC", cnic);
      }
      break;
      case 5:
      {
        string name;
        InputTaking("Enter Name to search for");
        cin >> name;
        searchAccount(accounts, "Name", name);
      }
      break;
      case 6:
        system("cls");
        break;
      case 7:
        return;
      default:
        PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
        break;
      }
    }
  }
}

void Bank::removeAccount()
{
  accounts.clear();
  cout << "\n";
  PrintErrorsORSucess("Fetching Accounts...", WaitingMessagesColorCode);
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

    displayAccountsInTable(accounts);

    // Show options after displaying accounts
    vector<string> options = {"Sort by Balance", "Sort by CNIC", "Sort by Name", "Search by CNIC", "Search by Name", "Remove Account", "Clear Screen", "Exit"};
    int choice;
    while (true)
    {
      InputTaking("Enter your choice");
      for (auto i = 0; i < options.size(); i++)
      {
        ShowMenuOptions(i + 1, options[i]);
      }
      cin >> choice;
      switch (choice)
      {
      case 1:
        sortAccounts(accounts, "Balance");
        break;
      case 2:
        sortAccounts(accounts, "CNIC");
        break;
      case 3:
        sortAccounts(accounts, "Name");
        break;
      case 4:
      {
        string cnic;
        InputTaking("Enter CNIC to search for");
        cin >> cnic;
        searchAccount(accounts, "CNIC", cnic);
        break;
      }
      case 5:
      {
        string name;
        InputTaking("Enter Name to search for");
        cin >> name;
        searchAccount(accounts, "Name", name);
        break;
      }
      case 6:
        removeSelectedAccount();
        break;
      case 7:
        system("cls");
        break;
      case 8:
        return;
      default:
        PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
        break;
      }
    }
  }
}

bool Bank::isNumeric(const string &str)
{
  for (char const &c : str)
  {
    if (isdigit(c) == 0)
      return false;
  }
  return true;
}

double Bank::getValidatedAmount()
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

string Bank::getPasswordInput()
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

void Bank::transferMoney(Account *from_account)
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
  for (const auto &pair : accounts)
  {
    if (pair.second->getCNIC() == recipient_CNIC)
    {
      recipient_account = pair.second;
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
      stream << fixed;
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

void Bank::login()
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
void Bank::forgotPassword()
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
void Bank::RecoverPasswordWithCNICAndKey()
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
    for (const auto &pair : accounts)
    {
      if (pair.second->getCNIC() == cnic)
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
            changeUserPassword(pair.second->getCNIC(), new_password);
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
void Bank::RecoverPasswordWithEmailAndOTP()
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
    return;
  }

  if (!verifyOtp(email, "Password Recovery"))
  {
    PrintErrorsORSucess("Invalid OTP. Please try again.", ErrorMessagesColorCode);
    PrintColoredText("Press any key to continue...", PressKeyColorCode);
    _getch(); // Wait for user input
    return;
  }

  accounts.clear();
  loadAccountsFromBackend();
  for (const auto &pair : accounts)
  {
    if (pair.second->getEmail() == email)
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
          changeUserPassword(pair.second->getCNIC(), new_password);
          break;
        }
      }
      break;
    }
  }
}

bool Bank::CheckCNIC(string cnic)
{
  PrintErrorsORSucess("Please wait while your CNIC is being verified...", WaitingMessagesColorCode);
  LoadAllAccountsFromBackend();
  return accounts.find(cnic) == accounts.end() && pendingAccounts.find(cnic) == pendingAccounts.end();
}
bool Bank::CheckEmail(string email)
{
  PrintErrorsORSucess("Please wait while your EMAIL is being verified...", WaitingMessagesColorCode);
  LoadAllAccountsFromBackend();
  for (const auto &pair : accounts)
  {
    if (pair.second->getEmail() == email)
    {
      return false;
    }
  }
  for (const auto &pair : pendingAccounts)
  {
    if (pair.second->getEmail() == email)
    {
      return false;
    }
  }
  return true;
}
int Bank::SendOTP(string email, string reason)
{
  srand(time(0)); // Seed the random number generator with current time
  int code = rand() % 9000 + 1000;
  string scode = to_string(code);
  PrintErrorsORSucess("Please wait we are sending you OTP for " + reason + "...", WaitingMessagesColorCode);
  string command = "sendOTPEmail " + email + " " + scode + " \"" + reason + "\"";
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

bool Bank::verifyOtp(string email, string reason)
{
  int result = SendOTP(email, reason);
  return result == 0;
}
string Bank::generateRandomKey(int length)
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

bool Bank::checkExit(const string &input)
{
  if (input == "exit" || input == "EXIT")
  {
    PrintErrorsORSucess("Registration cancelled.", ErrorMessagesColorCode);
    PrintErrorsORSucess("Press any key to continue......", WaitingMessagesColorCode);
    _getch();
    return true;
  }
  return false;
}
void Bank::createAccount()
{
  system("cls");
  cout << "\n\n";
  PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
  PrintColoredTittle("|                                                                                     |", TittleColorCode);
  PrintColoredTittle("|                                      User Registration                              |", TittleColorCode);
  PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

  PrintColoredText("(Enter 'exit' at any prompt to cancel registration)\n\n", ErrorMessagesColorCode);

  string command;
  string cnic, username, password, email, account_type;
  double initial_deposit = 0;

  while (true)
  {
    InputTaking("Enter CNIC (xxxxx-xxxxxxx-x)");
    cin >> cnic;

    if (checkExit(cnic))
      return;
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
      break; // Cnic is unique, exit the loop
    }
  }

  while (true)
  {
    InputTaking("Enter username");
    cin >> username;

    if (checkExit(username))
      return;
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

    if (checkExit(password))
      return;
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

    if (checkExit(email))
      return;
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

  srand(time(0));     // Seeding the random number generator
  int key_length = 5; // Length of the random key
  string forgot_password_key = generateRandomKey(key_length);
  while (true)
  {
    InputTaking("Enter account type (Savings 'S' /Current 'C')");
    cin >> account_type;

    if (checkExit(account_type))
      return;
    if (account_type == "S" || account_type == "s")
    {
      createAccount(new SavingsAccount(cnic, username, password, email, initial_deposit, forgot_password_key), true);
      break;
    }
    else if (account_type == "C" || account_type == "c")
    {
      createAccount(new CurrentAccount(cnic, username, password, email, initial_deposit, forgot_password_key), true);
      break;
    }
    else
    {
      PrintErrorsORSucess("Invalid account type. Please try again.", ErrorMessagesColorCode);
    }
  }
  cout << "\n";
  PrintColoredTittle("-------------------------------------------------------------------------------------", ErrorMessagesColorCode);
  PrintColoredTittle("|     Save this secret key. You'll need it if you forget your password : " + forgot_password_key + "      |", SuccessMessagesColorCode);
  PrintColoredTittle("-------------------------------------------------------------------------------------", ErrorMessagesColorCode);
  cout << "\n";
  command = "forgot_password " + cnic + " " + forgot_password_key + " " + "create";
  callJavaScript(command);

  PrintErrorsORSucess("Hold on creating your account.", WaitingMessagesColorCode);

  command = "Register " + username + " " + password + " " + email + " " + account_type + " " + cnic + " " + forgot_password_key;
  callJavaScript(command);
  PrintErrorsORSucess("Account created successfully. Pending admin approval.", SuccessMessagesColorCode);
  cout << "\n";
  PrintColoredText("Press any key to continue...", PressKeyColorCode);
  _getch(); // Wait for user input
}

void Bank::adminLogin()
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

void Bank::adminMenu()
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

void Bank::userMenu(Account *account)
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
      for (const auto &pair : accounts)
      {
        if (pair.second->getCNIC() == account->getCNIC())
        {
          account->setBalance(pair.second->getBalance());
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
void Bank::About()
{
  system("cls");

  cout << "\n\n";
  PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
  PrintColoredTittle("|                                                                                     |", TittleColorCode);
  PrintColoredTittle("|                                      About Us                                       |", TittleColorCode);
  PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
  cout << "\n";

  // Team Section
  PrintColoredText("Who We Are", TittleColorCode);
  cout << "\n";
  PrintErrorsORSucess("AUT Finance Bank is a dynamic and innovative project developed to simplify financial", DefaultColorCode);
  PrintErrorsORSucess("operations with the power of technology. Our mission is to empower users by providing", DefaultColorCode);
  PrintErrorsORSucess("secure and seamless banking services, all while setting new benchmarks in user experience.", DefaultColorCode);
  cout << "\n\n";

  PrintColoredText("Meet the Team", TittleColorCode);
  cout << "\n";

  string Names[] = {
      " [1] AbdulAhad Sadiq",
      " [2] Taimoor Hassan",
      " [3] Usman Ali",
      " [4] Ali Hassan",
  };
  for (const auto &name : Names)
  {
    PrintErrorsORSucess(name, HighlightColorCode);
  }
  cout << "\n";

  // Project Details Section
  PrintColoredText("What Makes Us Stand Out", TittleColorCode);
  cout << "\n";
  PrintErrorsORSucess("Advanced OOP & DSA: Utilizing C++ to create a robust and", DefaultColorCode);
  PrintErrorsORSucess("    efficient frontend system.", DefaultColorCode);
  PrintErrorsORSucess("Seamless Backend Integration: Powered by Node.js for real-time performance.", DefaultColorCode);
  PrintErrorsORSucess("Secure Cloud-Based Database: With Appwrite, we ensure your data remains", DefaultColorCode);
  PrintErrorsORSucess("    protected while offering global accessibility.", DefaultColorCode);
  cout << "\n";

  // Acknowledgment Section
  PrintColoredText("Our Commitment", TittleColorCode);
  cout << "\n";
  PrintErrorsORSucess("Your trust drives us forward. At AUT Finance Bank, we strive to deliver exceptional", DefaultColorCode);
  PrintErrorsORSucess("services, combining state-of-the-art technology with a user-first approach.", DefaultColorCode);
  cout << "\n\n";

  PrintErrorsORSucess("Thank you for choosing AUT Finance Bank. Together, let's shape the future of banking!", SuccessMessagesColorCode);
  cout << "\n\n";

  PrintErrorsORSucess("Press any key to return to the main menu...", PressKeyColorCode);
  _getch(); // Wait for user input
}
