/*
	zelo preprosto
	zelo preprosto
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class Lokacije {
	std::vector<int> seznam1, seznam2;

public:
	Lokacije() : seznam1({}), seznam2({}) {}

	void dodajPodatek(int st1, int st2) {
		seznam1.push_back(st1);
		seznam2.push_back(st2);
	}

	void sortiraj() {
		std::sort(seznam1.begin(), seznam1.end());
		std::sort(seznam2.begin(), seznam2.end());
	}

	int razdalja() const {
		int resitev = 0;

		for (int i = 0; i < seznam1.size(); i++) {
			//std::cout << seznam1[i] << '|' << seznam2[i] << ':' << resitev << '\n';
			resitev += abs(seznam1[i] - seznam2[i]);
		}

		return resitev;
	}

	unsigned long long podobnost() const {
		unsigned long long resitev = 0;

		for (unsigned long long stevilo : seznam1) {
			int ponovitev = std::count(seznam2.begin(), seznam2.end(), stevilo);
			resitev += stevilo * ponovitev;
		}

		return resitev;
	}


	friend std::istream& operator>>(std::istream& is, Lokacije& lo);
};

std::istream& operator>>(std::istream& is, Lokacije& lo) {
	int st1, st2;
	is >> st1 >> st2;
	lo.dodajPodatek(st1, st2);
	return is;
}


Lokacije preberiPodatke(std::string pot) {

	Lokacije resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);


	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena\n";
		return resitev;
	}

	while (podatki.peek() != EOF) {

		podatki >> resitev;
	}

	podatki.close();


	resitev.sortiraj();


	return resitev;
}


int main() {

	Lokacije lokacije(preberiPodatke("Advent24-1.txt"));


	std::cout << "Vsota razlik je " << lokacije.razdalja() << ".\n";

	std::cout << "Podobnostno stevilo je " << lokacije.podobnost() << ".\n";
}