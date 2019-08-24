#ifndef CONFIGURATION_CPP
#define CONFIGURATION_CPP

#include "FSManager.cpp"
#include <string>
using std::string;

class Configuration {
public:
	const string& getMVSCommandLineToolsPath() const {
		return s_MVSCommandLineToolsPath;
	}

	const string& getJavaExtraOptions() const {
		return s_JavaExtraOptions;
	}

	const string& getGPPExtraOptions() const {
		return s_GPPExtraOptions;
	}

	const string& getMVSCPPExtraOptions() const {
		return s_MVSCPPExtraOptions;
	}

	const bool getCloseImmediatelyUponEnd() const {
		return s_CloseImmediatelyUponEnd;
	}

	static const Configuration Load(const FSManager& fs, const string& path) {
        const string MVSCommandLineToolsPath = "Compiler:MVSCommandLineToolsPath";
        const string JavaExtraOptions = "Compiler:JavaExtraOptions";
        const string GPPExtraOptions = "Compiler:GPPExtraOptions";
        const string MVSCPPExtraOptions = "Compiler:MVSCPPExtraOptions";
        const string CloseImmediatelyUponEnd = "Other:CloseImmediatelyUponEnd";

		Configuration config = Configuration();
		if (!fs.exists(path)) {
			vector<string> defaultConfig{
				MVSCommandLineToolsPath+"=",
				JavaExtraOptions+"=",
				GPPExtraOptions+"=",
				MVSCPPExtraOptions+"=",
				CloseImmediatelyUponEnd+"=false",
			};
			fs.writeFile(path, defaultConfig);
		}
		auto lines = fs.readFile(path);
		for (string line : *lines) {
			auto pos = line.find("=");
			if (pos != std::string::npos) {
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1, line.size());
				if (key == MVSCommandLineToolsPath)
					config.s_MVSCommandLineToolsPath = value;
				else if (key == JavaExtraOptions)
					config.s_JavaExtraOptions = value;
				else if (key == GPPExtraOptions)
					config.s_GPPExtraOptions = value;
				else if (key == MVSCPPExtraOptions)
					config.s_MVSCPPExtraOptions = value;
				else if (key == CloseImmediatelyUponEnd)
					config.s_CloseImmediatelyUponEnd = (value == "true");
			}
		}
		return config;
	}

private:
	string s_MVSCommandLineToolsPath;
	string s_JavaExtraOptions;
	string s_GPPExtraOptions;
	string s_MVSCPPExtraOptions;
	bool s_CloseImmediatelyUponEnd;

	Configuration() { 
		s_MVSCommandLineToolsPath = "";
		s_JavaExtraOptions = "";
		s_GPPExtraOptions = "";
		s_MVSCPPExtraOptions = "";
		s_CloseImmediatelyUponEnd = false;
	}

};

#endif