/*
	t = 1:27:36 ur
	precej lahko (zelo kul)
	t = 12:31 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <bit>


class Tocka {
public:
	int x, y;
	int korak;

	Tocka() : x(-1), y(-1), korak(0) {}
	Tocka(int x, int y) : x(x), y(y), korak(0) {}
	Tocka(int x, int y, int korak) : x(x), y(y), korak(korak) {}

	bool veljavna() const {
		return x >= 0 && y >= 0;
	}
};
Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y, to1.korak + to2.korak);
}
void operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	to1.korak += to2.korak;
}
bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}
bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y < to2.y;
	return to1.x < to2.x;
}
bool operator>(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y > to2.y;
	return to1.x > to2.x;
}
std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << '(' << to.x << ", " << to.y << ')';
}


int preberiPodatke(const std::string& pot) {

	int resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	podatki >> resitev;

	podatki.close();

	return resitev;
}


bool prosto(const Tocka& tocka, const int& najljubsaSt) {

	const unsigned int& x = tocka.x;
	const unsigned int& y = tocka.y;

	const unsigned int racun = x*x + 3*x + 2*x*y + y + y*y + najljubsaSt;

	const unsigned int stEnic = std::popcount(racun);

	return !(stEnic % 2);
}


int potujDoCilja(const Tocka& zacetek, const Tocka& konec, const int& najljubsaSt) {

	std::set<Tocka> seznamObiskanihTock;
	std::vector<Tocka> seznamPrihodnjihTock;
	const std::vector<Tocka> seznamSosed{ Tocka(1,0,1),Tocka(0,1,1) ,Tocka(-1,0,1) ,Tocka(0,-1,1) };

	seznamObiskanihTock.insert(zacetek);

	Tocka zadnja = zacetek;
	
	while (zadnja != konec) {

		for (const Tocka& soseda : seznamSosed) {

			Tocka tekoca = zadnja + soseda;

			if (!tekoca.veljavna())
				continue;

			if (!prosto(tekoca, najljubsaSt))
				continue;

			if (seznamObiskanihTock.contains(tekoca))
				continue;

			if (std::find(seznamPrihodnjihTock.begin(), seznamPrihodnjihTock.end(), tekoca) != seznamPrihodnjihTock.end())
				continue;

			seznamPrihodnjihTock.push_back(tekoca);
		}

		zadnja = seznamPrihodnjihTock.front();
		seznamPrihodnjihTock.erase(seznamPrihodnjihTock.begin());

		seznamObiskanihTock.insert(zadnja);
	}

	return zadnja.korak;
}

int steviloObiskanihLokacij(const Tocka& zacetek, const int& maxKorakov, const int& najljubsaSt) {

	std::set<Tocka> seznamObiskanihTock;
	std::vector<Tocka> seznamPrihodnjihTock;
	const std::vector<Tocka> seznamSosed{ Tocka(1,0,1),Tocka(0,1,1) ,Tocka(-1,0,1) ,Tocka(0,-1,1) };

	seznamObiskanihTock.insert(zacetek);

	Tocka zadnja = zacetek;

	while (true) {

		for (const Tocka& soseda : seznamSosed) {

			Tocka tekoca = zadnja + soseda;

			if (tekoca.korak > maxKorakov)
				continue;

			if (!tekoca.veljavna())
				continue;

			if (!prosto(tekoca, najljubsaSt))
				continue;

			if (seznamObiskanihTock.contains(tekoca))
				continue;

			if (std::find(seznamPrihodnjihTock.begin(), seznamPrihodnjihTock.end(), tekoca) != seznamPrihodnjihTock.end())
				continue;

			seznamPrihodnjihTock.push_back(tekoca);
		}

		if (seznamPrihodnjihTock.size() == 0)
			break;

		zadnja = seznamPrihodnjihTock.front();
		seznamPrihodnjihTock.erase(seznamPrihodnjihTock.begin());

		seznamObiskanihTock.insert(zadnja);
	}

	return seznamObiskanihTock.size();
}


void printPravokotnik(const Tocka& konec, const int& najljubsaSt, const bool& prikaziKoordinate = false) {
	
	if (prikaziKoordinate) {
		std::cout << '\t';
		for (int x = 0; x <= konec.x; x++)
			std::cout << x;
		std::cout << '\n' << '\n';
	}

	char c;

	for (int y = 0; y <= konec.y; y++) {
		
		if (prikaziKoordinate)
			std::cout << y << '\t';
		
		for (int x = 0; x <= konec.x; x++) {

			if (prosto(Tocka(x, y), najljubsaSt)) c = '.';
			else c = '#';

			std::cout << c;
		}
		std::cout << '\n';
	}
}

int main() {

	Tocka zacetek(1, 1);
	Tocka konec(31, 39);
	int maxKorakov = 50;
	int najljubsaSt = preberiPodatke("2016/13.txt");
	//printPravokotnik(Tocka(50,50), najljubsaSt);


	int resitev1 = potujDoCilja(zacetek, konec, najljubsaSt);
	std::cout << "Do lokacije " << konec << " pridemo v " << resitev1 << " korakih.\n";

	int resitev2 = steviloObiskanihLokacij(zacetek, maxKorakov, najljubsaSt);
	std::cout << "V " << maxKorakov << " korakih lahko obiscemo " << resitev2 << " lokacij.\n";


	return 0;
}
