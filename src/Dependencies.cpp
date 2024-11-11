#include "../include/Dependencies.h"
#include "../include/ConsoleUtils.h"
#include "../include/Constants.h"
#include "../include/Menu.h"
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>

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
  unordered_map<string, bool> dependencies = {
      {"npm", checkDependency("npm -v > nul 2>&1", "npm")},
      {"Node.js", checkDependency("node -v > nul 2>&1", "Node.js")},
      {"Appwrite", checkDependency("npm list appwrite > nul 2>&1", "Appwrite")},
      {"Argon2", checkDependency("npm list argon2 > nul 2>&1", "Argon2")},
      {"Nodemailer", checkDependency("npm list nodemailer > nul 2>&1", "Nodemailer")}};

  if (!dependencies["Node.js"] && !dependencies["npm"])
  {
    PrintErrorsORSucess("Node.js or npm is required to run this program. Please download and install Node.js from https://nodejs.org/en/.", ErrorMessagesColorCode);
    cout << "Press Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return false;
  }

  if (!dependencies["Appwrite"] || !dependencies["Argon2"])
  {
    char choice;
    InputTaking("Some packages are missing. Would you like to install them? (y/n)");
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
      if (!dependencies["Appwrite"])
      {
        installDependency("npm install -g appwrite", "Appwrite");
      }
      if (!dependencies["Argon2"])
      {
        installDependency("npm install argon2", "Argon2");
      }
      if (!dependencies["Nodemailer"])
      {
        installDependency("npm install nodemailer", "Nodemailer");
      }
      return true;
    }
    else
    {
      if (!dependencies["Appwrite"])
      {
        PrintColoredText("Appwrite is required to run this program. Exiting...", ErrorMessagesColorCode);
      }
      if (!dependencies["Argon2"])
      {
        PrintColoredText("Argon2 is required to run this program. Exiting...", ErrorMessagesColorCode);
      }
      if (!dependencies["Nodemailer"])
      {
        PrintColoredText("Nodemailer is required to run this program. Exiting...", ErrorMessagesColorCode);
      }
      return false;
    }
  }

  PrintColoredText("All dependencies are installed.", SuccessMessagesColorCode);
  return true;
}