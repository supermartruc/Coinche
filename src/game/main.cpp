#include "view.hpp"
#include "server.hpp"
#include "client.hpp"
#include <algorithm>

int		main(void) {
	std::cout << "Serveur ou client ? (s/c)" << std::endl;

	std::string instance_role;
	std::cin >> instance_role;

	if (instance_role == "s"){
		if (servermain()){
			exit(0);
		}
	}
	else if(instance_role == "c"){
		if (clientmain()){
			exit(0);
		}
	}
	else {
		std::cout << "Role invalide ! " << std::endl;
		exit(0);
	}

	return 0;
}