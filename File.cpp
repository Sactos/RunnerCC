#ifndef FILE_CPP
#define FILE_CPP

#include <string>
#include <experimental/filesystem>

class File {
public:
	const std::string Name;
	const std::string Path;
    const std::string Extension;
	const std::string ParentPath;

    File(const std::experimental::filesystem::path& path) : 
        Name(path.filename().string()), 
        Path(path.string()),
        Extension(path.extension().string()),
		ParentPath(path.parent_path().string()) { };

    const std::string NameWithOut(const std::string& extencion) const {
        auto pos = Name.find(extencion);
        if (pos == std::string::npos) {
            return Name;
        }
        return Name.substr(0, pos);
    }

    const std::string NameNoExtension() const {
        return NameWithOut(Extension);
    }

    const std::string PathNoExtension() const {
        auto pos = Path.find(Extension);
        if (pos == std::string::npos) {
            return Name;
        }
        return Path.substr(0, pos);
    }

private:
	File() {}
};

#endif