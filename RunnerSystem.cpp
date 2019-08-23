#ifndef RUNNERSYSTEM_CPP
#define RUNNERSYSTEM_CPP

#include "File.cpp"

#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;

enum RSCompailerType {
    GPP,
    JAVA,
    VSCPP,
    UNKNOWN
};

class RunnerSystem {
public:

	bool compileFile(const File& file, const string& pathIN, const string& pathOUT) const {
        RSCompailerType type = getType(file);
		switch(type) {
            case JAVA:
                return compileFileJAVA(file, pathIN, pathOUT);
            case GPP:
                return compileFileGPP(file, pathIN, pathOUT);
            case VSCPP:
            default:
                return false;        
        }
	}

    bool compileFileGPP(const File& file, const string& pathIN, const string& pathOUT) const {
		string command = "g++ -g " + pathIN + "/" + file.Name + " -o " + pathOUT + "/" + file.NameNoExtension() + ".exe";
		int code = system(command.c_str());
		return code == 0;
	}

    bool compileFileJAVA(const File& file, const string& pathIN, const string& pathOUT) const {
		string command = "javac " + pathIN + "/" + file.Name + " -d " + pathOUT;
		int code = system(command.c_str());
		return code == 0;
	}

    //void compileFiles(std::shared_ptr<vector<File>> files, const string& pathIN, const string& pathOUT) const {
    //    for (auto& file : *files) {
	//	    compileFile(file.Name, pathIN, pathOUT);
	//    }
    //}

    bool runTest(const File& file, const string& pathIN, const string& pathOUT) {
        RSCompailerType type = getType(file);
		switch(type) {
            case JAVA:
                return runTestJAVA(file, pathIN, pathOUT);
            case GPP:
				return runTestGPP(file, pathIN, pathOUT);
            case VSCPP:
            default:
				return false;
        }
    }

    bool runTestGPP(const File& file, const string& pathIN, const string& pathOUT) {
        string command = file.Path + " < " + pathIN + " > " + pathOUT;
		int code = system(command.c_str());
        return code == 0;
    }

    bool runTestJAVA(const File& file, const string& pathIN, const string& pathOUT) {
        string command = "java -classpath " + file.ParentPath + " " + file.NameNoExtension() + " < " + pathIN + " > " + pathOUT;
		int code = system(command.c_str());
        return code == 0;
    }

private:
    RSCompailerType getType(const File& file) const {
        if (file.Extension == ".cpp")
            return GPP;
        if (file.Extension == ".exe")
            return GPP;
        if (file.Extension == ".class")
            return JAVA;
        if (file.Extension == ".java")
            return JAVA;
        if (file.Extension == ".sln")
            return VSCPP;
        return UNKNOWN;
    }
};

#endif