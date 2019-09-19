#include "FSManager.cpp"
#include "Configuration.cpp"
#include "RunnerSystem.cpp"
#include "RunnerException.cpp"
#include "StreambufDoubler.cpp"
#include <iostream>
#include <string>
using std::string;

constexpr auto IN_PATH = "./in";
constexpr auto OUT_PATH = "./out/";
constexpr auto EXP_PATH = "./exp/";
constexpr auto PROJECT_PATH = "./exercise";
constexpr auto BIN_PATH = "./bin";
constexpr auto CONFIG_PATH = "./settings.config";
constexpr auto RESULT_PATH = "./result.txt";

void init() {
	bool result = true;
    std::cout << "Inicializando..." << std::endl;
	result |= FSManager::createFolder(IN_PATH);
	result |= FSManager::createFolder(OUT_PATH);
	result |= FSManager::createFolder(EXP_PATH);
	result |= FSManager::createFolder(PROJECT_PATH);
	result |= FSManager::createFolder(BIN_PATH);
	if (!result) {
		throw RunnerException("ERROR: No se pudieron crear los directorios necesarios o alguno de sus archivos de configuracion.");
	}
}

void compile(RunnerSystem& rcc) {
    std::vector<string> extensions{".cpp", ".java"};
	auto file = FSManager::getFirstFileInFolder(PROJECT_PATH, extensions);
	if (file == nullptr) {
		throw RunnerException("ERROR: No hay archivos validos para compilar dentro de la carpeta exercise.");
	}
	if (!FSManager::clearFolder(BIN_PATH)) {
		throw RunnerException("ERROR: No se pudieron borrar los compilados dentro de la carpeta bin.");
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

void run(Configuration& config, RunnerSystem& rcc) {
    std::vector<string> extensionsIN{ config.getFileInExtension() };
    auto filesIN = FSManager::getFilesInFolder(IN_PATH, extensionsIN);
    if (filesIN == nullptr || filesIN->size() == 0) {
        throw RunnerException("ERROR: No hay archivos de entrada en la carpeta in.");
    }
    std::vector<string> extension{".exe", ".class"};
	auto exe = FSManager::getFirstFileInFolder(BIN_PATH, extension, true);
    if (exe == nullptr) {
        throw RunnerException("ERROR: No se encontró el ejecutable dentro de la carpeta bin.");
    }
    if (!FSManager::clearFolder(OUT_PATH)) {
        throw RunnerException("ERROR: No se pudieron borrar los archivos dentro de la carpeta out.");
    }
    std::cout << "Ejecutando Pruebas... Pruebas a ejecutar: " << filesIN->size() << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    unsigned int countErrors = 0;
    unsigned int notFound = 0;
    for (auto& fileIN : *filesIN) {
        std::cout << "Ejecutando: " << fileIN.name() << "... ";
        auto fileOUT = FSManager::fixPath(OUT_PATH + fileIN.nameWithOut(config.getFileInExtension()) + config.getFileOutExtension());
        bool result = rcc.runTest(*exe, fileIN.path(), fileOUT);
        std::cout << "Finalizado (" << result << "/1)" << std::endl;
        std::cout << "Verificando: " << fileIN.name() << "... ";
		if (!result) {
			countErrors++;
			std::cout << "NO SE PUDO EJECUTAR LA APLICACION" << std::endl;
			std::cout << "-------------------------------------------------" << std::endl;
			continue;
		}
		auto fileEXP = FSManager::fixPath(EXP_PATH + fileIN.nameWithOut(config.getFileInExtension()) + config.getFileExpExtension());
        if (!FSManager::exists(fileEXP)) {
            notFound++;
            std::cout << "NO SE ENCONTRO EL ARCHIVO EXP CORRESPONDIENTE" << std::endl;
            std::cout << "-------------------------------------------------" << std::endl;
            continue;
        }
        auto errors = std::make_shared<std::vector<unsigned int>>();
        result = File::sameContent(fileOUT, fileEXP, *errors);
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
    //CONFIG OUTPUT
    FSManager::createFile(RESULT_PATH);
    auto resultFile = FSManager::getFile(RESULT_PATH);
    resultFile.clear();
    auto resultOutput = std::ofstream(resultFile.path(), std::ofstream::out);
    StreambufDoubler doubler(std::cout.rdbuf(), resultOutput.rdbuf());
    std::cout.rdbuf(&doubler);

	Configuration config = Configuration::load(CONFIG_PATH);
    RunnerSystem rcc = RunnerSystem(config);
	try {
		init();
		compile(rcc);
        run(config, rcc);
	} catch (RunnerException& e) {
		std::cout << e.what() << std::endl;
	}
	if (!config.getCloseImmediatelyUponEnd()) {
		std::cout << "Ingrese un char para terminar la ejecucion..." << std::endl;
		char i;
		std::cin >> i;
	}
	return 0;
}