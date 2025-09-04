/*
	t = 2:27:42 h
	relativno lahko (ne rabis programirat za resitev)
	t = 18:11 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}

	bool preveriNotranjost(const std::pair<Tocka, Tocka>& obmocje) const {
		return x >= obmocje.first.x && x <= obmocje.second.x && y >= obmocje.first.y && y <= obmocje.second.y;
	}

	void ucinekOkolja() {
		if (x > 0) x--;
		else if (x < 0) x++;
		y--;
	}
};

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}
Tocka& operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	return to1;
}

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y;
}


std::pair<Tocka, Tocka> preberiPodatke(const std::string& pot) {

	std::pair<Tocka, Tocka> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	podatki.ignore(15);
	podatki >> resitev.first.x;
	podatki.ignore(2);
	podatki >> resitev.second.x;
	podatki.ignore(4);
	podatki >> resitev.first.y;
	podatki.ignore(2);
	podatki >> resitev.second.y;

	podatki.close();

	return resitev;
}


int najdiMax(const int& hitrost) {
	return (hitrost + 1) * hitrost / 2;
}
Tocka najdiMaxTocke(Tocka hitrost) {
	return Tocka(najdiMax(hitrost.x), najdiMax(hitrost.y));
}

int najdiHitrost(const int& max) {
	return ((-1) + sqrt(1 + 8 * max)) / 2;
}


void simuliraj(const std::pair<Tocka, Tocka>& obmocje) {

	int maxY = (-obmocje.first.y);
	bool najdena = false;
	int stIteracije = 0;

	while (true) {
		while (true) {

			Tocka testnaTocka(obmocje.first.x + 1, 0);
			Tocka hitrost(0, maxY);

			stIteracije = 0;

			while (!(testnaTocka.y < obmocje.first.y)) {

				testnaTocka += hitrost;
				hitrost.ucinekOkolja();

				stIteracije++;
				
				if (testnaTocka.preveriNotranjost(obmocje)) {
					najdena = true;
					break;
				}
			}
			if (najdena)
				break;

			maxY--;

			if (maxY <= 0) {
				std::cout << "resitev ni najdena v pozitivni smeri.\n";
				return;
			}
		}
		najdena = false;

		int minX = najdiHitrost(obmocje.first.x);
		int maxX = najdiHitrost(obmocje.second.x);

		for (int x = minX; x <= maxX; x++) {

			Tocka testnaTocka(0, 0);
			Tocka hitrost(x, maxY);

			for (int i = 0; i < stIteracije + 1; i++) {
				testnaTocka += hitrost;
				hitrost.ucinekOkolja();

				if (testnaTocka.preveriNotranjost(obmocje)) {
					std::cout << "Najvisji del je " << najdiMax(maxY) << '\n';
					return;
				}
			}
		}
	}
}

void simuliraj2(const std::pair<Tocka, Tocka>& obmocje) {

	int resitev = 0;

	int minY = obmocje.first.y;
	int maxY = -obmocje.first.y - 1;
	int minX = najdiHitrost(obmocje.first.x);
	int maxX = obmocje.second.x;

	for (int y = minY; y <= maxY; y++) {
		for (int x = minX; x <= maxX; x++) {

			Tocka testnaTocka(0, 0);
			Tocka hitrost(x, y);

			while (testnaTocka.x <= obmocje.second.x && testnaTocka.y >= obmocje.first.y) {

				testnaTocka += hitrost;
				hitrost.ucinekOkolja();

				if (testnaTocka.preveriNotranjost(obmocje)) {
					resitev++;
					break;
				}
			}
		}
	}

	std::cout << "Stevilo moznih resitev je " << resitev << ".\n";
}



void prikaziPot(Tocka hitrost, const int& stIteracij, const std::pair<Tocka,Tocka>& obmocje = {}) {

	std::cout << "max visina (x, y): " << najdiMaxTocke(hitrost) << '\n';

	Tocka pozicija(0, 0);
	std::vector<Tocka> seznamPozicij{pozicija};

	int maxX = 0;
	int maxY = 0;
	int minX = 0;
	int minY = 0;

	if (obmocje.first.x > maxX) maxX = obmocje.first.x;
	else if (obmocje.first.x < minX) minX = obmocje.first.x;
	if (obmocje.first.y > maxY) maxY = obmocje.first.y;
	else if (obmocje.first.y < minY) minY = obmocje.first.y;
	if (obmocje.second.x > maxX) maxX = obmocje.second.x;
	else if (obmocje.second.x < minX) minX = obmocje.second.x;
	if (obmocje.second.y > maxY) maxY = obmocje.second.y;
	else if (obmocje.second.y < minY) minY = obmocje.second.y;

	for (int i = 0; i < stIteracij; i++) {
		
		pozicija += hitrost;
		hitrost.ucinekOkolja();

		seznamPozicij.push_back(pozicija);

		if (pozicija.x > maxX) maxX = pozicija.x;
		else if (pozicija.x < minX) minX = pozicija.x;
		if (pozicija.y > maxY) maxY = pozicija.y;
		else if (pozicija.y < minY) minY = pozicija.y;
	}

	std::vector<std::string> mapa = std::vector<std::string>(maxY - minY + 1, std::string(maxX - minX + 1, '.'));

	for (Tocka to : seznamPozicij) {
		std::cout << to << '\n';
		mapa[to.y - minY][to.x - minX] = '#';
	}
	mapa[obmocje.first.y - minY][obmocje.first.x - minX] = 'O';
	mapa[obmocje.second.y - minY][obmocje.second.x - minX] = 'O';
	mapa[-minY][-minX] = 'X';

	for (const std::string& str : mapa)
		std::cout << str << '\n';
}

int main() {

	std::pair<Tocka, Tocka> obmocje(preberiPodatke("2021/17.txt"));
	//prikaziPot(Tocka(6, 9), 20, obmocje);


	simuliraj(obmocje);

	simuliraj2(obmocje);


	return 0;
}