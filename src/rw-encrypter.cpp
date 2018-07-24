#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

#include "encryption.hpp"


void ProcessDirectory(std::string directory);
void ProcessFile(std::string file, std::string parent);
void ProcessEntity(struct dirent* entity, std::string parent);

std::string path;

int main(int argc, char* argv[])
{
	struct stat s;
	
	if ( argc != 2 ) {
		std::cout<<"Invalid Arguments..!"<<std::endl<<std::endl<<"USAGE : "<<argv[0]<<" <ABSOLUTE PATH>"<<std::endl;
		return 1;
	}
	
    path = argv[1];

	if ( stat(path.c_str(),&s) == 0 ) {
		if ( s.st_mode & S_IFDIR ) {
			std::cout<<"Processing directory: "<<path<<std::endl<<std::endl;
			ProcessDirectory("");
		} else if ( s.st_mode & S_IFREG ) {
			std::cout<<"Processing file: "<<path<<std::endl<<std::endl;
			
			const char *relativepath = path.c_str();
			char *actualpath;
			
			actualpath = realpath(relativepath, NULL);
			
			if ( actualpath != NULL ) {
				path = actualpath;
				std::string parent = path.substr(0, path.find_last_of('/') + 1);
				std::string file = path.substr(path.find_last_of('/') + 1, path.length());
				ProcessFile(file, parent);
			} else {
				std::cout<<"Error, unable to find absolute path..."<<std::endl;
				return 1;
			}
		} else {
			std::cout<<"Error, type of path is unknown..."<<std::endl;
			return 1;
		}
	} else {
		std::cout<<"Error on determining the type of path..."<<std::endl;
		return 1;
	}
	
    return 0;
}

void ProcessDirectory(std::string directory)
{
    std::string dirToOpen = path + directory;
	
    auto dir = opendir(dirToOpen.c_str());
	
    path = dirToOpen + "/";
	
    if(NULL == dir)
    {
		std::cout<<"Error opening directory: "<<dirToOpen<<std::endl;
        return;
    }
	
    auto entity = readdir(dir);
	
    while(entity != NULL)
    {
        ProcessEntity(entity,dirToOpen);
        entity = readdir(dir);
    }
	
    path.resize(path.length() - 1 - directory.length());
    closedir(dir);
}

void ProcessEntity(struct dirent* entity, std::string parent)
{
    if(entity->d_type == DT_DIR)
    {
		if(entity->d_name[0] == '.')
        {
            return;
        }

        ProcessDirectory(std::string(entity->d_name));
		
        return;
    }

    if(entity->d_type == DT_REG)
    {
		ProcessFile(std::string(entity->d_name), parent);
        return;
    }
}

void ProcessFile(std::string file, std::string parent)
{
	std::string fullname = parent + "/" + file;
    std::cout << "Encrypting/Decrypting File : \"" << fullname.c_str() << "\".";
	
	std::string encryptedname = parent + "/" + EncryptDecryptString(file);
	
	if (EncryptDecryptFile(fullname,encryptedname)) {
		std::cout<<"Done."<<std::endl;
		std::remove(fullname.c_str());
	}
	else
		std::cout<<"Failed!"<<std::endl;
			
}