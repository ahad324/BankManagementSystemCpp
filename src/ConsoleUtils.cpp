#include "../include/ConsoleUtils.h"
#include "../include/Constants.h"
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

// <===================== 🎨 FUNCTIONS TO SET COLORS FOR DIFFERENT STATEMENTS 🌈 =====================>
void setConsoleForegroundColor(int colorCode)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, colorCode);
}
void PrintColoredText(const string &text, int color)
{
  setConsoleForegroundColor(color);
  cout << Spaces_to_be_Centered << text << endl;
  setConsoleForegroundColor(DefaultColorCode);
}
void PrintColoredTittle(const string &text, int color)
{
  setConsoleForegroundColor(color);
  cout << "\t\t\t\t\t\t" << text << endl;
  setConsoleForegroundColor(DefaultColorCode);
}
void PrintErrorsORSucess(const string &text, int color)
{
  setConsoleForegroundColor(color);
  cout << Spaces_to_be_Centered;
  for (char c : text)
  {
    cout << c;
    Sleep(10); // Sleep for 10 milliseconds
  }
  cout << endl;
  setConsoleForegroundColor(DefaultColorCode);
}
void setConsoleTitle(const char *title)
{
  SetConsoleTitleA(title);
}