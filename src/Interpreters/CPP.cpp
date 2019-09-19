#ifndef CPP_CPP
#define CPP_CPP

#include "Interpreter.cpp"

class CPP : public Interpreter {
public:
    CPP() : _config(Configuration()) {}
    CPP(Configuration& c) : _config(c) { }

    virtual bool compileFile(const File& file, const std::string& pathIN, const std::string& pathOUT) override {
        const std::string& options = getExtraOptions(_config.getGPPExtraOptions());

        const File& validFile = FSManager::getFile(pathIN + "/" + file.name());
        const std::string& validOut = FSManager::fixPath(pathOUT + "/" + file.nameNoExtension() + ".exe");

		string command = "g++ " + options + "-g " + validFile.path() + " -o " + validOut;

		int code = system(command.c_str());
		return code == 0;
    }

    virtual bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) override {
        const std::string& command = file.path() + " < " + pathIN + " > " + pathOUT;

		int code = system(command.c_str());
        return code == 0;
    }

    virtual bool isTestable(const File& file) const override { 
        return file.extension() == ".exe";
    }

    virtual bool isCompilable(const File& file) const override { 
        return file.extension() == ".cpp";
    }

    virtual bool isInterpreterAvailable() const override { 
        return true;
    }

private:
	const Configuration& _config;

};

#endif