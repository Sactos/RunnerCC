#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include "../Configuration.cpp"
#include "../FSManager.cpp"
#include "../File.cpp"
#include <string>

class Interpreter {
public:
    virtual bool compileFile(const File& file, const std::string& pathOUT) = 0;

    bool compile(const std::string& pathIN, const std::string& pathOUT) {
        auto file = findMainFile(pathIN, _mainName, *_extensionCompile);
        if (file == nullptr) {
            //throw new RunnerException("No file with \"" + _mainName + "\" was found.");
            return false;
        }
        return compileFile(*file, pathOUT);
    }

    virtual bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) const = 0;

    bool isTestable(const File& file) const {
        auto it = find(_extensionTest->cbegin(), _extensionTest->cend(), file.extension());
        return it != _extensionTest->cend();
    };

    bool isCompilable(const File& file) const {
        auto it = find(_extensionCompile->cbegin(), _extensionCompile->cend(), file.extension());
        return it != _extensionCompile->cend();
    };

    virtual bool isInterpreterAvailable() const = 0;

protected:
    std::unique_ptr<std::vector<std::string>> _extensionCompile;
    std::unique_ptr<std::vector<std::string>> _extensionTest;
    std::string _mainName;

    const std::string getExtraOptions(const std::string& options) const {
        if (options == "") {
            return "";
        }
        return "" + options + " ";
    }

    virtual std::unique_ptr<File> findMainFile(const std::string& path, const std::string& main, const std::vector<std::string>& extensions) const {
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
};

#endif
