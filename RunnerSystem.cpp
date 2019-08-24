#ifndef RUNNERSYSTEM_CPP
#define RUNNERSYSTEM_CPP

#include "Configuration.cpp"
#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;

enum RSCompailerType {
	//C++
	ANY_CPP,
	GPP,
	MVSCPP,
	//JAVA
	JAVA,
	//UNKNOWN
	UNKNOWN,
};

class RunnerSystem {
public:
	RunnerSystem(Configuration& c) : config(c), fs() { }

	bool compileFile(const File& file, const string& pathIN, const string& pathOUT) const {
        RSCompailerType type = getCompailerType(file);
		switch(type) {
            case JAVA:
                return compileFileJAVA(file, pathIN, pathOUT);
            case GPP:
                return compileFileGPP(file, pathIN, pathOUT);
            case MVSCPP:
#if _WINDOWS_        
				return compileFileMVSCPP(file, pathIN, pathOUT);
#endif
            default:
                return false;        
        }
	}

#if _WINDOWS_
	bool compileFileMVSCPP(const File& file, const string& pathIN, const string& pathOUT) const {
        const string options = getExtraOptions(config.getMVSCPPExtraOptions());
		const string routeToVsDev = config.getMVSCommandLineToolsPath();
		string preCommand = "\"" + fs.getFile(routeToVsDev + "/" + VSDevCmd).Path + "\"";
		string command = "cl " + options + pathIN + "/" + file.Name + " /o " + pathOUT + "/" + file.NameNoExtension() + ".exe";
		string task = preCommand + " && " + command;
		int code = system(task.c_str());
		return code == 0;
	}
#endif

    bool compileFileGPP(const File& file, const string& pathIN, const string& pathOUT) const {
        const string options = getExtraOptions(config.getGPPExtraOptions());
		string command = "g++ " + options + "-g " + pathIN + "/" + file.Name + " -o " + pathOUT + "/" + file.NameNoExtension() + ".exe";
		int code = system(command.c_str());
		return code == 0;
	}

    bool compileFileJAVA(const File& file, const string& pathIN, const string& pathOUT) const {
        const string options = getExtraOptions(config.getJavaExtraOptions());
		string command = "javac " + options + pathIN + "/" + file.Name + " -d " + pathOUT;
		int code = system(command.c_str());
		return code == 0;
	}

    bool runTest(const File& file, const string& pathIN, const string& pathOUT) {
		RSCompailerType type = getTestType(file);
		switch(type) {
            case JAVA:
                return runTestJAVA(file, pathIN, pathOUT);
            case ANY_CPP:
				return runTestCPP(file, pathIN, pathOUT);
            default:
				return false;
        }
    }

    bool runTestCPP(const File& file, const string& pathIN, const string& pathOUT) {
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
	const string VSDevCmd = "VsDevCmd.bat";

	const Configuration& config;
	const FSManager fs;

	RSCompailerType getTestType(const File& file) const {
		if (file.Extension == ".exe")
			return ANY_CPP;
		if (file.Extension == ".class")
			return JAVA;
		return UNKNOWN;
	}

    RSCompailerType getCompailerType(const File& file) const {
        if (file.Extension == ".cpp") {
#if _WINDOWS_
			if (fs.exists(fs.getFile(config.getMVSCommandLineToolsPath() + "/" + VSDevCmd).Path)) {
				return MVSCPP;
			}
            return GPP;
#else
			return GPP;
#endif
		}
        if (file.Extension == ".java")
            return JAVA;
#if _WINDOWS_
        if (file.Extension == ".sln")
            return MVSCPP;
#endif
        return UNKNOWN;
    }

    const string getExtraOptions(const string& options) const {
        if (options == "") {
            return "";
        }
        return "" + options + " ";
    }

};

#endif