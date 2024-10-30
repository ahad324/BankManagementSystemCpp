#include <limits>
#include <conio.h>
#include <iostream>
#include "Constants.h"
#include "../include/Menu.h"
#include "../include/Bank.h"
#include "../include/ConsoleUtils.h"

using namespace std;

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
      cin.clear();                                         // Clear the error flag
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
      _getch();                                            // Wait for user input
      break;
    }
  }
}