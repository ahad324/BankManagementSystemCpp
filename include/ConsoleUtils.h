#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <string>
using namespace std;

void PrintErrorsORSucess(const string &message, int colorCode);
void PrintColoredText(const string &message, int colorCode);
void setConsoleTitle(const char *title);
void PrintColoredTittle(const string &text, int color);
void setConsoleForegroundColor(int colorCode);

#endif // CONSOLE_UTILS_H