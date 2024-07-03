#include <cstdlib>
#include <conio.h>
int main()
{
  // Run the batch file as administrator
  system("powershell.exe Start-Process 'cmd.exe' -ArgumentList '/c \"./SetExecutionPolicy.bat\"' -Verb RunAs");
  return 0;
}
