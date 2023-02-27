#include"game.hpp"

std::mt19937 gen(std::random_device{}());

std::ostream &operator<<(std::ostream &os, Couleur c) {
	switch (c) {
		case Couleur::Pique:
			os << "Pique";
			break;
		case Couleur::Coeur:
			os << "Coeur";
			break;
		case Couleur::Trefle:
			os << "Trefle";
			break;
		case Couleur::Carreau:
			os << "Carreau";
			break;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, Valeur v) {
	switch (v) {
		case Valeur::As:
			os << "As";
			break;
		case Valeur::Roi:
			os << "Roi";
			break;
		case Valeur::Dame:
			os << "Dame";
			break;
		case Valeur::Valet:
			os << "Valet";
			break;
		case Valeur::Dix:
			os << "Dix";
			break;
		case Valeur::Neuf:
			os << "Neuf";
			break;
		case Valeur::Huit:
			os << "Huit";
			break;
		case Valeur::Sept:
			os << "Sept";
			break;
	}
	return os;
}


std::ostream &operator<<(std::ostream &os, Carte c) {
	auto [val, coul] = c;
	os << val << " de " << coul;
	return os;
}

Paquet Jeu::createPaquet(){
	Paquet provPaquet = {};
	for (auto c = couleurs.begin(); c != couleurs.end(); c++){
		for (auto v = valeurs.begin(); v != valeurs.end(); v++){
			provPaquet.push_back(std::tuple {*v,*c});
		}
	}
	std::shuffle (provPaquet.begin(), provPaquet.end(), gen);
	return provPaquet;
}