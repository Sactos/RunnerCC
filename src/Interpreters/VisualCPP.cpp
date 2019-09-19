#ifndef VISUALCPP_CPP
#define VISUALCPP_CPP

#include "CPP.cpp"

class VisualCPP : public CPP {
public:
    VisualCPP() : _config(Configuration()) {}
    VisualCPP(Configuration& c) : _config(c) { }

    bool compileFile(const File& file, const std::string& pathIN, const std::string& pathOUT) override {
        const std::string& options = getExtraOptions(_config.getMVSCPPExtraOptions());
		const std::string& routeToVsDev = _config.getMVSCommandLineToolsPath();
		const std::string preCommand = "\"" + FSManager::fixPath(routeToVsDev + "/" + VSDevCmd) + "\"";

        const std::string& validIn = FSManager::fixPath(pathIN + "/" + file.name());
        const std::string& validOut = FSManager::fixPath(pathOUT + "/" + file.nameNoExtension() + ".exe");

		const std::string& command = "cl " + options + validIn + " /o " + validOut;
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
	const Configuration& _config;
    const std::string& VSDevCmd = "VsDevCmd.bat";

};

#endif