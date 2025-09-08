/*
	t = 36:47 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


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


int steviloZasedenihSosed(const std::vector<std::string>& mapa, const int y, const int x) {

	int resitev = 0;

	for (int yy : {-1, 0, 1})
		for (int xx : {-1, 0, 1}) {

			if (xx == 0 && yy == 0)
				continue;

			int yyy = y + yy;
			int xxx = x + xx;

			if (yyy < 0 || yyy >= mapa.size() || xxx < 0 || xxx > mapa[yyy].size())
				continue;

			resitev += mapa[yyy][xxx] == '#';
		}

	return resitev;
}

int simulirajSedeze(std::vector<std::string> zacetnaMapa) {

	int resitev = 0;

	std::vector<std::string> novaMapa = zacetnaMapa;
	
	while (true) {

		bool niSpremembe = true;

		for (int y = 0; y < zacetnaMapa.size(); y++)
			for (int x = 0; x < zacetnaMapa[y].size(); x++) {

				switch (zacetnaMapa[y][x]) {
				case 'L':
					if (steviloZasedenihSosed(zacetnaMapa, y, x) == 0) {
						novaMapa[y][x] = '#';
						niSpremembe = false;
					}
					break;

				case '#':
					if (steviloZasedenihSosed(zacetnaMapa, y, x) >= 4) {
						novaMapa[y][x] = 'L';
						niSpremembe = false;
					}
					break;

				default:
					break;
				}
			}

		if (niSpremembe)
			break;

		zacetnaMapa = novaMapa;
	}

	for (const auto& vrstica : novaMapa)
		for (const auto& c : vrstica)
			resitev += c == '#';

	return resitev;
}


int main() {

	std::vector<std::string> bazaPodatkov(preberiPodatke("2020/11.txt"));

	int resitev1 = simulirajSedeze(bazaPodatkov);
	std::cout << "Stevilo zasedenih sedezev po umiritvi je " << resitev1 << ".\n";


	return 0;
}