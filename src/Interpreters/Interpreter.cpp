#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include "../Configuration.cpp"
#include "../FSManager.cpp"
#include "../File.cpp"
#include <string>

class Interpreter {
public:
    virtual bool compileFile(const File& file, const std::string& pathIN, const std::string& pathOUT) = 0;

    virtual bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) = 0;

    virtual bool isTestable(const File& file) const = 0;

    virtual bool isCompilable(const File& file) const = 0;

    virtual bool isInterpreterAvailable() const = 0;

protected:
    virtual std::string getMainName() const = 0;

    const std::string getExtraOptions(const std::string& options) const {
        if (options == "") {
            return "";
        }
        return "" + options + " ";
    }

    auto findMainFile(const std::string& path, const std::string& extension) {
        auto extensions = std::vector<std::string> { extension };
        auto files = FSManager::getFilesInFolder(path, extensions, true);
        for (auto file : *files) {
            auto lines = file.read();
            for (auto line : *lines) {
                auto index = line.find(getMainName());
                if (index != std::string::npos) {
                    return std::make_unique<File>(File(file));
                }
            }
        }
        return nullptr;
    }
};

#endif
