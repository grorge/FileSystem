#include "filesystem.h"

FileSystem::FileSystem() {
	this->nrOfUsed = 0;
	this->currDir = "/";
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
		this->mMemblockDevice.writeBlock(this->mMemblockDevice.size() - this->nrOfUsed - 1, i);

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
		this->mMemblockDevice.writeBlock(this->mMemblockDevice.size() - this->nrOfUsed - 1, this->fill("#F"));

		this->nrOfUsed++;
	}
}

void FileSystem::removeFile(const std::string & filePath)
{
	int i = this->getFileIndex(filePath);

	if (i != -1)
	{
		this->mMemblockDevice[i].reset();
		this->mMemblockDevice[this->mMemblockDevice.size() - i - 1].reset();

		//Unnecessary if we remove the last file in memory
		this->mMemblockDevice.writeBlock(i, this->mMemblockDevice[this->nrOfUsed].toString());
		this->mMemblockDevice.writeBlock(this->mMemblockDevice.size() - i - 1, this->mMemblockDevice[this->mMemblockDevice.size() - this->nrOfUsed - 1].toString());

		this->nrOfUsed--;
	}
}


//Wil not clear floder first, use the ListDir algoritm
void FileSystem::removeFolder(const std::string & folderPath)
{
	this->removeFile(folderPath);
}

void FileSystem::goToFolder(const std::string &folderPath)
{
	// Limit to available folders
	this->currDir = folderPath;
}

std::string FileSystem::listDir()
{
	//TODO: make it so it prints the folder location aswell

	std::string allElements = "";
	int length				= this->currDir.length();

	//Will go throug all used spaces
	for (int i = 0; i < this->nrOfUsed; i++)
	{
		//Will go to end of current dirrectory
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
					//checks if its a folder
					if (
						(this->mMemblockDevice[i].toString()[j] == '/' &&
						this->mMemblockDevice[i].toString()[j + 1] == ' ') ||
						this->mMemblockDevice[i].toString()[j] == ' ')
					{
						//int k = length;
						for(int k = length; k < j/*this->mMemblockDevice[i].toString()[k] != ' '*/
								; k++)
						{
							allElements += this->mMemblockDevice[i].toString()[k];
							//k++;
						}
						if (this->mMemblockDevice[i].toString()[j] == '/' &&
							this->mMemblockDevice[i].toString()[j + 1] == ' ')
						{
							allElements += '/';
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

std::string FileSystem::listAll()
{
	std::string allElements = "";

	for (int i = 0; i < this->nrOfUsed; i++)
	{
		allElements += this->mMemblockDevice[i].toString() + "\n";
	}

	return allElements;
}

std::string FileSystem::printFile(const std::string &filePath)
{
	std::string data = "error (out of bounds : not found)";

	int i = this->getFileIndex(filePath);

	// Data was found
	if (i != -1)
		data = this->vacuum(this->mMemblockDevice[250 - i - 1].toString());

	return data;
}

std::string FileSystem::vacuum(const std::string& str)
{
	int j = 0;
	std::string vacuum = "";
	while (!(str[j] == ' ' && str[j + 1] == ' '))
	{
		vacuum += str[j];
		j++;
	}
	return vacuum;
}

std::string FileSystem::fill(const std::string& str)
{
	std::string fp = str;

	while (fp.size() < 512)
		fp += ' ';

	return fp;
}

int FileSystem::getFileIndex(const std::string& filePath)
{
	std::string fp = this->fill(filePath);

	int i = 0;
	while (i < this->nrOfUsed && this->mMemblockDevice[i].toString() != fp)
		i++;

	// file not found
	if (i >= this->nrOfUsed)
		i = -1;

	return i;
}

std::string FileSystem::toString()
{
	std::string save = "";

	for (int i = 0; i < 250; i++)
	{
		save += this->mMemblockDevice[i].toString();
		save += '\n';
	}

	return save;
}

void FileSystem::insert(const std::string& data, const int i)
{
	this->mMemblockDevice[i].writeBlock(data);
}

void FileSystem::calcNrOfUsed()
{
	int i = 0;
	while (this->mMemblockDevice[i].toString()[0] != 0 && i < 125)
		i++;

	this->nrOfUsed = i;
}


