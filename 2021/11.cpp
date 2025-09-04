/*
	t = 1:08:15 h
	nekoliko lahko
	t = 15:53 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>


struct Hobotnica {
	short energija;
	bool zeOsvetljena;

	Hobotnica(short energija) : energija(energija), zeOsvetljena(false) {}

	void ponastavi() {
		if (zeOsvetljena) {
			energija = 0;
			zeOsvetljena = false;
		}
	}
};


std::vector<std::vector<Hobotnica>> preberiPodatke(const std::string& pot) {

	std::vector<std::vector<Hobotnica>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}
	
	char c;

	for (int y = 0; y < 10; y++) {
		resitev.push_back(std::vector<Hobotnica>());
		for (int x = 0; x < 10; x++) {
			podatki >> c;
			resitev[y].push_back(c - '0');
		}
	}

	podatki.close();

	return resitev;
}


void osvetlji(std::vector<std::vector<Hobotnica>>& mapa, const short& xx, const short& yy, int& resitev) {

	Hobotnica& hobotnica = mapa[yy][xx];

	hobotnica.zeOsvetljena = true;
	resitev++;

	for (short i = -1; i <= 1; i++)
		for (short j = -1; j <= 1; j++) {
			
			short y = yy + i;
			short x = xx + j;

			if (y < 0 || x < 0 || y >= 10 || x >= 10)
				continue;

			Hobotnica& hobotnica = mapa[y][x];

			hobotnica.energija++;

			if (hobotnica.energija > 9 && hobotnica.zeOsvetljena == false)
				osvetlji(mapa, x, y, resitev);
		}
}

std::pair <int,int> simuliraj(std::vector<std::vector<Hobotnica>> mapa, int steviloIteracij) {

	int resitev1 = 0;
	int resitev2 = -1;

	for (int i = 0; i != steviloIteracij; i++) {

		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {

				Hobotnica& hobotnica = mapa[y][x];

				hobotnica.energija++;

				if (hobotnica.energija > 9 && hobotnica.zeOsvetljena == false)
					osvetlji(mapa, x, y, resitev1);
			}
		}

		for (int y = 0; y < 10; y++)
			for (int x = 0; x < 10; x++)
				mapa[y][x].ponastavi();

		bool vseBliskajo = true;
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				if (!vseBliskajo)
					break;
				if (mapa[y][x].energija != 0) {
					vseBliskajo = false;
					break;
				}
			}
		}
		if (vseBliskajo && resitev2 == -1) {
			resitev2 = i + 1;
			break;
		}
	}

	return { resitev1,resitev2 };
}


int main() {

	std::vector<std::vector<Hobotnica>> mapa(preberiPodatke("2021/11.txt"));
	//for (int y = 0; y < 10; y++) {
	//	for (int x = 0; x < 10; x++)
	//		std::cout << mapa[y][x].energija;
	//	std::cout << '\n';
	//}


	std::pair<int,int> resitev1(simuliraj(mapa, 100));
	
	std::cout << "Stevilo osvetljitev je " << resitev1.first << ".\n"; // 1705


	std::pair<int, int> resitev2(simuliraj(mapa, -1));

	std::cout << "Vse se osvetljijo po " << resitev2.second << " iteracijah.\n"; // 1705


	return 0;
}
