#ifndef JAVA_CPP
#define JAVA_CPP

#include "Interpreter.cpp"

class Java : public Interpreter {
public:
    Java(Configuration& c) : _config(c), _packageName(""), _packageRoot("") { }

    bool compileFile(const File& file, const std::string& pathIN, const std::string& pathOUT) override {
        const File& validFile = FSManager::getFile(pathIN + "/" + file.name());

        const std::string& options = getExtraOptions(_config.getJavaExtraOptions());
		const std::string& command = "javac " + options + validFile.path() + " -d " + pathOUT;

		int code = system(command.c_str());
        if (code == 0) {
            findPackage(validFile);
            _packageRoot = pathOUT;
            return true;
        }
		return false;
    }

    bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) override {
        std::string classFile = file.nameNoExtension();
        std::string classRoot = file.parentpath();
        if(_packageName != "") {
            classFile = _packageName + "." + file.nameNoExtension();
            classRoot = _packageRoot;
        }

        const std::string& command = "java -classpath " + classRoot + " " + classFile + " < " + pathIN + " > " + pathOUT;
        
		int code = system(command.c_str());
        return code == 0;
    }

    bool isTestable(const File& file) const override { 
        return file.extension() == ".class";
    }

    bool isCompilable(const File& file) const override { 
        return file.extension() == ".java";
    }

    bool isInterpreterAvailable() const override { 
        return true;
    }

private:
	const Configuration& _config;
    std::string _packageName;
    std::string _packageRoot;

    void findPackage(const File& file) {
        auto lines = file.read();
        for(auto line : *lines) {
            auto index = line.find("package ");
            if (index != std::string::npos) {
                _packageName = line.substr(index + 8);
                auto semicolon = _packageName.find(";");
                _packageName = _packageName.substr(0, semicolon);
                return;
            }
        }
        _packageName = "";
    }
};

#endif
