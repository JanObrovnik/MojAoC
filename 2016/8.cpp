/*
	t = 47:49 min
	lahko
	t = 30 s
	zelo lahko (vsec mi je)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class Tocka {
public:

	int x, y;
	
	Tocka() : x(-1), y(-1) {}
	Tocka(int x, int y) : x(x), y(y) {}
};

bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}


void dodajTocko(std::vector<Tocka>& seznamTock, const Tocka& tocka) {
	if (std::find(seznamTock.begin(), seznamTock.end(), tocka) == seznamTock.end())
		seznamTock.push_back(tocka);
}
void dodajPravokotnik(std::vector<Tocka>& seznamTock, const int& x, const int& y) {
	for (int xx = 0; xx < x; xx++)
		for (int yy = 0; yy < y; yy++)
			dodajTocko(seznamTock, Tocka(xx, yy));
}

void zasukajX(std::vector<Tocka>& seznamTock, const int& x, const int& za, const int& visina) {
	for (Tocka& tocka : seznamTock)
		if (tocka.x == x) {
			tocka.y += za;
			tocka.y %= visina;
		}
}
void zasukajY(std::vector<Tocka>& seznamTock, const int& y, const int& za, const int& sirina) {
	for (Tocka& tocka : seznamTock)
		if (tocka.y == y) {
			tocka.x += za;
			tocka.x %= sirina;
		}
}

void printSeznamTock(const std::vector<Tocka>& seznamTock, const int& sirina, const int& visina) {
	
	std::vector<std::string> tabela(visina, std::string(sirina, '.'));
	
	for (const Tocka& tocka : seznamTock)
		tabela[tocka.y][tocka.x] = '#';
	
	for (const std::string& vrstica : tabela)
		std::cout << vrstica << '\n';
}

int preberiPodatke(const std::string& pot, const int& sirina, const int& visina) {

	std::vector<Tocka> seznamTock;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return -1;
	}

	std::string ukaz;
	int vrednost1, vrednost2;

	while (podatki.peek() != EOF) {

		podatki >> ukaz;

		if (ukaz == "rect") {

			podatki >> vrednost1;
			podatki.ignore(1);
			podatki >> vrednost2;

			dodajPravokotnik(seznamTock, vrednost1, vrednost2);
		}
		else if (ukaz == "rotate") {

			podatki >> ukaz;
			podatki.ignore(3);
			podatki >> vrednost1;
			podatki.ignore(4);
			podatki >> vrednost2;

			if (ukaz == "column") { // x
				zasukajX(seznamTock, vrednost1, vrednost2, visina);
			}
			else if (ukaz == "row") { // y
				zasukajY(seznamTock, vrednost1, vrednost2, sirina);
			}
		}
	}

	podatki.close();

	printSeznamTock(seznamTock, sirina, visina);

	return seznamTock.size();
}


int main() {

	const int sirina = 50;
	const int visina = 6;

	int resitev = preberiPodatke("2016/8.txt", sirina, visina);
	std::cout << "Osvetljenih je " << resitev << " luci.\n";

	return 0;
}
