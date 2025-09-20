/*
	t = 43:17 min
	lahko
	t = 26:21 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>


class Tocka {
public:
	int x, y;
	int usmerjenost;

	Tocka() : x(0), y(0), usmerjenost() {}
	Tocka(int x, int y) : x(x), y(y), usmerjenost() {}

	void pomakniNaprej(const int& vrednost) {
		switch (usmerjenost) {
		case 0:
			x += vrednost;
			break;
		case 90:
			y += vrednost;
			break;
		case 180:
			x -= vrednost;
			break;
		case 270:
			y -= vrednost;
			break;
		default:
			std::cout << "NAPAKA - tocka_pomakniNaprej";
			break;
		}
	}

	int manhatenRazdalja() const {
		return abs(x) + abs(y);
	}

	void rotirajOkoliNicle(const char& smer, int vrednost) {
		while (vrednost > 0) {
			int vmesna = y;
			y = x;
			x = vmesna;
			if (smer == 'L')
				x *= -1;
			else if (smer == 'R')
				y *= -1;
			vrednost -= 90;
		}
	}

	void pomakniNaprej2(const int& vrednost, const Tocka& smernaTocka) {
		x += smernaTocka.x * vrednost;
		y += smernaTocka.y * vrednost;
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


void premakniLadjo(Tocka& ladja, const char& simbol, const int& vrednost) {

	switch (simbol) {
	case 'N':
		ladja.y += vrednost;
		break;
	case 'E':
		ladja.x += vrednost;
		break;
	case 'S':
		ladja.y -= vrednost;
		break;
	case 'W':
		ladja.x -= vrednost;
		break;
	case 'F':
		ladja.pomakniNaprej(vrednost);
		break;
	case 'R':
		ladja.usmerjenost -= vrednost;
		if (ladja.usmerjenost < 0)
			ladja.usmerjenost += 360;
		break;
	case 'L':
		ladja.usmerjenost += vrednost;
		if (ladja.usmerjenost > 270)
			ladja.usmerjenost -= 360;
		break;
	default:
		std::cout << "NAPAKA - premakniLadjo\n";
		break;
	}
}

void premakniSmernoTocko(Tocka& ladja, Tocka& smernaTocka, const char& simbol, const int& vrednost) {

	switch (simbol) {
	case 'N':
		smernaTocka.y += vrednost;
		break;
	case 'E':
		smernaTocka.x += vrednost;
		break;
	case 'S':
		smernaTocka.y -= vrednost;
		break;
	case 'W':
		smernaTocka.x -= vrednost;
		break;
	case 'F':
		ladja.pomakniNaprej2(vrednost, smernaTocka);
		break;
	case 'R':
		smernaTocka.rotirajOkoliNicle(simbol, vrednost);
		break;
	case 'L':
		smernaTocka.rotirajOkoliNicle(simbol, vrednost);
		break;
	default:
		std::cout << "NAPAKA - premakniSmernoTocko\n";
		break;
	}
}

std::pair<int, int> preberiPodatke(const std::string& pot) {

	int resitev1 = 0;
	int resitev2 = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	Tocka ladja1, ladja2;
	Tocka smernaTocka(10, 1);

	char simbol;
	int vrednost;

	while (podatki.peek() != EOF) {

		podatki >> simbol >> vrednost;

		premakniLadjo(ladja1, simbol, vrednost);
		premakniSmernoTocko(ladja2, smernaTocka, simbol, vrednost);
	}

	podatki.close();

	resitev1 = ladja1.manhatenRazdalja();
	resitev2 = ladja2.manhatenRazdalja();

	return { resitev1, resitev2 };
}


int main() {

	std::pair<int, int> resitve = preberiPodatke("2020/12.txt");

	std::cout << "Manhaten razdalja je " << resitve.first << ".\n";
	std::cout << "Manhaten razdalja s smerno tocko je " << resitve.second << ".\n";

	return 0;
}
