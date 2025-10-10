/*
	t = 1:11:02 h
	relativno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


struct Polja {
	std::map<std::string, std::vector<std::pair<short, short>>> seznamPolj;

	void dodaj(std::string vrstica) {
		
		const size_t& mesto = vrstica.find(':');
		const std::string& naziv = vrstica.substr(0, mesto);
		vrstica.erase(0, mesto + 2);

		std::vector<std::pair<short, short>> seznam;

		for (int i = 0; i < 2; i++) {
			
			size_t pretvorba;
			short obmocje1, obmocje2;

			obmocje1 = short(std::stoi(vrstica, &pretvorba, 10));

			vrstica.erase(0, pretvorba + 1);
			//std::cout << pretvorba << ": " << obmocje1 << " - " << vrstica << '\n';

			obmocje2 = short(std::stoi(vrstica, &pretvorba, 10));

			vrstica.erase(0, pretvorba + 4);
			//std::cout << pretvorba << ": " << obmocje2 << " - " << vrstica << '\n';

			seznam.push_back({ obmocje1,obmocje2 });
		}

		seznamPolj[naziv] = seznam;
	}

	size_t velikost() const {
		return seznamPolj.size();
	}

	short veljaven(const short& vrednost) const {
		for (const auto& it : seznamPolj) {
			const std::vector<std::pair<short, short>>& vec = it.second;
			if ((vrednost >= vec.front().first && vrednost <= vec.front().second) ||
				(vrednost >= vec.back().first && vrednost <= vec.back().second))
				return 0;
		}
		return vrednost;
	}
};



struct Karta {
	std::vector<short> seznamVrednosti;


};



void preberiPodatke(const std::string& pot,
	Polja& polja, Karta& mojaKarta, std::vector<Karta>& bliznjeKarte) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	std::string vrstica;
	short vrednost;

	std::getline(podatki, vrstica);
	while (vrstica != "") {
		polja.dodaj(vrstica);
		std::getline(podatki, vrstica);
	}

	std::getline(podatki, vrstica);
	for (int i = 0; i < polja.velikost(); i++) {
		podatki >> vrednost;
		mojaKarta.seznamVrednosti.push_back(vrednost);
		podatki.ignore(1);
	}
	podatki.ignore(1);

	std::getline(podatki, vrstica);
	while (podatki.peek() != EOF) {
		bliznjeKarte.push_back(Karta());
		for (int i = 0; i < polja.velikost(); i++) {
			podatki >> vrednost;
			bliznjeKarte.back().seznamVrednosti.push_back(vrednost);
			podatki.ignore(1);
		}
	}

	podatki.close();
}


int najdiNeveljavneBliznjeKarte(const Polja& polje, const std::vector<Karta>& bliznjeKarte) {

	int resitev = 0;

	for (const Karta& karta : bliznjeKarte)
		for (const short& vrednost : karta.seznamVrednosti)
			resitev += polje.veljaven(vrednost);

	return resitev;
}


int main() {

	Polja polje;
	Karta mojaKarta;
	std::vector<Karta> bliznjeKarte;

	preberiPodatke("2020/16.txt", polje, mojaKarta, bliznjeKarte);

	int resitev1 = najdiNeveljavneBliznjeKarte(polje, bliznjeKarte);
	std::cout << "Vsota neveljavnih stevil na bliznjih kartah je " << resitev1 << ".\n";


	return 0;
}
