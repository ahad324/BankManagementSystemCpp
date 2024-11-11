#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../include/EmailUtils.h"
#include "../include/ConsoleUtils.h"
#include "../include/Menu.h"
#include "../include/Constants.h"
#include "../include/JavascriptUtils.h"

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
      if (result == "SUCCESS")
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
