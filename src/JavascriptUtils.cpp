#include <string>
#include <fstream>
#include <sstream>

using namespace std;
// <===================== 🌐 FUNCTIONS FOR FETCHING DATA FROM BACKEND 📡 =====================>
string callJavaScript(const string &command)
{
  string cmd = "server.exe " + command + " > output.txt";
  system(cmd.c_str());

  ifstream file("output.txt");
  string result;
  getline(file, result);
  file.close();

  ofstream clearFile("output.txt", ios::trunc);
  clearFile.close();
  return result;
}
string FetchAllLargeDataFromJavaScript(const string &command)
{
  string cmd = "server.exe " + command + " > output.txt";
  system(cmd.c_str());

  ifstream file("output.txt");
  stringstream buffer;
  buffer << file.rdbuf();
  string result = buffer.str();
  file.close();

  ofstream clearFile("output.txt", ios::trunc);
  clearFile.close();

  return result;
}