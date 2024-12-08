#include <limits>
#include <conio.h>
#include <vector>
#include <iostream>
#include "Constants.h"
#include "../include/Menu.h"
#include "../include/Bank.h"
#include "../include/ConsoleUtils.h"

using namespace std;

void ShowMenuOptions(const vector<string> &options)
{
  for (size_t i = 0; i < options.size(); i++)
  {
    setConsoleForegroundColor(TittleColorCode);
    cout << Spaces_to_be_Centered << "[" << i + 1 << "]" << "- ";
    setConsoleForegroundColor(DefaultColorCode);
    cout << options[i] << endl;
  }
}

void InputTaking(const string &text)
{
  setConsoleForegroundColor(TittleColorCode);
  cout << Spaces_to_be_Centered << text << ": ";
  setConsoleForegroundColor(DefaultColorCode);
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
    vector<string> options = {"Login", "Forgot Password", "Create Account", "Admin Login", "About Us", "Exit"};
    ShowMenuOptions(options);

    cout << "\n";
    int choice;
    InputTaking("Enter your choice");
    cin >> choice;

    switch (choice)
    {
    case 1:
      bank.login();
      break;
    case 2:
      bank.forgotPassword();
      break;
    case 3:
      bank.createAccount();
      break;
    case 4:
      bank.adminLogin();
      break;
    case 5:
      bank.About();
      break;
    case 6:
      return;
    default:
      PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      _getch();
      break;
    }
  }
}