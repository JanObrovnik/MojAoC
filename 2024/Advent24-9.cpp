/*
	Precej lahko
	Nekoliko lahka
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


std::vector<int> preberiPodatke(std::string pot) {

	std::vector<int> resitev{};


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena.\n";
		return resitev;
	}

	int ponovitev = 0;
	char c;
	while (podatki >> c) {
		
		for (int i = 0; i < c - '0'; i++)
			resitev.push_back((ponovitev % 2 == 0) ? ponovitev / 2 : -1);
		
		ponovitev++;
	}

	podatki.close();


	return resitev;
}


void preurediSpomin(std::vector<int>& vec) {

	int zac = 0;
	int kon = vec.size() - 1;

	while (zac < kon) {

		if (vec[zac] != -1) {
			zac++;
			continue;
		}
		if (vec[kon] == -1) {
			kon--;
			continue;
		}

		std::swap(vec[zac++], vec[kon--]);
	}
}

unsigned long long kontrolnaVsota(std::vector<int>& vec) {

	unsigned long long resitev = 0;

	for (int i = 0; vec[i] != -1; i++)
		resitev += i * vec[i];

	return resitev;
}


int najdiDolzino(std::vector<int>& vec, int mesto, int simbol = -1) {

	int resitev(1);

	for (int i = mesto; i < vec.size(); i++) {
		if (vec[i] == simbol)
			resitev++;
		else break;

		if (i == vec.size() - 1) return 0;
	}

	return resitev;
}

void preurediSpomin2del(std::vector<int>& vec) {

	std::vector<std::pair<int, int>> seznamZacKon;
	seznamZacKon.push_back({ -1,vec.size() });

	int trenutna = *vec.rbegin();

	for (auto it = vec.rbegin(); it != vec.rend(); it++) {

		if (*it != trenutna) {

			if (*it != -1 && trenutna != -1) {
				seznamZacKon.back().first = vec.rend() - it;
				seznamZacKon.push_back({ -1,vec.rend() - it });
			}
			else if (trenutna != -1)
				seznamZacKon.back().first = vec.rend() - it;
			else if (*it != -1)
				seznamZacKon.push_back({ -1,vec.rend() - it });

			trenutna = *it;
		}

	}
	seznamZacKon.back().first = 0;

	//for (auto& par : seznamZacKon) std::cout << par.first << '|' << par.second << '\n';


	for (auto& par : seznamZacKon)
		for (int i = 0; i < par.first; i++) if (vec[i] == -1) {

			int dolzinaPraznine(najdiDolzino(vec, i));
			
			//if (dolzinaPraznine == 0) break;
			if (!(par.second - par.first < dolzinaPraznine)) continue;


			std::swap_ranges(vec.begin() + par.first, vec.begin() + par.second, vec.begin() + i);

			break;
		}
}

unsigned long long kontrolnaVsota2del(std::vector<int>& vec) {

	unsigned long long resitev = 0;

	for (int i = 0; i < vec.size(); i++)
		if (vec[i] != -1)
			resitev += i * vec[i];

	return resitev;
}


int main() {

	std::vector<int> zemljevidDiska(preberiPodatke("Advent24-9.txt"));
	//for (int i : zemljevidDiska) std::cout << i; std::cout << '\n';

	preurediSpomin(zemljevidDiska);
	//for (int i : zemljevidDiska) std::cout << i; std::cout << '\n';

	std::cout << "Kontrolna vsota datotecnega sistema je " << kontrolnaVsota(zemljevidDiska) << ".\n";


	std::vector<int> zemljevidDiska2(preberiPodatke("Advent24-9.txt"));
	//for (int i : zemljevidDiska2) std::cout << i; std::cout <<  '\n';

	preurediSpomin2del(zemljevidDiska2);
	//for (int i : zemljevidDiska2) std::cout << i; std::cout << '\n';

	std::cout << "Druga kontrolna vsota datotecnega sistema je " << kontrolnaVsota2del(zemljevidDiska2) << ".\n";


	return 0;
}
