#ifndef VISUALCPP_CPP
#define VISUALCPP_CPP

#include "CPP.cpp"

class VisualCPP : public CPP {
public:
    VisualCPP() : _config(Configuration()) {
        this->_codeExtension = std::make_unique<std::vector<std::string>>();
        this->_codeExtension->push_back(".cpp");
        //this->_codeExtension->push_back(".sln");
        this->_programExtension = std::make_unique<std::vector<std::string>>();
        this->_programExtension->push_back(".exe");
        this->_mainName = "int main";
    }

    VisualCPP(const Configuration& c) : _config(c) { 
        this->_codeExtension = std::make_unique<std::vector<std::string>>();
        this->_codeExtension->push_back(".cpp");
        //this->_codeExtension->push_back(".sln");
        this->_programExtension = std::make_unique<std::vector<std::string>>();
        this->_programExtension->push_back(".exe");
        this->_mainName = "int main";
    }

    bool compileFile(const File& file, const std::string& pathOUT) override {
        const std::string& options = getExtraOptions(_config.getMVSCPPExtraOptions());
		const std::string& routeToVsDev = _config.getMVSCommandLineToolsPath();
		const std::string& preCommand = "\"" + FSManager::fixPath(routeToVsDev + "/" + VSDevCmd) + "\"";
        const std::string& validOut = FSManager::fixPath(pathOUT + "/" + file.nameNoExtension() + ".exe");
		const std::string& command = "cl " + options + file.path() + " /o " + validOut;
		const std::string& task = preCommand + " && " + command;
		int code = system(task.c_str());
		return code == 0;
    }

    bool isInterpreterAvailable() const override { 
        #if _WINDOWS_
			return FSManager::exists(FSManager::fixPath(_config.getMVSCommandLineToolsPath() + "/" + VSDevCmd));
#else
			return false;
#endif
    }

private:
	const Configuration _config;
    const std::string VSDevCmd = "VsDevCmd.bat";

};

#endif