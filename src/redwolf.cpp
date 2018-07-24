#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <sys/stat.h>

#include "encryption.hpp"

using namespace std;

string GetEnv ( const string );
string SplitString ( string, string, int );
bool DirectoryExists( const char* );
bool FileExists( const char* );
string ToUpper( string );
int getdir ( string, vector<string> &);

int main()
{
    string CMD;
    string OUT,TARGET_ARCH,TW_DEVICE_VERSION,RW_DEVICE_VERSION,TARGET_PRODUCT;
    string RW_BUILD,RW_WORK,RW_DEVICE,RW_OUT_NAME;

    string RW_VENDOR = "vendor/redwolf";

    string RED = "\033[0;31m";
    string GREEN = "\033[0;32m";
    string BLUE = "\033[0;34m";
    string NC = "\033[0m";

    cout << RED << "================================================================" << NC << endl;
    cout << RED << "          @.                                         &@       " << NC << endl;
    cout << RED << "         @@@@@(                                   &@@@@@      " << NC << endl;
    cout << RED << "        @@@ (@@@@                               @@@@@ @@@     " << NC << endl;
    cout << RED << "       @@@     @@@@#                          @@@@@@   @@%    " << NC << endl;
    cout << RED << "      ,@@. @     @@@@#                      @@@@@@@* @  @@    " << NC << endl;
    cout << RED << "      @@@ .@@      @@@@/(#%&&@@@@@@@@@@@%.@@@@@@@@@ @@  @@&   " << NC << endl;
    cout << RED << "      @@   ,@@      &@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@  @@   " << NC << endl;
    cout << RED << "     #@@    @@@                        @@@@@@@@@@*@@@@@  @@   " << NC << endl;
    cout << RED << "     @@@    ,@@@                        %@@@@@@@@@@@@@@( @@*  " << NC << endl;
    cout << RED << "     @@@   .@@@                          @@@@@@@@@@@@@@@ @@/  " << NC << endl;
    cout << RED << "     @@@, @@@@                              @@@@@@@@@@@@%@@*  " << NC << endl;
    cout << RED << "     %@@@@@@*         @@                 &@@@@@@@@@@@@@@@@@   " << NC << endl;
    cout << RED << "     .@@@@@        @@@ #@@             @@, @@@@@@@@@@@@@@@@   " << NC << endl;
    cout << RED << "      @@@@          ,@@/.@@@@@     @@@@@.#@@@@@@@@@@@@@@@@@   " << NC << endl;
    cout << RED << "      /@@.            @@@@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@   " << NC << endl;
    cout << RED << "     @@@%                              .%@@@@@@@@@@@@@@@@@@&  " << NC << endl;
    cout << RED << "    .@@@        %            @@@@@@@   @@@@@@@@@@@@@@@@@@@@@  " << NC << endl;
    cout << RED << "    @@@#        &           @#@@@@@/@  (@@@@@@@@@@@@@@@@@@@@@ " << NC << endl;
    cout << RED << "   *@@@@@%       @   %   .  @@@@@@@@@%@@@@@@@@@@@@@@@@.@@@@@@." << NC << endl;
    cout << RED << "   @@@@@@        @@ @   @@@@& %@@@@@@@@@@@@@@@@@@@@@@@ ,@@@@@@" << NC << endl;
    cout << RED << "   @@@@@@        %@@@  %@@@,@@@@@@@@@.@@@@@@@@@@@@@@@@  @@@@ %" << NC << endl;
    cout << RED << "    ,@@@@         @@@  @@@@          *@@@@@@@@@@@@@@@/  @@@@  " << NC << endl;
    cout << RED << "    %@@@@@         @@@/  @@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@  " << NC << endl;
    cout << RED << "    @@@@@@@@        @@@@  @@@ #@@@( @@@@@@@@@@@@@@@@   @@@@@( " << NC << endl;
    cout << RED << "     *@@@@@@@@@       @@@%  @@@@@@@@@@@@@@@@@@@@@@@   @@@@@@  " << NC << endl;
    cout << RED << "        @@@@@@@@@      .@@@@     ,,,,@@@@@@@@@@@@@. .@@@@@@   " << NC << endl;
    cout << RED << "          @@@@@@@@@      /@@@@@  ,&@@@@@@@@@@@@@@, @@@@@@@    " << NC << endl;
    cout << RED << "            &@@@@@@@@       @@@@@@@@@  @@@@@@@@@  @@@@@@#     " << NC << endl;
    cout << RED << "              @@@@@@@@&               @@@@@@@@@ &@@@@@@       " << NC << endl;
    cout << RED << "                @@@@@@@@             (@@@@@@@@ @@@@@@@        " << NC << endl;
    cout << RED << "                 /@@@@@@@@          ,@@@@@@@@@@@@@@@.         " << NC << endl;
    cout << RED << "                   @@@@@@@@%       (@@@@@@@@@@@@@@&           " << NC << endl;
    cout << RED << "                    &@@@@@@@@     %@@@@@@@@@@@@@@             " << NC << endl;
    cout << RED << "                      @@@@@@@@/  @@@@@@@@@@@@@@               " << NC << endl;
    cout << RED << "                       @@@@@@@@@@@@@@@@@@@@@@                 " << NC << endl;
    cout << RED << "                        @@@@@@@@@@@@@@@@@@&                   " << NC << endl;
    cout << RED << "                         &@@@@@@@@@@@@@@.                     " << NC << endl;
    cout << RED << "                          &@@@@@@@@@@/                        " << NC << endl;
    cout << RED << "                           &@@@@@@*                           " << NC << endl;
    cout << RED << "-------------------------Making RedWolf-------------------------" << NC << endl;


    cout << BLUE << "-- Setting up environment variables" << NC << endl;

    TARGET_ARCH = GetEnv("TARGET_ARCH");
    TW_DEVICE_VERSION = GetEnv("TW_DEVICE_VERSION");
    RW_DEVICE_VERSION = GetEnv("RW_DEVICE_VERSION");
    OUT = GetEnv("OUT");
    TARGET_PRODUCT = GetEnv("TARGET_PRODUCT");

    if(RW_DEVICE_VERSION != ""){
	RW_BUILD = RW_DEVICE_VERSION;
    }else{
        if(TW_DEVICE_VERSION == "") {
            RW_BUILD = "Unofficial";
        } else {
            RW_BUILD = TW_DEVICE_VERSION;
        }
    }
    RW_WORK = OUT + "/RW_AIK";
    RW_DEVICE = SplitString(TARGET_PRODUCT,"_",1);
    RW_OUT_NAME = "RedWolf-" + RW_BUILD + "-" + RW_DEVICE;

    if(DirectoryExists(RW_WORK.c_str())) {
        cout << BLUE << "-- Working folder found in OUT. Cleaning up" << NC << endl;
        CMD = "rm -rf " + RW_WORK;
        system(CMD.c_str());
    }

    cout << BLUE << "-- Unpacking recovery image" << NC << endl;
    CMD = "bash " + RW_VENDOR + "/tools/mkboot " + OUT + "/recovery.img " + RW_WORK + " > /dev/null 2>&1";
    system(CMD.c_str());


    cout << BLUE << "-- Copying binaries to sbin" << NC << endl;
    if(TARGET_ARCH == "arm" || TARGET_ARCH == "arm64" || TARGET_ARCH == "x86" || TARGET_ARCH == "x86_64") {
        cout << GREEN << "-- " + ToUpper(TARGET_ARCH) + " arch detected. Copying " + ToUpper(TARGET_ARCH) + " binaries" << NC << endl;
        
		string prebuilts = RW_VENDOR + "/prebuilt/" + TARGET_ARCH + "/";
		vector<string> files = vector<string>();
		
		getdir(prebuilts,files);
		
		for (unsigned int i = 0;i < files.size();i++) {
			string outfile = RW_WORK + "/ramdisk/sbin/" + EncryptDecryptString(files[i]);
			string infile = prebuilts + files[i];
			EncryptDecryptFile(infile, outfile);
			system(("chmod a+x " + outfile).c_str());
		}
    } else {
        cout << RED << "-- Couldn't detect current device architecture or it is not supported" << NC << endl;
    }


    cout << BLUE << "-- Repacking and copying recovery" << NC << endl;
    CMD = "bash " + RW_VENDOR +"/tools/mkboot " + RW_WORK + " " + OUT + "/" + RW_OUT_NAME + ".img > /dev/null 2>&1";
    system(CMD.c_str());
    CMD = "cd " + OUT + " && md5sum " + RW_OUT_NAME + ".img > " + RW_OUT_NAME + ".img.md5 && cd - > /dev/null 2>&1";
    system(CMD.c_str());

    cout << RED << "--------------------Finished making RedWolf---------------------" << NC << endl;
    cout << GREEN << "RedWolf image: ${OUT}/" << RW_OUT_NAME << ".img" << NC << endl;
    cout << GREEN << "          MD5: ${OUT}/" << RW_OUT_NAME << ".img.md5" << NC << endl;
    cout << RED << "================================================================" << NC << endl;

    return 0;
}

string GetEnv( const string var ) {
    const char * val = ::getenv( var.c_str() );
    if ( val == 0 ) {
        return "";
    }
    else {
        return val;
    }
}

string SplitString( string var, string delimiter, int index) {
    int i = 0;
    size_t pos = 0;

    string token;
    while ((pos = var.find(delimiter)) != std::string::npos) {
        token = var.substr(0, pos);
        var.erase(0, pos + delimiter.length());
        if(i == index) {
            return token;
        } else {
            i++;
        }
    }
    return var;
}

bool DirectoryExists( const char* pzPath ) {
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;
        (void) closedir (pDir);
    }

    return bExists;
}

bool FileExists( const char* filePath) {
  struct stat buffer;
  return (stat (filePath, &buffer) == 0); 
}

string ToUpper( string str ) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
		string name = string(dirp->d_name);
        if (name != "." && name != "..") {
			files.push_back(name);
		}
    }
    closedir(dp);
    return 0;
}