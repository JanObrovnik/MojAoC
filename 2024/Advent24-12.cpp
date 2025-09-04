/*
	Precej lahka
	Nekoliko lahka
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>


class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}
};
const std::list<Tocka> seznamSosed{ Tocka(1,0),Tocka(0,1), Tocka(-1,0), Tocka(0,-1) };

Tocka operator+(Tocka& to1, Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}

Tocka operator-(Tocka& to1, Tocka& to2) {
	return Tocka(to1.x - to2.x, to1.y - to2.y);
}

Tocka& operator+=(Tocka& to1, Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	return to1;
}

Tocka& operator-=(Tocka& to1, Tocka& to2) {
	to1.x -= to2.x;
	to1.y -= to2.y;
	return to1;
}

bool operator==(Tocka& to1, Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}



class Regija {
	std::list<Tocka> seznamTock;

public:
	Regija() : seznamTock({}) {}

	void dodajTocko(Tocka tocka) {
		seznamTock.push_back(tocka);
	}


	int povrsina() const {
	
		return seznamTock.size();
	}

	int obseg() const {

		int resitev = seznamTock.size() * 4;

		for (Tocka tocka : seznamTock) {

			for (Tocka soseda : seznamSosed) {

				soseda += tocka;

				if (std::any_of(seznamTock.begin(), seznamTock.end(), [&](Tocka to) {return to == soseda; }))
					resitev--;
			}
		}

		return resitev;
	}

	int steviloRob(bool natisni = false) const {

		std::list<Tocka> kopijaTock = seznamTock;

		int minX = std::min_element(kopijaTock.begin(), kopijaTock.end(), [](Tocka& to1, Tocka& to2) {return to1.x < to2.x; })->x;
		int maxX = std::max_element(kopijaTock.begin(), kopijaTock.end(), [](Tocka& to1, Tocka& to2) {return to1.x < to2.x; })->x;

		int minY = std::min_element(kopijaTock.begin(), kopijaTock.end(), [](Tocka& to1, Tocka& to2) {return to1.y < to2.y; })->y;
		int maxY = std::max_element(kopijaTock.begin(), kopijaTock.end(), [](Tocka& to1, Tocka& to2) {return to1.y < to2.y; })->y;

		std::vector<std::string> prikaz(maxY - minY + 3, std::string(maxX - minX + 3, '.'));
		
		
		Tocka zamik(minX, minY);
		std::for_each(seznamTock.begin(), seznamTock.end(), [&](Tocka tocka) {
			tocka -= zamik;
			prikaz[tocka.y + 1][tocka.x + 1] = '#';
			});


		if (natisni) {
			for (std::string str : prikaz)
				std::cout << str << '\n';
			std::cout << '\n';
		}

		int resitev = 0;
		
		for (int y = 1; y < prikaz.size() - 1; y++) {
			bool ograjaZgoraj = false;
			bool ograjaSpodaj = false;

			for (int x = 1; x < prikaz[y].size() - 1; x++) {

				if ('#' == prikaz[y][x]) {
					if ('.' == prikaz[y - 1][x]) {
						if (!ograjaZgoraj) {
							ograjaZgoraj = true;
							resitev++;
						}
					}
					else ograjaZgoraj = false;

					if ('.' == prikaz[y + 1][x]) {
						if (!ograjaSpodaj) {
							ograjaSpodaj = true;
							resitev++;
						}
					}
					else ograjaSpodaj = false;
				}
				else {
					ograjaZgoraj = false;
					ograjaSpodaj = false;
				}
			}
		}

		for (int x = 1; x < prikaz[0].size() - 1; x++) {
			bool ograjaLevo = false;
			bool ograjaDesno = false;

			for (int y = 1; y < prikaz.size() - 1; y++) {

				if ('#' == prikaz[y][x]) {
					if ('.' == prikaz[y][x - 1]) {
						if (!ograjaLevo) {
							ograjaLevo = true;
							resitev++;
						}
					}
					else ograjaLevo = false;

					if ('.' == prikaz[y][x + 1]) {
						if (!ograjaDesno) {
							ograjaDesno = true;
							resitev++;
						}
					}
					else ograjaDesno = false;
				}
				else {
					ograjaLevo = false;
					ograjaDesno = false;
				}
			}
		}


		return resitev;
	}
};



std::vector<std::string> preberiPodatke(std::string pot) {

	std::vector<std::string> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	while (podatki >> vrstica)
		resitev.push_back(vrstica);

	podatki.close();


	return resitev;
}


void poisciRegijo(std::vector<std::string>& podatki, Tocka& to, char ime, Regija& resitev) {

	for (Tocka soseda : seznamSosed) {

		Tocka tocka = to;
		tocka += soseda;

		if (tocka.x < 0 || tocka.y < 0 || tocka.x >= podatki[0].size() || tocka.y >= podatki.size())
			continue;

		if (podatki[tocka.y][tocka.x] == ime) {
			podatki[tocka.y][tocka.x] = '.';
			resitev.dodajTocko(tocka);
			poisciRegijo(podatki, tocka, ime, resitev);
		}
	}
}

std::list<Regija> popisiVrt(std::vector<std::string>& podatki) {

	std::list<Regija> resitev;


	for (int y = 0; y < podatki.size(); y++) {

		for (int x = 0; x < podatki[y].size(); x++) {

			if (podatki[y][x] != '.') {

				char ime = podatki[y][x];
				podatki[y][x] = '.';

				Tocka tocka(x, y);

				Regija regija;
				regija.dodajTocko(tocka);

				poisciRegijo(podatki, tocka, ime, regija);

				resitev.push_back(regija);
			}
		}
	}


	return resitev;
}


int main() {

	std::vector<std::string> podatki(preberiPodatke("Advent24-12.txt"));

	std::list<Regija> regije(popisiVrt(podatki));
	//std::cout << regije.size();

	int resitev = 0;
	for (Regija regija : regije) {
		//std::cout << regija.obseg() << " * " << regija.povrsina() << " = " << regija.obseg() * regija.povrsina() << '\n';
		resitev += regija.obseg() * regija.povrsina();
	}

	std::cout << "Cena 1 je " << resitev << ".\n";


	int resitev2 = 0;
	for (Regija regija : regije) {
		//std::cout << regija.steviloRob() << " * " << regija.povrsina() << " = " << regija.steviloRob() * regija.povrsina() << '\n';
		resitev2 += regija.steviloRob(true) * regija.povrsina();
	}

	std::cout << "Cena 2 je " << resitev2 << ".\n";


	//Regija regijaA;
	//regijaA.dodajTocko(Tocka(0, 0));
	//regijaA.dodajTocko(Tocka(1, 0));
	//regijaA.dodajTocko(Tocka(1, 1));
	//regijaA.dodajTocko(Tocka(0, 1));
	//regijaA.dodajTocko(Tocka(0, 2));
	//
	//std::cout << "Povrsina: " << regijaA.povrsina() << '\n';
	//std::cout << "Obseg: " << regijaA.obseg() << '\n';
	//std::cout << "Stevilo strani " << regijaA.steviloRob() << '\n';



	return 0;
}