/*
	t = 37:23 min
	precej lahko
	t = 48:46 min
	nekoliko tezko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#define NEVELJAVNO -1


std::vector<short> preberiPodatke(const std::string& pot) {
	
	std::vector<short> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	short vrednost;

	while (podatki.peek() != EOF) {
		podatki >> vrednost;
		resitev.push_back(vrednost);
		podatki.ignore(1);
	}

	podatki.close();

	return resitev;
}


short najdiNaKorakuMK1(std::vector<short> podatki, const short& iskanKorak = 2020) {

	while (podatki.size() != iskanKorak) {

		const short& zadnjeStevilo = podatki.back();
		short mestoIstegaStevila = NEVELJAVNO;

		for (short i = podatki.size() - 2; i >= 0; i--)
			if (podatki[i] == zadnjeStevilo) {
				mestoIstegaStevila = i;
				break;
			}

		if (mestoIstegaStevila == NEVELJAVNO)
			podatki.push_back(0);
		else
			podatki.push_back(podatki.size() - mestoIstegaStevila - 1);
	}

	return podatki.back();
}

int najdiNaKorakuMK2(const std::vector<short>& podatki, const int& iskanKorak = 2020) {

	int mesto = 1;

	std::unordered_map<int, int> spomin;

	for (int i = 0; i < podatki.size() - 1; i++)
		spomin[int(podatki[i])] = mesto++;

	int zadnjaVrednost = podatki.back();

	while (mesto < iskanKorak) {
		
		if (mesto % (iskanKorak/100) == 0)
			std::cout << 100ll*mesto/iskanKorak << "%\n";
		
		auto it = spomin.find(zadnjaVrednost);

		if (it == spomin.end()) {
			spomin[zadnjaVrednost] = mesto;
			zadnjaVrednost = 0;
		}
		else {
			const int trenutnoMesto = it->second;
			spomin[zadnjaVrednost] = mesto;
			zadnjaVrednost = mesto - trenutnoMesto;
		}

		mesto++;
	}

	return zadnjaVrednost;
}


int main() {

	std::vector<short> podatki(preberiPodatke("2020/15.txt"));

	int korak = 2020;
	short resitev1 = najdiNaKorakuMK1(podatki, korak);
	std::cout << "Na koraku " << korak << " je bila receno stevilo " << resitev1 << ".\n";

	korak = 30000000;
	int resitev2 = najdiNaKorakuMK2(podatki, korak);
	std::cout << "Na koraku " << korak << " je bila receno stevilo " << resitev2 << ".\n\n";
	

	return 0;
}