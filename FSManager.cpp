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
	static bool exists(const string& path) {
		return fs::exists(path);
	}

    static bool remove(const string& path) {
        return fs::remove(path);
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

	static std::unique_ptr<vector<File>> getFilesInFolder(const string& path, const vector<string>& extensions) {
		if (!fs::exists(path)) {
			return nullptr;
		}
		auto files = std::make_unique<vector<File>>();
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

	static std::unique_ptr<File> getFirstFileInFolder(const string& path, const vector<string>& extensions) {
        auto files = getFilesInFolder(path, extensions);
        if (files == nullptr || files->size() == 0) {
            return nullptr;
        }
        auto temp = (*files)[0];
        return std::make_unique<File>(File(temp));
    }

};

#endif