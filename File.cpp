#ifndef FILE_CPP
#define FILE_CPP

#include <string>
#include <experimental/filesystem>
#include <vector>
#include <memory>
#include <fstream>

class File {
public:
	const std::string& name() const {
        return _name;
    }

	const std::string& path() const {
        return _path;
    }

    const std::string& extension() const {
        return _extension;
    }

	const std::string& parentpath() const {
        return _parentpath;
    }

    File(const std::experimental::filesystem::path& path) : 
        _name(path.filename().string()), 
        _path(path.string()),
        _extension(path.extension().string()),
		_parentpath(path.parent_path().string()) { };

    const std::string nameWithOut(const std::string& extencion) const {
        auto pos = _name.find(extencion);
        if (pos == std::string::npos) {
            return _name;
        }
        return _name.substr(0, pos);
    }

    const std::string nameNoExtension() const {
        return nameWithOut(_extension);
    }

    const std::string pathNoExtension() const {
        auto pos = _path.find(_extension);
        if (pos == std::string::npos) {
            return _name;
        }
        return _path.substr(0, pos);
    }

    void writeLine(const std::string& line) const {
		std::ofstream o(_path);
        o << line << std::endl;
	}

    void write(const std::vector<std::string>& lines) const {
        std::ofstream o(_path);
		for (auto line : lines) {
			o << line << std::endl;
		}
    }

    void clear() {
        std::ofstream o(_path, std::ios_base::trunc);
        o << "";
    }

    auto read() const {
		auto lines = std::make_unique<std::vector<std::string>>();
		std::ifstream file(_path);
		std::string line;
		while (!file.eof()) {
			std::getline(file, line);
			ifEndInRRemoveIt(line);
			lines->push_back(line);
		}
		return lines;
	}

    bool exists() const {
        return std::experimental::filesystem::exists(_path);
    }

    static bool sameContent(const std::string& path1, const std::string& path2, std::vector<unsigned int> &errors) {
        std::ifstream file1(path1);
        std::ifstream file2(path2);
        std::string line1, line2;
        unsigned int errorLine = 0;
        while (!file1.eof() && !file2.eof()) {
            errorLine++;
            std::getline(file1, line1);
            std::getline(file2, line2);
            ifEndInRRemoveIt(line1);
            ifEndInRRemoveIt(line2);
            if (line1 != line2) {
                errors.push_back(errorLine);
                return false;
            }
        }
        bool result = file1.eof() && file2.eof();
        if (!result) {
            errors.push_back(errorLine);
        }
        return result;
    }

private:
    const std::string _name;
	const std::string _path;
    const std::string _extension;
	const std::string _parentpath;

	File() {}

    static void ifEndInRRemoveIt(std::string& line) {
        if (line.size() == 0) {
            return;
        }
        if (line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }
    };
};

#endif