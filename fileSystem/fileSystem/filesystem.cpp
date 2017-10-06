#include "filesystem.h"

FileSystem::FileSystem() {
	this->nrOfUsed = 0;
	this->currDir = "/";

	createFile("/aaa/testfile1", "test");
	createFile("/testfile2", "test");
}

FileSystem::~FileSystem() {

}


/* Please insert your code */

void FileSystem::createFile(const std::string &filePath , const std::string &input)
{
	if (this->mMemblockDevice.spaceLeft() > 0)
	{
		std::string fp = filePath;
		std::string i = input;
		while (fp.size() < 512)
			fp += ' ';
		while (i.size() < 512)
			i += ' ';

		this->mMemblockDevice.writeBlock(this->nrOfUsed, fp);
		this->mMemblockDevice.writeBlock(this->mMemblockDevice.size() - this->nrOfUsed, i);

		this->nrOfUsed++;
	}
		
}

void FileSystem::createFolder(const std::string &folderPath)
{
	if (this->mMemblockDevice.spaceLeft() > 0)
	{
		std::string fp = folderPath;
		if(fp[fp.length() - 1] != '/')
			fp += '/';
		while (fp.size() < 512)
			fp += ' ';

		this->mMemblockDevice.writeBlock(this->nrOfUsed, fp);
		this->mMemblockDevice.writeBlock(this->mMemblockDevice.size() - this->nrOfUsed, "#F");

		this->nrOfUsed++;
	}
}

void FileSystem::removeFile(const std::string & filePath)
{
	for (int i = 0; i < this->nrOfUsed; i++)
	{
		if (filePath == this->mMemblockDevice[i].toString())
		{
			this->mMemblockDevice[i].reset();
			this->mMemblockDevice[this->mMemblockDevice.size() - i].reset();
			
			//Unnecessary if we remove the last file in memory
			this->mMemblockDevice.writeBlock(i, this->mMemblockDevice[this->nrOfUsed].toString());
			this->mMemblockDevice.writeBlock(this->mMemblockDevice.size() - i, this->mMemblockDevice[this->mMemblockDevice.size() - this->nrOfUsed].toString());

			this->nrOfUsed--;
		}
	}
}


//Wil not clear floder first, use the ListDir algoritm
void FileSystem::removeFolder(const std::string & folderPath)
{
	for (int i = 0; i < this->nrOfUsed; i++)
	{
		if (folderPath == this->mMemblockDevice[i].toString())
		{
			this->mMemblockDevice[i].reset();
			this->mMemblockDevice[this->mMemblockDevice.size() - i].reset();

			//Unnecessary if we remove the last file in memory
			this->mMemblockDevice.writeBlock(i, this->mMemblockDevice[this->nrOfUsed].toString());
			this->mMemblockDevice.writeBlock(this->mMemblockDevice.size() - i, this->mMemblockDevice[this->mMemblockDevice.size() - this->nrOfUsed].toString());

			this->nrOfUsed--;
		}
	}
}

void FileSystem::goToFolder(const std::string &folderPath)
{
	// Limit to available folders
	this->currDir = folderPath;
}

std::string FileSystem::listDir()
{
	std::string allElements = "";
	int length				= this->currDir.length();

	for (int i = 0; i < this->nrOfUsed; i++)
	{
		for (int j = 0; j < length; j++)
		{
			// Check to see if in current directory
			if (this->mMemblockDevice[i].toString()[j] == this->currDir[j])
			{
				if (j == length - 1)
				{
					// VV FORTSÄTT HÄR VVV (root funkar inte än)
					j += 1;
					while (this->mMemblockDevice[i].toString()[j] != '/' &&
						this->mMemblockDevice[i].toString()[j] != ' ' &&
						j < this->mMemblockDevice[i].size())
						j++;

					//Can implement diffrence between FOLDER and file
					if (this->mMemblockDevice[i].toString()[j] != ' ')
					{
						int k = j + 1;
						while (this->mMemblockDevice[i].toString()[k] != ' ')
						{
							allElements += this->mMemblockDevice[i].toString()[k];
							k++;
						}
						allElements += '\n';
					}

				}
			}
			else
				break;		// NYTT
		}
	}

	return allElements;
}

