#include "FSManager.cpp"
#include "RunnerSystem.cpp"
#include "RunnerException.cpp"

#include <iostream>

constexpr auto IN_PATH = "./in";
constexpr auto OUT_PATH = "./out/";
constexpr auto EXP_PATH = "./exp/";
constexpr auto PROJECT_PATH = "./exercise";
constexpr auto BIN_PATH = "./bin";
constexpr auto CLOSE_IMMEDIATELY_UPON_END = true;

void init(FSManager& fs) {
	bool result = true;
    std::cout << "Inicializando..." << std::endl;
	result |= fs.createFolder(IN_PATH);
	result |= fs.createFolder(OUT_PATH);
	result |= fs.createFolder(EXP_PATH);
	result |= fs.createFolder(PROJECT_PATH);
	result |= fs.createFolder(BIN_PATH);
	if (!result) {
		throw RunnerException("ERROR: No se pudieron crear los directorios necesarios o alguno de sus archivos de configuración.");
	}
}

void compileCPLUSPLUS(FSManager& fs, RunnerSystem& rcc) {
    vector<string> extensions{".cpp", ".java"};
	auto file = fs.getFirstFileInFolder(PROJECT_PATH, extensions);
	if (file == nullptr) {
		throw RunnerException("ERROR: No hay archivos validos para compilar dentro de la carpeta cpp.");
	}
	if (!fs.clearFolder(BIN_PATH)) {
		throw RunnerException("ERROR: No se pudieron borrar los compilados (.exe) dentro de la carpeta bin.");
	}
    std::cout << "Compilando Archivos..." << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
	bool result = rcc.compileFile(*file, PROJECT_PATH, BIN_PATH);
    if (result) {
        std::cout << "-------------SE COMPILO CORRECTAMENTE------------" << std::endl;
    }
    std::cout << "-------------------------------------------------" << std::endl;
    if (!result) {
        throw RunnerException("ERROR: No se pudo compilar el ejercicio.");
    }
}

void run(FSManager& fs, RunnerSystem& rcc) {
    vector<string> extensionsIN{".in.txt"};
    auto filesIN = fs.getFilesInFolder(IN_PATH, extensionsIN);
    if (filesIN == nullptr || filesIN->size() == 0) {
        throw RunnerException("ERROR: No hay archivos de entrada en la carpeta in.");
    }
    vector<string> extension{".exe", ".class"};
	auto exe = fs.getFirstFileInFolder(BIN_PATH, extension);
    if (exe == nullptr) {
        throw RunnerException("ERROR: No se encontró el ejecutable dentro de la carpeta bin.");
    }
    if (!fs.clearFolder(OUT_PATH)) {
        throw RunnerException("ERROR: No se pudieron borrar los resultados de salida anteriores (out).");
    }
    std::cout << "Ejecutando Pruebas... Pruebas a ejecutar: " << filesIN->size() << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    unsigned int countErrors = 0;
    unsigned int notFound = 0;
    for (auto& fileIN : *filesIN) {
        std::cout << "Ejecutando: " << fileIN.Name << "... ";
        auto fileOUT = fs.getFile(OUT_PATH + fileIN.NameWithOut(".in.txt") + ".out.txt");
        bool result = rcc.runTest(*exe, fileIN.Path, fileOUT.Path);
        std::cout << "Finalizado (" << result << "/1)" << std::endl;
        std::cout << "Verificando: " << fileIN.Name << "... ";
		if (!result) {
			countErrors++;
			std::cout << "NO SE PUDO EJECUTAR LA APLICACIÓN" << std::endl;
			std::cout << "-------------------------------------------------" << std::endl;
			continue;
		}
		auto fileEXP = fs.getFile(EXP_PATH + fileIN.NameWithOut(".in.txt") + ".xp.txt");
        if (!fs.exists(fileEXP.Path)) {
            notFound++;
            std::cout << "NO SE ENCONTRÓ EL ARCHIVO EXP CORRESPONDIENTE" << std::endl;
            std::cout << "-------------------------------------------------" << std::endl;
            continue;
        }
        auto errors = std::make_shared<vector<unsigned int>>();
        result = fs.areFilesEqual(fileOUT.Path, fileEXP.Path, *errors);
        if (result) {
            std::cout << "OK" << std::endl;
        } else {
            countErrors++;
            std::cout << "ERROR EN LINEA: " << errors->front() << std::endl;
        }
        std::cout << "-------------------------------------------------" << std::endl;
    }
    std::cout << "OK (" << ((filesIN->size() - countErrors) - notFound) << "/" << filesIN->size() << ")" << std::endl;
    std::cout << "ERRORES (" << countErrors << "/" << filesIN->size() << ")" << std::endl;
    std::cout << "EXP NO ENCONTRADOS (" << notFound << "/" << filesIN->size() << ")" << std::endl;
}

int main(int argc, char **argv) {
	FSManager fs = FSManager();
    RunnerSystem rcc = RunnerSystem();
	try {
		init(fs);
		compileCPLUSPLUS(fs, rcc);
        run(fs, rcc);
	} catch (RunnerException& e) {
		std::cout << e.what() << std::endl;
	}
	if (!CLOSE_IMMEDIATELY_UPON_END) {
		std::cout << "Ingrese un char para terminar la ejecución..." << std::endl;
		char i;
		std::cin >> i;
	}
	return 0;
}