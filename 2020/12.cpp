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

std::pair<int, int> preberiPodatke(const std::string& pot) {

	int resitev1 = 0;
	int resitev2 = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	Tocka ladja;

	char simbol;
	int vrednost;

	while (podatki.peek() != EOF) {

		podatki >> simbol >> vrednost;

		premakniLadjo(ladja, simbol, vrednost);
	}

	podatki.close();

	resitev1 = ladja.manhatenRazdalja();

	return { resitev1, resitev2 };
}


int main() {

	std::pair<int, int> resitve = preberiPodatke("2020/12.txt");

	std::cout << "Manhaten razdalja je " << resitve.first << ".\n";
	std::cout << "Manhaten razdalja je " << resitve.second << ".\n";

	return 0;
}
