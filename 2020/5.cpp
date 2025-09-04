/*
	t = 26:37 min
	lahko (zelo kul)
	t = 27:33 min
	lahko (malo nerazumljiva navodila)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


std::vector<std::string> preberiPodatke(const std::string& pot) {

	std::vector<std::string> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki >> vrstica)
		resitev.push_back(vrstica);

	podatki.close();

	return resitev;
}


int sedezID(const std::string& sedez) {

	int FB = 0;
	int LR = 0;

	for (const char& c : sedez) {
		if (c == 'B') {
			FB = FB << 1;
			FB++;
		}
		else if (c == 'F') {
			FB = FB << 1;
		}
		else if (c == 'R') {
			LR = LR << 1;
			LR++;
		}
		else if (c == 'L') {
			LR = LR << 1;
		}
	}
	//std::cout << 8 * FB + LR << '\n';

	return 8 * FB + LR;
}

int maxSedezID(const std::vector<std::string>& bazaPodatkov) {

	int resitev = 0;

	for (const std::string podatek : bazaPodatkov) {
		const int& novaResitev = sedezID(podatek);
		if (novaResitev > resitev)
			resitev = novaResitev;
	}

	return resitev;
}

int najdiPrazenSedezID(const std::vector<std::string>& bazaPodatkov) {

	std::set<int> seznamSedezevID;

	for (const std::string podatek : bazaPodatkov) {
		const int& novaResitev = sedezID(podatek);
		seznamSedezevID.insert(novaResitev);
	}
	
	int iskanSedez = *seznamSedezevID.begin() - 1;

	for (const int& i : seznamSedezevID) {
		if (++iskanSedez != i)
			return iskanSedez;
	}

	return -1;
}


int main() {

	std::vector<std::string> bazaPodatkov(preberiPodatke("2020/5.txt"));


	int resitev1 = maxSedezID(bazaPodatkov);

	std::cout << "Najvecji ID sedeza je " << resitev1 << ".\n";


	int resitev2 = najdiPrazenSedezID(bazaPodatkov);

	std::cout << "Tvoj ID sedeza je " << resitev2 << ".\n";


	return 0;
}
