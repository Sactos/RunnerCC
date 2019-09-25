#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include "../Configuration.cpp"
#include "../FSManager.cpp"
#include "../File.cpp"
#include <string>

class Interpreter {
public:
    virtual bool compileFile(const File& file, const std::string& pathOUT) = 0;

    virtual bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) const = 0;

    virtual ~Interpreter() { }

    bool compile(const std::string& pathIN, const std::string& pathOUT) {
        _mainFile = findFileWithMain(pathIN, _mainName, *_codeExtension);
        if (_mainFile == nullptr) {
            //throw new RunnerException("No file with \"" + _mainName + "\" was found.");
            return false;
        }
        return compileFile(*_mainFile, pathOUT);
    }

    bool runTest(const std::string& pathProgram, const std::string& pathIN, const std::string& pathOUT) const {        
        if (_mainFile == nullptr) {
            //throw new RunnerException("First you need to run the compile method.");
            return false;
        }
        auto _programFile = findFileWithName(pathProgram,  _mainFile->nameNoExtension(), *_programExtension);
        if(_programFile == nullptr) {
            //throw new RunnerException("No file with \"" + _mainFile-nameNoExtension() + "\" was found.");
            return false;
        }
        return runTest(*_programFile, pathIN, pathOUT);
    }

    bool isTestable(const std::string& extension) const {
        auto it = find(_programExtension->cbegin(), _programExtension->cend(), extension);
        return it != _programExtension->cend();
    };

    bool isCompilable(const std::string& extension) const {
        auto it = find(_codeExtension->cbegin(), _codeExtension->cend(), extension);
        return it != _codeExtension->cend();
    };

    virtual bool isInterpreterAvailable() const = 0;

protected:
    std::unique_ptr<std::vector<std::string>> _codeExtension;
    std::unique_ptr<std::vector<std::string>> _programExtension;
    std::string _mainName;
    std::unique_ptr<File> _mainFile;

    const std::string getExtraOptions(const std::string& options) const {
        if (options == "") {
            return "";
        }
        return "" + options + " ";
    }

    virtual std::unique_ptr<File> findFileWithMain(const std::string& path, const std::string& main, const std::vector<std::string>& extensions) const {
        auto files = FSManager::getFilesInFolder(path, extensions);
        for (auto file : *files) {
            auto lines = file.read();
            for (auto line : *lines) {
                auto index = line.find(main);
                if (index != std::string::npos) {
                    return std::make_unique<File>(File(file));
                }
            }
        }
        return nullptr;
    }

    virtual std::unique_ptr<File> findFileWithName(const std::string& path, const std::string& name, const std::vector<std::string>& extensions) const {
        auto files = FSManager::getFilesInFolder(path, extensions, true);
        for (auto file : *files) {
            if (name == file.nameNoExtension()) {
                return std::make_unique<File>(File(file));
            }
        }
        return nullptr;
    }
};

#endif
