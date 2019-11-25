#ifndef FSMANAGER_CPP
#define FSMANAGER_CPP

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#if _WINDOWS_ 
	#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	/*#include <filesystem>
	namespace fs = std::filesystem;*/
#endif
#include <experimental/filesystem>
#include "File.cpp"

namespace fs = std::experimental::filesystem;

namespace FSManager {
	static bool exists(const std::string& path) {
		return fs::exists(path);
	}

    static bool remove(const std::string& path) {
        return fs::remove(path);
    }

    static bool removeAll(const std::string& path) {
        try {
            //fs::remove_all(path); NO BORRA RECURSIVO
		    if (fs::exists(path)) {
                std::vector<std::string> folders { path.c_str() };
                std::vector<std::string> foldersRemoval { path.c_str() };
                while (!folders.empty()) {
			        const std::string currentFolder = folders.back();
			        folders.pop_back();
                    for (auto& f : fs::directory_iterator(currentFolder)) {
                        if (fs::is_directory(f) && !fs::is_empty(f)) {
                            folders.push_back(f.path().string());
                            foldersRemoval.push_back(f.path().string());
                            continue;
                        }
                        fs::remove(f);
                    }
                }
                while (!foldersRemoval.empty()) {
                    const std::string f = foldersRemoval.back();
			        foldersRemoval.pop_back();
                    fs::remove(f);
                }
            }
			return fs::exists(path);
		} catch (fs::filesystem_error&) {
			return false;
		}
    }

    static bool clearFolder(const std::string& path) {
		try {
            removeAll(path);
		    fs::create_directory(path);
			return fs::is_empty(path);
		} catch (fs::filesystem_error&) {
			return false;
		}
	}

	static bool createFolder(const std::string& path) {
		return fs::create_directory(path);
	}

	static bool createFile(const std::string& path) {
		if (!exists(path)) {
			std::ofstream o(path);
			o.close();
			return exists(path);
		}
		return true;
	}

	static std::string fixPath(const std::string& path) {
        auto p = fs::path(path);
        return p.string();
    }

	static File getFile(const std::string& path) {
		auto p = fs::path(path);
		return File(p);
	}

	static std::unique_ptr<std::vector<File>> getFilesInFolder(const std::string& path, const std::vector<std::string>& extensions, bool searchInChildren = false) {
		if (!fs::exists(path)) {
			return nullptr;
		}
        auto files = std::make_unique<std::vector<File>>();

        std::vector<std::string> folders { path.c_str() };
        while (!folders.empty()) {
			const std::string current_folder = folders.back();
			folders.pop_back();

            for (auto& currentFile : fs::directory_iterator(current_folder)) {
                if (fs::is_directory(currentFile)) {
                    if (searchInChildren) {
                        folders.push_back(currentFile.path().string());
                    }
                    continue;
                }

                auto& pathToFile = currentFile.path();
                //string type = pathToFile.extension().string();
                std::string type = pathToFile.filename().string();
                int pos = type.find('.');
                type = type.substr(pos);
                auto it = find(extensions.cbegin(), extensions.cend(), type);
                if (it != extensions.cend()) {
                    files->push_back(File(pathToFile));
                }
            }
        }

		return files;
	}

	static std::unique_ptr<File> getFirstFileInFolder(const std::string& path, const std::vector<std::string>& extensions, bool searchInChildren = false) {
        auto files = getFilesInFolder(path, extensions, searchInChildren);
        if (files == nullptr || files->size() == 0) {
            return nullptr;
        }
        auto temp = (*files)[0];
        return std::make_unique<File>(File(temp));
    }

};

#endif