/*
	t = 42:06 min
	lahko
	t = 11:29 min
	lahko (lazje od 1. dela)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>


class Tocka {
public:
	short x, y;
	Tocka() : x(-1), y(-1) {}
	Tocka(short x, short y) : x(x), y(y) {}
};
std::istream& operator>>(std::istream& is, Tocka& to) {
	is >> to.x;
	is.ignore(1);
	is >> to.y;
	return is;
}
std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y;
}

class Ukaz {
public:
	std::string navodilo;
	Tocka zacetek, konec;
};
std::istream& operator>>(std::istream& is, Ukaz& uk) {
	is >> uk.navodilo;
	if (uk.navodilo == "turn")
		is >> uk.navodilo;
	is >> uk.zacetek;
	is.ignore(9);
	is >> uk.konec;
	return is;
}
std::ostream& operator<<(std::ostream& os, const Ukaz& uk) {
	return os << uk.navodilo << "\t|" << uk.zacetek << "|     \t|" << uk.konec << '|';
}


std::vector<Ukaz> preberiPodatke(const std::string& pot) {

	std::vector<Ukaz> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	Ukaz ukaz;

	while (podatki.peek() != EOF) {
		podatki >> ukaz;
		resitev.push_back(ukaz);
	}

	podatki.close();

	return resitev;
}


int steviloLuci(const std::vector<Ukaz>& podatki) {

	int resitev = 0;

	std::array<std::array<short, 1000>, 1000>* polje = new std::array<std::array<short, 1000>, 1000>();

	for (const Ukaz& ukaz : podatki) {
		//std::cout << ukaz << '\n';

		for (int x = ukaz.zacetek.x; x <= ukaz.konec.x; x++)
			for (int y = ukaz.zacetek.y; y <= ukaz.konec.y; y++) {
				
				if (ukaz.navodilo == "on") (*polje)[y][x] = 1;
				else if (ukaz.navodilo == "off") (*polje)[y][x] = 0;
				else (*polje)[y][x]++;
			}
	}

	for (const std::array<short, 1000>& arr : (*polje))
		for (const short& val : arr) {
			resitev += val % 2;
		}

	return resitev;
}


long long mocLuci(const std::vector<Ukaz>& podatki) {

	long long resitev = 0;

	std::array<std::array<short, 1000>, 1000>* polje = new std::array<std::array<short, 1000>, 1000>();

	for (const Ukaz& ukaz : podatki) {
		//std::cout << ukaz << '\n';

		for (int x = ukaz.zacetek.x; x <= ukaz.konec.x; x++)
			for (int y = ukaz.zacetek.y; y <= ukaz.konec.y; y++) {

				if (ukaz.navodilo == "on") (*polje)[y][x]++;
				else if (ukaz.navodilo == "off" && (*polje)[y][x] > 0) (*polje)[y][x]--;
				else if (ukaz.navodilo == "toggle") (*polje)[y][x] += 2;
			}
	}

	for (const std::array<short, 1000>&arr : (*polje))
		for (const short& val : arr) {
			resitev += val;
		}

	return resitev;
}


int main() {

	std::vector<Ukaz> podatki(preberiPodatke("2015/6.txt"));


	int resitev1 = steviloLuci(podatki);
	std::cout << "Stevilo prizganih luci je " << resitev1 << ".\n";

	int resitev2 = mocLuci(podatki);
	std::cout << "Stevilo prizganih luci je " << resitev2 << ".\n";


	return 0;
}
