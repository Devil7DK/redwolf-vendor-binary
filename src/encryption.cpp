#include <iostream>
#include <fstream>

#include "encryption.hpp"

std::string EncryptDecryptString(std::string text) {
	std::string result = "";
	std::string special = ".-_+0123456789 ";
	
    for (int i=0;i<text.length();i++) {
		if (special.find(text[i]) != std::string::npos)
			result += text[i];
		else if (isupper(text[i]))
			result += char(int(text[i]+13-65)%26 +65);
		else
			result += char(int(text[i]+13-97)%26 +97);
	}
	
	return result;
}

bool EncryptDecryptFile(std::string filename, std::string output) {
	unsigned int blockSize = 0x10000;
	int bufferSize = blockSize;
	int difference = 0;
	int filePointer = 0;
	
	std::fstream fin,fout;
	fin.open (filename.c_str(), std::ios::in | std::ios::binary);
	fout.open (output.c_str(), std::ios::out | std::ios::binary);
	
	if (!fin.is_open() || !fout.is_open()) {
		return false;
	}
	
	fin.seekg (0, std::ios::end);
	int fileSize = fin.tellg();
	fin.seekg (0, std::ios::beg);
	
	while (filePointer < fileSize) {
		char buffer [blockSize];
		
		difference = fileSize - filePointer;
		if (difference < bufferSize) {
			bufferSize = difference;
		}
		
		fin.read (buffer, bufferSize);
		
		for (int i = 0; i < bufferSize; i++){
			buffer[i] = ~buffer[i];
		}
		
		fout.write(buffer, bufferSize);
		
		filePointer += bufferSize;
	 }
	return true;
}