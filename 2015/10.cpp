/*
	t = 33:25 min
	enostavno
	t = 1:23 min
	izredno enostavno (enaka koda)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>


std::list<unsigned long long> preberiPodatke(const std::string& pot) {

	std::list<unsigned long long> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	char c;

	while (podatki.peek() != EOF) {
		podatki >> c;
		resitev.push_back(c - '0');
	}

	podatki.close();

	return resitev;
}


int iteriraj(const std::list<unsigned long long>& vhodniPodatki, const int& stIteracij) {

	std::list<unsigned long long> seznam1 = vhodniPodatki;
	std::list<unsigned long long> seznam2 = {};

	std::list<unsigned long long>* bralniSeznam = &seznam1;
	std::list<unsigned long long>* pisalniSeznam = &seznam2;

	for (int i = 0; i < stIteracij; i++) {
		
		for (auto it = bralniSeznam->begin(); it != bralniSeznam->end(); it++) {

			unsigned long long trenutnaVrednost = *it;
			int stTrenutneVrednosti = 1;
			
			auto itPrihodnji = it;
			itPrihodnji++;

			while (itPrihodnji != bralniSeznam->end() && *itPrihodnji == trenutnaVrednost) {
				stTrenutneVrednosti++;
				itPrihodnji++;
			}

			it = --itPrihodnji;

			pisalniSeznam->push_back(stTrenutneVrednosti);
			pisalniSeznam->push_back(trenutnaVrednost);
		}

		bralniSeznam->clear();

		//std::cout << bralniSeznam << '\t' << pisalniSeznam << '\n';

		if (bralniSeznam == &seznam1) {
			bralniSeznam = &seznam2;
			pisalniSeznam = &seznam1;
		}
		else {
			bralniSeznam = &seznam1;
			pisalniSeznam = &seznam2;
		}

		std::cout << i << '\n';
	}

	return bralniSeznam->size();
}


int main() {

	std::list<unsigned long long> podatki = preberiPodatke("2015/10.txt");
	int steviloIteracij = 50;

	size_t resitev1 = iteriraj(podatki, steviloIteracij);
	std::cout << "Po iteraciji " << steviloIteracij << " je stevilo dolgo " << resitev1 << " znakov.\n";


	return 0;
}
