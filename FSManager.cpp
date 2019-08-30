#ifndef FSMANAGER_CPP
#define FSMANAGER_CPP

#include "File.cpp"
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
using std::string;
using std::vector;
namespace fs = std::experimental::filesystem;

namespace FSManager {
	namespace {
		static void ifEndInRRemoveIt(string& line) {
			if (line.size() == 0) {
				return;
			}
			if (line[line.size() - 1] == '\r') {
				line = line.substr(0, line.size() - 1);
			}
		};
	};

	static bool exists(const string& path) {
		return fs::exists(path);
	}

	static bool clearFolder(const string& path) {
		try {
            fs::remove_all(path);
		    fs::create_directory(path);
			return fs::is_empty(path);
		} catch (fs::filesystem_error&) {
			return false;
		}
	}

	static bool createFolder(const string& path) {
		return fs::create_directory(path);
	}

	static bool createFile(const string& path) {
		if (!exists(path)) {
			std::ofstream o(path);
			o.close();
			return exists(path);
		}
		return true;
	}

	static string fixPath(const string& path) {
        auto p = fs::path(path);
        return p.string();
    }

	static File getFile(const string& path) {
		auto p = fs::path(path);
		return File(p);
	}

	static std::shared_ptr<vector<File>> getFilesInFolder(const string& path, const vector<string>& extensions) {
		if (!fs::exists(path)) {
			return nullptr;
		}
		auto files = std::make_shared<vector<File>>();
		for (auto& currentFile : fs::directory_iterator(path)) {
			if (fs::is_directory(currentFile)) {
				continue;
			}
			auto& pathToFile = currentFile.path();
			//string type = pathToFile.extension().string();
            string type = pathToFile.filename().string();
            int pos = type.find('.');
            type = type.substr(pos);
            auto it = find(extensions.cbegin(), extensions.cend(), type);
			if (it != extensions.cend())
			{
				files->push_back(File(pathToFile));
			}
		}
		return files;
	}

	static std::shared_ptr<File> getFirstFileInFolder(const string& path, const vector<string>& extensions) {
        auto files = getFilesInFolder(path, extensions);
        if (files == nullptr || files->size() == 0) {
            return nullptr;
        }
        auto temp = (*files)[0];
        return std::make_shared<File>(File(temp));
    }

	static bool sameContent(const string& path1, const string& path2, vector<unsigned int> &errors) {
        std::ifstream file1(path1);
        std::ifstream file2(path2);
        string line1, line2;
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

	static std::shared_ptr<vector<string>> readFile(const string& path) {
		std::shared_ptr<vector<string>> lines = std::make_shared<vector<string>>();
		std::ifstream file(path);
		string line;
		while (!file.eof()) {
			std::getline(file, line);
			ifEndInRRemoveIt(line);
			lines->push_back(line);
		}
		return lines;
	}

	static bool writeFile(const string& path, const vector<string>& lines) {
		std::ofstream o(path);
		for (auto line : lines) {
			o << line << std::endl;
		}
		return true;
	}
};

#endif