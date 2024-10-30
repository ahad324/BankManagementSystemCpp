#include "include/Menu.h"
#include "include/ConsoleUtils.h"
#include "include/Dependencies.h"

using namespace std;

int main()
{
    setConsoleTitle("AUT Finance Bank");
    if (checkAndInstallDependencies())
    {
        start();
    }
    else
    {
        return 1; // Exit if dependencies are not installed
    }

    return 0;
}
