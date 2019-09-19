#ifndef CONFIGURATION_CPP
#define CONFIGURATION_CPP

#include "FSManager.cpp"
#include <map>
#include <string>

class Configuration {
public:
    Configuration() { }

	const std::string& getMVSCommandLineToolsPath() const {
		return _data.at("Compiler:MVSCommandLineToolsPath");
	}

	const std::string& getJavaExtraOptions() const {
		return _data.at("Compiler:JavaExtraOptions");
	}

	const std::string& getGPPExtraOptions() const {
		return _data.at("Compiler:GPPExtraOptions");
	}

	const std::string& getMVSCPPExtraOptions() const {
		return _data.at("Compiler:MVSCPPExtraOptions");
	}

	bool getCloseImmediatelyUponEnd() const {
		return _data.at("Other:CloseImmediatelyUponEnd") == "true";
	}

	const std::string& getFileInExtension() const {
		return _data.at("File:InExtension");
	}

	const std::string& getFileOutExtension() const {
		return _data.at("File:OutExtension");
	}

	const std::string& getFileExpExtension() const {
		return _data.at("File:ExpExtension");
	}

    bool getTestEndOnError() const {
		return _data.at("Test:EndOnError") == "true";
	}

	static const Configuration load(const std::string& path) {
        std::map<const std::string, std::string> defaultConfig{
            {"Compiler:MVSCommandLineToolsPath","C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\Common7\\Tools\\"},
            {"Compiler:JavaExtraOptions", ""},
            {"Compiler:GPPExtraOptions", ""},
            {"Compiler:MVSCPPExtraOptions", ""},
            {"Other:CloseImmediatelyUponEnd", "false"},
    		{"File:InExtension", ".in.txt"},
		    {"File:OutExtension", ".out.txt"},
		    {"File:ExpExtension", ".out.txt"},
            {"Test:EndOnError", "false"}
        };
        auto file = FSManager::getFile(path);
		Configuration config = Configuration(defaultConfig);
		if (!file.exists()) {
			std::vector<std::string> dc;
            std::transform(defaultConfig.cbegin(), defaultConfig.cend(), std::back_inserter(dc), 
                [](auto &pair) {
                    return pair.first + "=" + pair.second; 
                });
			file.write(dc);
		}
		auto lines = file.read();
		for (auto line : *lines) {
			auto pos = line.find("=");
			if (pos != std::string::npos) {
				auto key = line.substr(0, pos);
				auto value = line.substr(pos + 1, line.size());
                if (config._data.find(key) != config._data.end()) {
                    config._data[key] = value;
                }
			}
		}
		return config;
	}

private:
    std::map<const std::string, std::string> _data;

	Configuration(const std::map<const std::string, std::string> defaultValues) {
        _data = defaultValues;
    }
};

#endif