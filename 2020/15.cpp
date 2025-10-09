/*
	t = 37:23 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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


short najdiNaKoraku(std::vector<short> podatki, const short& iskanKorak = 2020) {

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

	//for (auto i : podatki)
	//	std::cout << i << '\n';

	return podatki.back();
}


int main() {

	std::vector<short> podatki(preberiPodatke("2020/15.txt"));
	
	short korak = 2020;
	short resitev1 = najdiNaKoraku(podatki, korak);
	std::cout << "Na koraku " << korak << " je bila receno stevilo " << resitev1 << ".\n";

	


	return 0;
}