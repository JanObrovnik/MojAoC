/*
	t = 36:47 min
	precej lahko
	t = 59:34 min
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Tocka {
public:
	int x, y;
	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}
};
Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}
Tocka& operator+=(Tocka& to1, const Tocka& to2) {
	to1 = to1 + to2;
	return to1;
}


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

bool izvenMeja(const std::vector<std::string>& mapa, const int& y, const int& x) {
	return (y < 0 || y >= mapa.size() || x < 0 || x > mapa[y].size());
}

int steviloZasedenihSosed(const std::vector<std::string>& mapa, const int y, const int x) {

	int resitev = 0;

	for (int yy : {-1, 0, 1})
		for (int xx : {-1, 0, 1}) {

			if (xx == 0 && yy == 0)
				continue;

			int yyy = y + yy;
			int xxx = x + xx;

			if (izvenMeja(mapa, yyy, xxx))
				continue;

			resitev += mapa[yyy][xxx] == '#';
		}

	return resitev;
}

int simulirajSedeze1(std::vector<std::string> zacetnaMapa) {

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


int steviloZasedenihVrst(const std::vector<std::string>& mapa, const int& y, const int& x) {

	int resitev = 0;

	std::vector<Tocka> seznamVrst;
	seznamVrst.push_back(Tocka(1, 0));
	seznamVrst.push_back(Tocka(1, 1));
	seznamVrst.push_back(Tocka(0, 1));
	seznamVrst.push_back(Tocka(-1, 1));
	seznamVrst.push_back(Tocka(-1, 0));
	seznamVrst.push_back(Tocka(-1, -1));
	seznamVrst.push_back(Tocka(0, -1));
	seznamVrst.push_back(Tocka(1, -1));
	
	for (const Tocka& soseda : seznamVrst) {

		Tocka iskana(x, y);

	ZANKA:
		iskana += soseda;

		if (izvenMeja(mapa, iskana.y, iskana.x))
			continue;

		if (mapa[iskana.y][iskana.x] == '.')
			goto ZANKA;

		resitev += mapa[iskana.y][iskana.x] == '#';
	}

	return resitev;
}

int simulirajSedeze2(std::vector<std::string> zacetnaMapa) {

	int resitev = 0;

	std::vector<std::string> novaMapa = zacetnaMapa;

	while (true) {

		bool niSpremembe = true;

		for (int y = 0; y < zacetnaMapa.size(); y++)
			for (int x = 0; x < zacetnaMapa[y].size(); x++) {

				switch (zacetnaMapa[y][x]) {
				case 'L':
					if (steviloZasedenihVrst(zacetnaMapa, y, x) == 0) {
						novaMapa[y][x] = '#';
						niSpremembe = false;
					}
					break;

				case '#':
					if (steviloZasedenihVrst(zacetnaMapa, y, x) >= 5) {
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

	int resitev1 = simulirajSedeze1(bazaPodatkov);
	std::cout << "Stevilo zasedenih sedezev po umiritvi je " << resitev1 << ".\n";

	int resitev2 = simulirajSedeze2(bazaPodatkov);
	std::cout << "Stevilo zasedenih sedezev po umiritvi je " << resitev2 << ".\n";

	return 0;
}