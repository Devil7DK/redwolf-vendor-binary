#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/stat.h>

using namespace std;

string GetEnv ( const string );
string SplitString ( string, string, int );
bool DirectoryExists( const char* );
bool FileExists( const char* );
string ToUpper( string );
string GetIncremental( string );
void UpdateXML( string, string, string, string );
void SetAttribute( xmlNode*, xmlDoc*, string, string );

int main()
{
    string CMD;
    string OUT,TARGET_ARCH,TW_DEVICE_VERSION,TARGET_PRODUCT,BUILD_NUMBER;
    string RW_BUILD,RW_WORK,RW_DEVICE,RW_OUT_NAME,RW_XML_PATH;

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
    OUT = GetEnv("OUT");
    TARGET_PRODUCT = GetEnv("TARGET_PRODUCT");

    if(TW_DEVICE_VERSION == "") {
        RW_BUILD = "Unofficial";
    } else {
        RW_BUILD = TW_DEVICE_VERSION;
    }
    RW_WORK = OUT + "/RW_AIK";
    RW_DEVICE = SplitString(TARGET_PRODUCT,"_",1);
    RW_OUT_NAME = "RedWolf-" + RW_BUILD + "-" + RW_DEVICE;
    RW_XML_PATH = "redwolf.xml";


    if(DirectoryExists(RW_WORK.c_str())) {
        cout << BLUE << "-- Working folder found in OUT. Cleaning up" << NC << endl;
        CMD = "rm -rf " + RW_WORK;
        system(CMD.c_str());
    }

    cout << BLUE << "-- Unpacking recovery image" << NC << endl;
    CMD = "bash " + RW_VENDOR + "/tools/mkboot " + OUT + "/recovery.img " + RW_WORK + " > /dev/null 2>&1";
    system(CMD.c_str());


    cout << BLUE << "-- Copying mkbootimg, unpackbootimg binaries to sbin" << NC << endl;
    if(TARGET_ARCH == "arm" || TARGET_ARCH == "arm64" || TARGET_ARCH == "x86" || TARGET_ARCH == "x86_64") {
        cout << GREEN << "-- " + ToUpper(TARGET_ARCH) + " arch detected. Copying " + ToUpper(TARGET_ARCH) + " binaries" << NC << endl;
        CMD = "cp " + RW_VENDOR + "/prebuilt/" + TARGET_ARCH + "/mkbootimg " + RW_WORK + "/ramdisk/sbin";
        CMD = CMD + " && " + "cp " + RW_VENDOR + "/prebuilt/" + TARGET_ARCH + "/unpackbootimg " + RW_WORK + "/ramdisk/sbin";
        CMD = CMD + " && " + "chmod a+x " + RW_WORK + "/ramdisk/sbin/*bootimg";
        system(CMD.c_str());
    } else {
        cout << RED << "-- Couldn't detect current device architecture or it is not supported" << NC << endl;
    }


    cout << BLUE << "-- Repacking and copying recovery" << NC << endl;
    CMD = "bash " + RW_VENDOR +"/tools/mkboot " + RW_WORK + " " + OUT + "/" + RW_OUT_NAME + ".img > /dev/null 2>&1";
    system(CMD.c_str());
    CMD = "cd " + OUT + " && md5sum " + RW_OUT_NAME + ".img > " + RW_OUT_NAME + ".img.md5 && cd - > /dev/null 2>&1";
    system(CMD.c_str());


    if(TW_DEVICE_VERSION != "") {
        if(!FileExists(RW_XML_PATH.c_str())) {
            cout << BLUE << "-- RedWolf XML not found. Downloading from remote" << NC << endl;
            CMD = "curl -o " + RW_WORK + "/redwolf.xml https://redwolfrecovery.github.io/redwolf.xml > /dev/null 2>&1";
            system(CMD.c_str());
        }else{
            cout << BLUE << "-- RedWolf XML found." << NC << endl;
            CMD = "cp " + RW_XML_PATH + " " + RW_WORK + "/redwolf.xml";
            system(CMD.c_str());
        }
        cout << BLUE << "-- Reading build number from recovery" << NC << endl;
        BUILD_NUMBER = GetIncremental(RW_WORK + "/ramdisk/default.prop");
        cout << BLUE << "-- Updating build number in XML" << NC << endl;
        UpdateXML( RW_WORK + "/redwolf.xml", RW_XML_PATH, RW_DEVICE, BUILD_NUMBER);
    }

    cout << RED << "--------------------Finished making RedWolf---------------------" << NC << endl;
    cout << GREEN << "RedWolf image: ${OUT}/" << RW_OUT_NAME << ".img" << NC << endl;
    cout << GREEN << "          MD5: ${OUT}/" << RW_OUT_NAME << ".img.md5" << NC << endl;
    if(TW_DEVICE_VERSION != ""){
    cout << GREEN << "          XML: redwolf.xml" << NC << endl;}
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

string GetIncremental( string cfilename ) {
    ifstream fileInput;
    int offset;
    string line;
    char* search = (char*)"ro.bootimage.build.date.utc";
    fileInput.open(cfilename.c_str());
    if(fileInput.is_open()) {
        while(!fileInput.eof()) {
            getline(fileInput, line);
            if ((offset = line.find(search, 0)) != string::npos) {
                return SplitString(line,"=",1);
            }
        }
        fileInput.close();
    } else cout << "Unable to open file.";
}

void UpdateXML( string inputFile, string outputFile, string deviceName, string buildNumber) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(inputFile.c_str(), NULL, 0);
    if (doc == NULL) {
        cout << "error: could not parse file " << endl;
    }

    root_element = xmlDocGetRootElement(doc);

    SetAttribute(root_element, doc, deviceName, buildNumber);

    xmlSaveFormatFile(outputFile.c_str(), doc, 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();

}


void SetAttribute( xmlNode *node, xmlDoc *doc, string deviceName, string buildNumber) {
    char *devicename;
    xmlNodePtr cur = node->xmlChildrenNode;
    int cnt = 0;

    while(cur != NULL) {
        if (cur->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur->name, (const xmlChar *)"device")) {
                devicename = (char *)xmlGetProp(cur,(const xmlChar *)"name");
                if(strcmp(devicename,deviceName.c_str())==0) {
                    xmlSetProp(cur,(const xmlChar *)"utc_build_date",(const xmlChar *)buildNumber.c_str());
                }
            } else if(!xmlStrcmp(cur->name, (const xmlChar *)"devices")) {
                SetAttribute(cur,doc,deviceName,buildNumber);
            }
        }
        cur = cur->next;
    }
}
