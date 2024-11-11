#include "include/Menu.h"
#include "include/ConsoleUtils.h"
#include "include/Dependencies.h"

using namespace std;

int main()
{
    if (checkAndInstallDependencies())
    {
        setConsoleTitle("AUT Finance Bank");
        start();
    }
    else
    {
        return 1; // Exit if dependencies are not installed
    }

    return 0;
}