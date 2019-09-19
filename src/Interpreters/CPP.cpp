#ifndef CPP_CPP
#define CPP_CPP

#include "Interpreter.cpp"

class CPP : public Interpreter {
public:
    CPP() : _config(Configuration()) {
        this->_extensionCompile = std::make_unique<std::vector<std::string>>();
        this->_extensionCompile->push_back(".cpp");
        this->_extensionTest = std::make_unique<std::vector<std::string>>();
        this->_extensionTest->push_back(".exe");
        this->_mainName = "int main";
    }

    CPP(Configuration& c) : _config(c) { 
        this->_extensionCompile = std::make_unique<std::vector<std::string>>();
        this->_extensionCompile->push_back(".cpp");
        this->_extensionTest = std::make_unique<std::vector<std::string>>();
        this->_extensionTest->push_back(".exe");
        this->_mainName = "int main";
    }

    virtual bool compileFile(const File& file, const std::string& pathOUT) override {
        const std::string& options = getExtraOptions(_config.getGPPExtraOptions());
        const std::string& validOut = FSManager::fixPath(pathOUT + "/" + file.nameNoExtension() + ".exe");
		const std::string& command = "g++ " + options + "-g " + file.path() + " -o " + validOut;
		int code = system(command.c_str());
		return code == 0;
    }

    virtual bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) const override {
        const std::string& command = file.path() + " < " + pathIN + " > " + pathOUT;
		int code = system(command.c_str());
        return code == 0;
    }

    virtual bool isInterpreterAvailable() const override { 
        return true;
    }

private:
	const Configuration& _config;
};

#endif