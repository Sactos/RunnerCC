#ifndef FILE_CPP
#define FILE_CPP

#include <string>
#include <experimental/filesystem>
using std::string;

class File {
public:
	const string Name;
	const string Path;
    const string Extension;
	const string ParentPath;

    File(const std::experimental::filesystem::path& path) : 
        Name(path.filename().string()), 
        Path(path.string()),
        Extension(path.extension().string()),
		ParentPath(path.parent_path().string()) { };

    const string NameWithOut(const string& extencion) const {
        auto pos = Name.find(extencion);
        if (pos == string::npos) {
            return Name;
        }
        return Name.substr(0, pos);
    }

    const string NameNoExtension() const {
        return NameWithOut(Extension);
    }

    const string PathNoExtension() const {
        auto pos = Path.find(Extension);
        if (pos == string::npos) {
            return Name;
        }
        return Path.substr(0, pos);
    }

private:
	File() {}
};

#endif