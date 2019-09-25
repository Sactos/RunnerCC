#ifndef JAVA_CPP
#define JAVA_CPP

#include "Interpreter.cpp"

class Java : public Interpreter {
public:
    Java(Configuration& c) : _config(c), _packageName(""), _packageRoot(""), _packageFile("") { 
        this->_codeExtension = std::make_unique<std::vector<std::string>>();
        this->_codeExtension->push_back(".java");
        this->_programExtension = std::make_unique<std::vector<std::string>>();
        this->_programExtension->push_back(".class");
        this->_mainName = "public static void main";
    }

    bool compileFile(const File& file, const std::string& pathOUT) override {
        const std::string& options = getExtraOptions(_config.getJavaExtraOptions());
		const std::string& command = "javac " + options + file.path() + " -d " + pathOUT;
		int code = system(command.c_str());
        if (code == 0) {
            findPackage(file);
            _packageRoot = pathOUT;
            _packageFile = file.nameNoExtension();
            return true;
        }
		return false;
    }

    bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) const override {
        std::string classFile = file.nameNoExtension();
        std::string classRoot = file.parentpath();
        if(_packageName != "") {
            classFile = _packageName + "." + _packageFile;
            classRoot = _packageRoot;
        }
        const std::string& command = "java -classpath " + classRoot + " " + classFile + " < " + pathIN + " > " + pathOUT;
		int code = system(command.c_str());
        return code == 0;
    }

    bool isInterpreterAvailable() const override { 
        return true;
    }

private:
	const Configuration& _config;
    std::string _packageName;
    std::string _packageRoot;
    std::string _packageFile;

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
