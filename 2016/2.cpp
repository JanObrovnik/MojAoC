/*
	t ~ 30 min
	lahko
	t = 25:24 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>


int premakni(int vrednost, const char& smer) {

	if		(smer == 'U' && (vrednost == 1 || vrednost == 2 || vrednost == 3)) return vrednost;
	else if (smer == 'D' && (vrednost == 7 || vrednost == 8 || vrednost == 9)) return vrednost;
	else if (smer == 'L' && (vrednost == 1 || vrednost == 4 || vrednost == 7)) return vrednost;
	else if (smer == 'R' && (vrednost == 3 || vrednost == 6 || vrednost == 9)) return vrednost;

	if		(smer == 'U') vrednost -= 3;
	else if (smer == 'D') vrednost += 3;
	else if (smer == 'L') vrednost -= 1;
	else if (smer == 'R') vrednost += 1;

	return vrednost;
}

std::string preberiPodatke1(const std::string& pot) {

	std::string resitev = "";

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	int vrednost = 5;

	while (podatki.peek() != EOF) {

		podatki >> vrstica;

		for (const char& smer : vrstica) {
			vrednost = premakni(vrednost, smer);
		}

		resitev.push_back(vrednost + '0');
	}

	podatki.close();

	return resitev;
}


class Koordinate {
public:
	int x, y;

	Koordinate() : x(0), y(0) {}
	Koordinate(int x, int y) : x(x), y(y) {}

	void print() const {
		std::cout << x << ", " << y << '\n';
	}
};

bool operator==(const Koordinate& ko1, const Koordinate& ko2) {
	return ko1.x == ko2.x && ko1.y == ko2.y;
}
bool operator<(const Koordinate& ko1, const Koordinate& ko2) {
	if (ko1.y != ko2.y) return ko1.y < ko2.y;
	return ko1.x < ko2.x;
}

Koordinate premakni(const Koordinate& koordinate, const char& smer, const std::map<Koordinate, char>& tipkalnik) {

	Koordinate novaKoordinata = koordinate;

	if		(smer == 'U') novaKoordinata.y--;
	else if (smer == 'D') novaKoordinata.y++;
	else if (smer == 'L') novaKoordinata.x--;
	else if (smer == 'R') novaKoordinata.x++;

	if (tipkalnik.find(novaKoordinata) != tipkalnik.cend()) return novaKoordinata;
	return koordinate;
}

std::string preberiPodatke2(const std::string& pot) {

	std::string resitev = "";

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	Koordinate koordinate(0,2);

	std::map<Koordinate, char> tipkalnik;
	tipkalnik[Koordinate(2, 0)] = '1';
	tipkalnik[Koordinate(1, 1)] = '2';
	tipkalnik[Koordinate(2, 1)] = '3';
	tipkalnik[Koordinate(3, 1)] = '4';
	tipkalnik[Koordinate(0, 2)] = '5';
	tipkalnik[Koordinate(1, 2)] = '6';
	tipkalnik[Koordinate(2, 2)] = '7';
	tipkalnik[Koordinate(3, 2)] = '8';
	tipkalnik[Koordinate(4, 2)] = '9';
	tipkalnik[Koordinate(1, 3)] = 'A';
	tipkalnik[Koordinate(2, 3)] = 'B';
	tipkalnik[Koordinate(3, 3)] = 'C';
	tipkalnik[Koordinate(2, 4)] = 'D';

	while (podatki.peek() != EOF) {

		podatki >> vrstica;

		for (const char& smer : vrstica) {
			koordinate = premakni(koordinate, smer, tipkalnik);
		}

		resitev.push_back(tipkalnik[koordinate]);
	}

	podatki.close();

	return resitev;
}


int main() {

	std::string resitev1 = preberiPodatke1("2016/2.txt");
	std::cout << "Prva koda je " << resitev1 << ".\n";

	std::string resitev2 = preberiPodatke2("2016/2.txt");
	std::cout << "Druga koda je " << resitev2 << ".\n";


	return 0;
}
