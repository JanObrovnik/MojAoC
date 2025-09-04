#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}

	void pomik(char smer) {
		if      (smer == '^') y--;
		else if (smer == '>') x++;
		else if (smer == 'v') y++;
		else if (smer == '<') x--;
	}
};

bool operator==(Tocka& lhs, Tocka& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(Tocka& to, std::vector<Tocka>& sez) {
	return std::find_if(sez.begin(), sez.end(), [&](Tocka& tocka) {return tocka == to; }) != sez.end();
}

bool operator!=(Tocka& to, std::vector<Tocka>& sez) {
	return !(to == sez);
}


int preberiPodatke1Del(std::string pot) {

	int resitev(0);


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni bila najdena.\n";
		return resitev;
	}

	std::vector<Tocka> seznamHis{};
	Tocka bozicek;
	seznamHis.push_back(bozicek);
	char c;
	while (podatki >> c) {

		bozicek.pomik(c);

		if (bozicek != seznamHis)
			seznamHis.push_back(bozicek);
	}

	podatki.close();


	resitev = seznamHis.size();


	return resitev;
}

int preberiPodatke2Del(std::string pot) {

	int resitev(0);


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni bila najdena.\n";
		return resitev;
	}

	std::vector<Tocka> seznamHis{};
	std::vector<Tocka> seznamBozickov{ Tocka(), Tocka() };
	seznamHis.push_back(seznamBozickov.front());
	char c;
	while (podatki.peek() != EOF) {

		for (int i = 0; i < seznamBozickov.size(); i++) {
			podatki >> c;
			seznamBozickov[i].pomik(c);
			if (seznamBozickov[i] != seznamHis)
				seznamHis.push_back(seznamBozickov[i]);
		}
	}

	podatki.close();


	resitev = seznamHis.size();


	return resitev;
}


int main() {

	std::cout << "Stevilo his je " << preberiPodatke1Del("Advent15-3.txt") << ".\n";
	std::cout << "Stevilo his je " << preberiPodatke2Del("Advent15-3.txt") << ".\n";


	return 0;
}