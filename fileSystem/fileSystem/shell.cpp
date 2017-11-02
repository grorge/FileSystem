#include <iostream>
#include <sstream>
#include <crtdbg.h>
#include <fstream>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 15;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
bool quit();
std::string help();

/* More functions ... */
void formatDisk(FileSystem &fs);
void listDir(FileSystem &fs);
void createFile(FileSystem &fs, const std::string userCommand);
void printFile(FileSystem &fs , const std::string userCommand);
void save(FileSystem &fs, const std::string userCommand);
void load(FileSystem &fs, const std::string userCommand);
void removeFile(FileSystem &fs, const std::string userCommand);
void copyFile(FileSystem &fs, const std::string source, const std::string destination);
void makeDir(FileSystem &fs, const std::string userCommand);
void changeDir(FileSystem &fs, const std::string userCommand);



int main(void) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output

	FileSystem fs;

	/*  CONTENTS IN DEFAULT
	fs.createFolder("/aaa/");
	fs.createFolder("/bbb/");
	fs.createFile("/bbb/testfile3", "test3");
	fs.createFile("/aaa/testfile1", "test1");
	fs.createFile("/testfile2", "test2");
	*/

    bool bRun = true;

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

			case 0: //quit
				bRun = quit();                
                break;
            case 1: // format
				formatDisk(fs); 
				currentDir = fs.currDir;
                break;
            case 2: // ls
                std::cout << "Listing directory" << std::endl;
				listDir(fs);
                break;
            case 3: // create
				createFile(fs, commandArr[1]);
                break;
            case 4: // cat
				printFile(fs, commandArr[1]);
                break;
            case 5: // createImage
				save(fs, commandArr[1]);
                break;
            case 6: // restoreImage
				load(fs, commandArr[1]);
                break;
            case 7: // rm
				removeFile(fs, commandArr[1]);
                break;
            case 8: // cp
				copyFile(fs, commandArr[1], commandArr[2]);
                break;
            case 9: // append
                break;
            case 10: // mv
                break;
            case 11: // mkdir
				makeDir(fs, commandArr[1]);
                break;
            case 12: // cd
				changeDir(fs, commandArr[1]);
				currentDir = fs.currDir;
                break;
            case 13: // pwd
				std::cout << currentDir << std::endl;
                break;
            case 14: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);

    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

bool quit() {
	std::cout << "Exiting\n";
	return false;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}

void formatDisk(FileSystem &fs)
{
	fs = FileSystem();
}

void listDir(FileSystem &fs)
{
	std::cout << std::endl;
	std::cout << fs.listDir() << std::endl;
}

void createFile(FileSystem &fs, const std::string userCommand)
{	
	std::cout << "Data: ";
	std::string fileData;
	getline(std::cin, fileData);
	fs.createFile(userCommand, fileData);
	std::cout << std::endl;
}

void printFile(FileSystem &fs, const std::string userCommand)
{
	std::cout << "Found data: ";
	std::cout << fs.printFile(userCommand) << std::endl;
}

void save(FileSystem &fs, const std::string userCommand)
{
	std::ofstream saveFile(userCommand);

	if(saveFile.is_open())
		saveFile << fs.toString();

	saveFile.close();
}

void load(FileSystem &fs, const std::string userCommand)
{
	std::string line;
	std::ifstream loadFile(userCommand);
	formatDisk(fs);

	for (int i = 0; i < 250 && loadFile.is_open(); i++)
	{
		std::getline(loadFile, line);
		fs.insert(line, i);
	}
	fs.calcNrOfUsed();
	loadFile.close();
}

void removeFile(FileSystem &fs, const std::string userCommand)
{
	fs.removeFile(userCommand);
}

void copyFile(FileSystem &fs, const std::string source, const std::string desination)
{
	fs.createFile(desination, fs.printFile(source));
}

void makeDir(FileSystem &fs, const std::string userCommand)
{
	fs.createFolder(userCommand);
}

void changeDir(FileSystem &fs, const std::string userCommand)
{
	fs.goToFolder(userCommand);
}

/* Insert code for your shell functions and call them from the switch-case */

