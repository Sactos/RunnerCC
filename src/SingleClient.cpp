#ifndef SINGLECLIENT_CPP
#define SINGLECLIENT_CPP

#include "FSManager.cpp"
#include "Configuration.cpp"
#include "RunnerSystem.cpp"
#include "RunnerException.cpp"
#include "StreambufferDoubler.cpp"
#include <iostream>
#include <string>

constexpr auto RESULT_FILE = "/result.txt";

struct SingleClientSettings {
    std::string IN_PATH = "./in";
    std::string OUT_PATH = "./out/";
    std::string EXP_PATH = "./exp/";
    std::string PROJECT_PATH = "./exercise";
    std::string BIN_PATH = "./bin";
    std::string RESULT_PATH = "./";
};

class SingleClient {
public:
    SingleClient(SingleClientSettings& settings) {
        this->in_path = settings.IN_PATH;
        this->out_path = settings.OUT_PATH;
        this->exp_path = settings.EXP_PATH;
        this->project_path = settings.PROJECT_PATH;
        this->bin_path = settings.BIN_PATH;
        this->result_path = FSManager::fixPath(settings.RESULT_PATH + RESULT_FILE);
        this->interpreter = nullptr;
        this->rcc = nullptr;
    }

    virtual ~SingleClient() {
        if (this->rcc != nullptr) {
            delete this->rcc;
        }
        if (this->interpreter != nullptr) {
            delete this->interpreter;
        }
    }

    void setFileCout(std::ostream& out) {
        FSManager::createFile(this->result_path);
        auto resultFile = FSManager::getFile(this->result_path);
        resultFile.clear();
        auto resultOutput = std::ofstream(resultFile.path(), std::ofstream::out);
        StreambufferDoubler doubler(out.rdbuf(), resultOutput.rdbuf());
        std::cout.rdbuf(&doubler);
    }

    void init() {
        bool result = true;
        std::cout << "Inicializando..." << std::endl;
        result |= FSManager::createFolder(this->in_path);
        result |= FSManager::createFolder(this->out_path);
        result |= FSManager::createFolder(this->exp_path);
        result |= FSManager::createFolder(this->project_path);
        result |= FSManager::createFolder(this->bin_path);
        if (!result) {
            throw RunnerException("ERROR: No se pudieron crear los directorios necesarios o alguno de sus archivos de configuracion.");
        }
        this->was_initialized = true;
    }

    void compile() {
        if (!this->was_initialized) {
            throw RunnerException("ERROR: Debe ejecutar init() de SingleClient antes llamar al metodo compile().");
        }
        std::vector<std::string> extensions{".cpp", ".java"};
        auto file = FSManager::getFirstFileInFolder(this->project_path, extensions);
        if (file == nullptr) {
            throw RunnerException("ERROR: No hay archivos validos para compilar dentro de la carpeta exercise.");
        }
        if (!FSManager::clearFolder(this->bin_path)) {
            throw RunnerException("ERROR: No se pudieron borrar los compilados dentro de la carpeta bin.");
        }
        std::cout << "Compilando Archivos..." << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;
        auto interpreter = this->rcc->interpreter(file->extension());
        bool result = interpreter->compile(this->project_path, this->bin_path);
        if (result) {
            std::cout << "-------------SE COMPILO CORRECTAMENTE------------" << std::endl;
        }
        std::cout << "-------------------------------------------------" << std::endl;
        if (!result) {
            throw RunnerException("ERROR: No se pudo compilar el ejercicio.");
        }
        this->was_compiled = true;
        this->interpreter = interpreter;
    }

    void run(Configuration& config) {
        if (!this->was_compiled) {
            throw RunnerException("ERROR: Debe ejecutar compile() de SingleClient antes llamar al metodo run().");
        }
        std::vector<std::string> extensionsIN{ config.getFileInExtension() };
        auto filesIN = FSManager::getFilesInFolder(this->in_path, extensionsIN);
        if (filesIN == nullptr || filesIN->size() == 0) {
            throw RunnerException("ERROR: No hay archivos de entrada en la carpeta in.");
        }
        if (!FSManager::clearFolder(this->out_path)) {
            throw RunnerException("ERROR: No se pudieron borrar los archivos dentro de la carpeta out.");
        }
        std::cout << "Ejecutando Pruebas... Pruebas a ejecutar: " << filesIN->size() << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;
        unsigned int countErrors = 0;
        unsigned int notFound = 0;
        for (auto fileIN : *filesIN) {
            std::cout << "Ejecutando: " << fileIN.name() << "... ";
            auto fileOUT = FSManager::fixPath(this->out_path + fileIN.nameWithOut(config.getFileInExtension()) + config.getFileOutExtension());
            bool result = this->interpreter->runTest(this->bin_path, fileIN.path(), fileOUT);
            std::cout << "Finalizado (" << result << "/1)" << std::endl;
            std::cout << "Verificando: " << fileIN.name() << "... ";
            if (!result) {
                countErrors++;
                std::cout << "NO SE PUDO EJECUTAR LA APLICACION" << std::endl;
                std::cout << "-------------------------------------------------" << std::endl;
                continue;
            }
            auto fileEXP = FSManager::fixPath(this->exp_path + fileIN.nameWithOut(config.getFileInExtension()) + config.getFileExpExtension());
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
            if(config.getTestEndOnError() && countErrors > 0) {
                break;
            }
        }
        if(!config.getTestEndOnError() || countErrors == 0) {
            std::cout << "OK (" << ((filesIN->size() - countErrors) - notFound) << "/" << filesIN->size() << ")" << std::endl;
        }
        std::cout << "ERRORES (" << countErrors << "/" << filesIN->size() << ")" << std::endl;
        std::cout << "EXP NO ENCONTRADOS (" << notFound << "/" << filesIN->size() << ")" << std::endl;
    }


private:
    std::string in_path;
    std::string out_path;
    std::string exp_path;
    std::string project_path;
    std::string bin_path;
    std::string result_path;
    bool was_initialized;
    bool was_compiled;

    RunnerSystem* rcc;
    Interpreter* interpreter;
};

#endif
