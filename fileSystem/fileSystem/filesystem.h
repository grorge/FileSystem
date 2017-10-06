#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"

class FileSystem
{
private:
    MemBlockDevice mMemblockDevice;
    // Here you can add your own data structures
	int nrOfUsed;
	std::string currDir;
public:
    FileSystem();
    ~FileSystem();

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
	void createFile(const std::string &filePath, const std::string &input);

    /* Creates a folder in the filesystem */
	void createFolder(const std::string &folderPath);

    /* Removes a file in the filesystem */
    void removeFile(const std::string &filePath);

    /* Removes a folder in the filesystem */
    void removeFolder(const std::string &folderPath);

    /* Function will move the current location to a specified location in the filesystem */
    void goToFolder(const std::string &folderPath);

    /* This function will get all the files and folders in the specified folder */
    std::string listDir();


    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H