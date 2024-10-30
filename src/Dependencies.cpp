#include "../include/Dependencies.h"
#include "../include/ConsoleUtils.h"
#include "../include/Constants.h"
#include "../include/Menu.h"
#include <iostream>
#include <limits>

using namespace std;

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
  bool nodemailerInstalled = checkDependency("npm list nodemailer > nul 2>&1", "Nodemailer");

  if (!nodeInstalled && !npmInstalled)
  {
    PrintErrorsORSucess("Node.js or npm is required to run this program. Please download and install Node.js from https://nodejs.org/en/.", ErrorMessagesColorCode);
    cout << "Press Enter to exit...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
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
      if (!nodemailerInstalled)
      {
        installDependency("npm install nodemailer", "Nodemailer");
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
      if (!nodemailerInstalled)
      {
        PrintColoredText("Nodemailer is required to run this program. Exiting...", ErrorMessagesColorCode);
      }
      return false;
    }
  }

  PrintColoredText("All dependencies are installed.", SuccessMessagesColorCode);
  return true;
}