/*
	t = 1:11:02 h
	relativno lahko
	t = 1:35:48 h
	sredja tezavnost
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>


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
				return -1;
		}
		return vrednost;
	}

	bool veljavenZnotraj(const short& vrednost, const std::string& naziv) const {

		const std::vector<std::pair<short, short>>& vec = seznamPolj.at(naziv);

		if ((vrednost >= vec.front().first && vrednost <= vec.front().second) ||
			(vrednost >= vec.back().first && vrednost <= vec.back().second))
			return true;

		return false;
	}
};

struct Karta {
	std::vector<short> seznamVrednosti;
};


std::string prvaBesedaStavka(const std::string& stavek) {
	std::stringstream ss(stavek);
	std::string resitev;
	ss >> resitev;
	return resitev;
}


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


void izbrisiNeveljavneKarte(const Polja& polje, std::vector<Karta>& bliznjeKarte) {

	for (int i = 0; i < bliznjeKarte.size(); i++) {
		int napake = 0;
		for (const short& vrednost : bliznjeKarte[i].seznamVrednosti)
			napake += polje.veljaven(vrednost);
		if (napake != -1 * polje.velikost()) {
			bliznjeKarte.erase(bliznjeKarte.begin() + i);
			i--;
		}
	}
	//std::cout << najdiNeveljavneBliznjeKarte(polje, bliznjeKarte)<<'\n';
}
std::map<std::string, short> urediSeznamNazivov(std::vector<std::set<std::string>>& seznamNazivov) {

	std::map<std::string, short> resitev;

	while (true) {

		bool koncaj = true;
		std::string trenutenNaziv = "";

		for (int i = 0; i < seznamNazivov.size(); i++) {
			//std::cout << set.size() << '\n';
			if (seznamNazivov[i].size() == 1) {
				trenutenNaziv = *seznamNazivov[i].begin();
				resitev[trenutenNaziv] = i;
				koncaj = false;
				break;
			}
		}

		for (std::set<std::string>& set : seznamNazivov) {
			set.erase(trenutenNaziv);
		}

		if (koncaj)
			break;
	}
	

	//for (auto it : resitev)
	//	std::cout << it.first << '\t' << it.second << '\n';

	return resitev;
}
long long dolociMojoStevilko(const std::map<std::string, short>& podatki, const Karta& mojaKarta) {

	long long resitev = 1;

	for (const auto& it : podatki)
		if (prvaBesedaStavka(it.first) == "departure")
			resitev *= mojaKarta.seznamVrednosti[it.second];

	return resitev;
}

long long najdiSvojoKarto(const Polja& polje, const Karta& mojaKarta, std::vector<Karta> bliznjeKarte) {

	izbrisiNeveljavneKarte(polje, bliznjeKarte);

	std::vector<std::set<std::string>> seznamNazivov(polje.velikost());

	for (int i = 0; i < seznamNazivov.size(); i++) {
		for (const auto& it : polje.seznamPolj) {
			seznamNazivov[i].insert(it.first);
		}
	}


	for (const Karta& karta : bliznjeKarte) {

		for (int i = 0; i < karta.seznamVrednosti.size(); i++) {

			std::vector<std::string> seznamNeveljavnihNazivov{};

			for (const std::string& naziv : seznamNazivov[i]) {
			
				if (!polje.veljavenZnotraj(karta.seznamVrednosti[i], naziv))
					seznamNeveljavnihNazivov.push_back(naziv);
			}

			for (const std::string& naziv : seznamNeveljavnihNazivov) {

				seznamNazivov[i].erase(naziv);
			}
		}
	}


	const std::map<std::string, short>& urejeniPodatki(urediSeznamNazivov(seznamNazivov));


	return dolociMojoStevilko(urejeniPodatki, mojaKarta);
}


int main() {

	Polja polje;
	Karta mojaKarta;
	std::vector<Karta> bliznjeKarte;

	preberiPodatke("2020/16.txt", polje, mojaKarta, bliznjeKarte);

	int resitev1 = najdiNeveljavneBliznjeKarte(polje, bliznjeKarte);
	std::cout << "Vsota neveljavnih stevil na bliznjih kartah je " << resitev1 << ".\n";

	long long resitev2 = najdiSvojoKarto(polje, mojaKarta, bliznjeKarte);
	std::cout << "Zmnozek mojih iskanih podatkov je " << resitev2 << ".\n";


	return 0;
}
