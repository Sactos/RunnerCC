#include "Configuration.cpp"
#include "RunnerException.cpp"
#include "SingleClient.cpp"
#include <iostream>
#include <vector>

constexpr auto CONFIG_PATH = "./settings.config";
constexpr auto RESULT_GLOBAL = "./result.txt";

int main(int argc, char **argv) {
    Configuration config = Configuration::load(CONFIG_PATH);
    int number = config.getExercisesNumber();
    number = (number <= 0)? 1 : number;
    std::vector<SingleClient> clients;
    SingleClientSettings settings;
    SingleClient client(settings);
    if (number == 1) {
        clients.push_back(client);
    } else {
        client.setFileCout(std::cout);
        for (int i = 1; i <= number; i++) {
            auto folder = "./" + std::to_string(i);
            if (!FSManager::exists(folder)) {
                FSManager::clearFolder(folder);
            }
            SingleClientSettings settings;
            settings.BIN_PATH = folder + "/bin";
            settings.EXP_PATH = folder + "/exp/";
            settings.IN_PATH = folder + "/in";
            settings.OUT_PATH = folder + "/out/";
            settings.PROJECT_PATH = folder + "/exercise";
            settings.RESULT_PATH = folder + "/";
            clients.push_back(settings);
        }
    }

    for (int i = 0; i < number; i++) {
        try {
            clients[i].setFileCout(std::cout);
            std::cout << "-------------------EJERCICIO: " << (i + 1) << "------------------" << std::endl;
            clients[i].init();
            clients[i].compile(config);
            clients[i].run(config);
        } catch (RunnerException& e) {
            std::cout << e.what() << std::endl;
        }
    }

	if (!config.getCloseImmediatelyUponEnd()) {
		std::cout << "Ingrese un char para terminar la ejecucion..." << std::endl;
		char i;
		std::cin >> i;
	}

    while (!clients.empty()) {
        clients.pop_back();
    }

	return 0;
}