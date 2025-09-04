/*
	Relativno enostavno
	Relativno enostavno
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


class VarnostnaNavodila {
	std::vector<int> seznamPrvihSt{};
	std::vector<int> seznamDrugihSt{};
	std::vector<std::vector<int>> vrstniRed{};

	void dodajStevili(std::string vrstica) {

		std::stringstream ss(vrstica);

		int i1, i2;
		char c;

		ss >> i1 >> c >> i2;

		seznamPrvihSt.push_back(i1);
		seznamDrugihSt.push_back(i2);
	}

	void dodajVrstniRed(std::string vrstica) {

		std::stringstream ss(vrstica);

		std::string str;

		vrstniRed.push_back({});

		while (std::getline(ss, str, ','))
			vrstniRed.back().push_back(std::stoi(str));
	}

	std::vector<int> najdiPrvaStevila(int drugoStevilo) {
		std::vector<int> resitev;

		for (int i = 0; i < seznamDrugihSt.size(); i++)
			if (drugoStevilo == seznamDrugihSt[i])
				resitev.push_back(seznamPrvihSt[i]);

		return resitev;
	}

	bool veljavnostVrstnegaReda(std::vector<int> seznam) {

		bool resitev = true;

		for (std::vector<int>::iterator it = seznam.begin(); it != seznam.end(); it++) {

			std::vector<int> seznamPrvih(najdiPrvaStevila(*it));

			for (int i : seznamPrvih) {

				if (std::find(it + 1, seznam.end(), i) != seznam.end())
					resitev = false;
			}
		}

		return resitev;
	}

	void preurediVrstniRed(std::vector<int>& vrsta) {
		//std::cout << '\n';

		while (!veljavnostVrstnegaReda(vrsta)) {

			bool koncaj(false);
			for (std::vector<int>::iterator it = vrsta.begin(); it != vrsta.end(); it++) {

				std::vector<int> seznamPrvih(najdiPrvaStevila(*it));

				for (int i : seznamPrvih) {

					std::vector<int>::iterator it2 = std::find(it + 1, vrsta.end(), i);
					if (it2 != vrsta.end()) {
						//std::cout << *it << '|' << *it2 << '\n';
						std::iter_swap(it, it2);
						koncaj = true;
					}
					if (koncaj) break;
				}
				if (koncaj) break;
			}
		}

	}

public:

	int preveriVrstniRed() {

		int resitev(0);

		std::for_each(vrstniRed.begin(), vrstniRed.end(), [&](auto vrsta) {
			resitev += veljavnostVrstnegaReda(vrsta) * vrsta[(vrsta.size() - 1) / 2];
			});

		return resitev;
	}

	int preveriNapacniVrstniRed() {

		int resitev(0);

		std::for_each(vrstniRed.begin(), vrstniRed.end(), [&](auto vrsta) {
			if (!veljavnostVrstnegaReda(vrsta)) {
				preurediVrstniRed(vrsta);
				if (!veljavnostVrstnegaReda(vrsta)) std::cout << "ERROR\n";
				resitev += vrsta[(vrsta.size() - 1) / 2];
			}
			});

		return resitev;
	}

	void izpis() {
		for (int i = 0; i < seznamPrvihSt.size(); i++)
			std::cout << seznamPrvihSt[i] << '|' << seznamDrugihSt[i] << '\n';
		
		std::cout << '\n';

		for (std::vector<int>& vrsta : vrstniRed) {
			for (int& i : vrsta)
				std::cout << i << ',';
			std::cout << '\n';
		}
	}



	friend void preberiPodatke(std::string pot, VarnostnaNavodila& vn);
};


void preberiPodatke(std::string pot, VarnostnaNavodila& vn) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena\n";
		return;
	}

	std::string vrstica;
	while (std::getline(podatki, vrstica)) {
		
		if (vrstica.empty()) break;

		vn.dodajStevili(vrstica);
	}

	while (std::getline(podatki, vrstica)) {

		vn.dodajVrstniRed(vrstica);
	}
}


int main() {

	VarnostnaNavodila vn;
	preberiPodatke("Advent24-5.txt", vn);
	//vn.izpis();

	int resitev(vn.preveriVrstniRed());


	std::cout << "\nVsota vseh srednjih vrednosti pravilnih vrstnih redov je " << resitev << ".\n";

	int resitev2(vn.preveriNapacniVrstniRed());

	std::cout << "\nVsota vseh srednjih vrednosti popravljenih napacnih vrstnih redov je " << resitev2 << ".\n";


	return 0;
}
