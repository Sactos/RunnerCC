#ifndef CONFIGURATION_CPP
#define CONFIGURATION_CPP

#include "FSManager.cpp"
#include <map>
#include <string>

class Configuration {
public:
	const std::string& getMVSCommandLineToolsPath() const {
		return data.at("Compiler:MVSCommandLineToolsPath");
	}

	const std::string& getJavaExtraOptions() const {
		return data.at("Compiler:JavaExtraOptions");
	}

	const std::string& getGPPExtraOptions() const {
		return data.at("Compiler:GPPExtraOptions");
	}

	const std::string& getMVSCPPExtraOptions() const {
		return data.at("Compiler:MVSCPPExtraOptions");
	}

	const bool getCloseImmediatelyUponEnd() const {
		return data.at("Other:CloseImmediatelyUponEnd") == "true";
	}

	const std::string& getFileInExtension() const {
		return data.at("File:InExtension");
	}

	const std::string& getFileOutExtension() const {
		return data.at("File:OutExtension");
	}

	const std::string& getFileExpExtension() const {
		return data.at("File:ExpExtension");
	}

	static const Configuration Load(const std::string& path) {
        std::map<const std::string, std::string> defaultConfig{
            {"Compiler:MVSCommandLineToolsPath","C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\Common7\\Tools\\"},
            {"Compiler:JavaExtraOptions", ""},
            {"Compiler:GPPExtraOptions", ""},
            {"Compiler:MVSCPPExtraOptions", ""},
            {"Other:CloseImmediatelyUponEnd", "false"},
    		{"File:InExtension", ".in.txt"},
		    {"File:OutExtension", ".out.txt"},
		    {"File:ExpExtension", ".xp.txt"},
        };
		Configuration config = Configuration(defaultConfig);
		if (!FSManager::exists(path)) {
			vector<std::string> dc;
            std::transform(defaultConfig.cbegin(), defaultConfig.cend(), std::back_inserter(dc), 
                [](auto &pair) {
                    return pair.first + "=" + pair.second; 
                });
			FSManager::writeFile(path, dc);
		}
		auto lines = FSManager::readFile(path);
		for (auto line : *lines) {
			auto pos = line.find("=");
			if (pos != std::string::npos) {
				auto key = line.substr(0, pos);
				auto value = line.substr(pos + 1, line.size());
                if (config.data.find(key) != config.data.end()) {
                    config.data[key] = value;
                }
			}
		}
		return config;
	}

private:
    std::map<const std::string, std::string> data;

    Configuration() { }

	Configuration(const std::map<const std::string, std::string> defaultValues) {
        data = defaultValues;
    }
};

#endif