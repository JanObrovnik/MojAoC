/*
	Relativno lahka
	Zelo lahka
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>


enum Smer {
	NAPREJ,
	NAZAJ
};


class Zaporedje {
private:
	std::vector<int> surovec;
	std::vector<std::vector<int>> zaporedje;

public:
	template<class T> Zaporedje(T sez) : surovec(sez.begin(), sez.end()) { zaporedje.push_back(surovec); izracunajZaporedje(); }

	void dodajVrednost(int num) { surovec.push_back(num); }
	template<class T> void nastaviSeznam(T sez) { surovec.assign(sez.begin(), sez.end()); }

	std::vector<int> dobiSeznam() const { return surovec; }
	std::vector<std::vector<int>> dobiZaporedje() const { return zaporedje; }


	void izracunajZaporedje() {

		while (!std::all_of(zaporedje[zaporedje.size() - 1].begin(), zaporedje[zaporedje.size() - 1].end(), [](int i) {return i == 0; })) {

			int* zacetek = &zaporedje[zaporedje.size() - 1][0];
			int* konec = zacetek + zaporedje[zaporedje.size() - 1].size() - 1;

			std::vector<int> podZaporednje;

			for (int* it = zacetek; it != konec; it++)
				podZaporednje.push_back(*(it + 1) - *it);

			zaporedje.push_back(podZaporednje);
		}
	}

	void ekstrapoliraj(Smer smer) {

		std::vector<int>* rzacetek = &zaporedje[zaporedje.size() - 1];
		std::vector<int>* rkonec = &zaporedje[0];
		rkonec--;

		if (smer == NAPREJ) for (std::vector<int>* it = rzacetek; it != rkonec; it--) {

			if (it == rzacetek) it->push_back(0);
			else {
				std::vector<int>* it2 = it + 1;
				it->push_back((*it)[it->size() - 1] + (*it2)[it2->size() - 1]);
			}
		}
		else if (smer == NAZAJ) for (std::vector<int>* it = rzacetek; it != rkonec; it--) {

			if (it == rzacetek) it->insert(it->begin(), 0);
			else {
				std::vector<int>* it2 = it + 1;
				it->insert(it->begin(), (*it)[0] - (*it2)[0]);
			}
		}
	}

	int naslednjaVrednost(Smer smer) {

		ekstrapoliraj(smer);
		
		return (smer == NAPREJ) ? zaporedje[0][zaporedje[0].size() - 1] : zaporedje[0][0];
	}
};

void preberiPodatke(std::list<Zaporedje>& seznam, std::string pot) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (podatki.is_open()) {

		while (podatki.peek() != EOF) {

			int vrednost;
			std::list<int> seznamBranja;

			std::string vrstica;
			std::getline(podatki, vrstica);
			std::stringstream ss(vrstica);

			while (ss >> vrednost)
				seznamBranja.push_back(vrednost);

			seznam.push_back(Zaporedje(seznamBranja));
		}

		podatki.close();
	}
}



int main() {

	std::list<Zaporedje> seznam;

	preberiPodatke(seznam, "PodatkiAdvent23-9.txt");


	int resitevNaprej = 0;

	std::for_each(seznam.begin(), seznam.end(), [&resitevNaprej](Zaporedje& zaporedje) {resitevNaprej += zaporedje.naslednjaVrednost(NAPREJ); });

	std::cout << "Vsota novih vrednost naprej je " << resitevNaprej << '\n';
	std::cout << std::endl;


	int resitevNazaj = 0;

	std::for_each(seznam.begin(), seznam.end(), [&resitevNazaj](Zaporedje& zaporedje) {resitevNazaj += zaporedje.naslednjaVrednost(NAZAJ); });

	std::cout << "Vsota novih vrednost nazaj je " << resitevNazaj << '\n';
	std::cout << std::endl;


	//for (Zaporedje zaporednje : seznam) {
	//	for (std::vector<int> sez : zaporednje.dobiZaporedje()) {
	//		for (int i : sez)
	//			std::cout << i << '\t';
	//
	//		std::cout << '\n';
	//	}
	//	std::cout << '\n';
	//}
	//std::cout << '\n';
	//
	//
	//std::for_each(seznam.begin(), seznam.end(), [](Zaporedje& zaporedje) {zaporedje.naslednjaVrednost(); });
	//
	//for (Zaporedje zaporednje : seznam) {
	//	for (std::vector<int> sez : zaporednje.dobiZaporedje()) {
	//		for (int i : sez)
	//			std::cout << i << '\t';
	//
	//		std::cout << '\n';
	//	}
	//	std::cout << '\n';
	//}
	//std::cout << std::endl;


	return 0;
}
