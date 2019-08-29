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

	const string& getFileInExtension() const {
		return s_FileInExtension;
	}

	const string& getFileOutExtension() const {
		return s_FileOutExtension;
	}

	const string& getFileExpExtension() const {
		return s_FileExpExtension;
	}

	static const Configuration Load(const string& path) {
        const string MVSCommandLineToolsPath = "Compiler:MVSCommandLineToolsPath";
        const string JavaExtraOptions = "Compiler:JavaExtraOptions";
        const string GPPExtraOptions = "Compiler:GPPExtraOptions";
        const string MVSCPPExtraOptions = "Compiler:MVSCPPExtraOptions";
        const string CloseImmediatelyUponEnd = "Other:CloseImmediatelyUponEnd";
		const string FileInExtension = "File:InExtension";
		const string FileOutExtension = "File:OutExtension";
		const string FileExpExtension = "File:ExpExtension";

		Configuration config = Configuration();
		if (!FSManager::exists(path)) {
			vector<string> defaultConfig{
				MVSCommandLineToolsPath + "=",
				JavaExtraOptions + "=",
				GPPExtraOptions + "=",
				MVSCPPExtraOptions + "=",
				CloseImmediatelyUponEnd + "=false",
				FileInExtension + "=" + config.s_FileInExtension,
				FileOutExtension + "=" + config.s_FileOutExtension,
				FileExpExtension + "=" + config.s_FileExpExtension,
			};
			FSManager::writeFile(path, defaultConfig);
		}
		auto lines = FSManager::readFile(path);
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
                else if (key == FileInExtension)
					config.s_FileInExtension = value;
                else if (key == FileOutExtension)
					config.s_FileOutExtension = value;
                else if (key == FileExpExtension)
					config.s_FileExpExtension = value;
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
	string s_FileInExtension;
	string s_FileOutExtension;
	string s_FileExpExtension;

	Configuration() { 
		s_MVSCommandLineToolsPath = "";
		s_JavaExtraOptions = "";
		s_GPPExtraOptions = "";
		s_MVSCPPExtraOptions = "";
		s_CloseImmediatelyUponEnd = false;
		s_FileInExtension = ".in.txt";
		s_FileOutExtension = ".out.txt";
		s_FileExpExtension = ".xp.txt";
	}

};

#endif