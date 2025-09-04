#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class Tocka {
public:
	int x, y;
	char visina;

	Tocka(int x, int y, char visina) : x(x), y(y), visina(visina) {}
};

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y, to1.visina + to2.visina);
}
Tocka& operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	to1.visina += to2.visina;
	return to1;
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

std::ostream& operator<<(std::ostream& os, Tocka to) {
	return os << to.visina << " (" << to.x << ", " << to.y << ')';
}


class Vozlisce {
public:
	Tocka tocka;
	int vrednost;
	bool odprta;

	Vozlisce(Tocka tocka) : tocka(tocka), vrednost(-100000), odprta(false) {}
	Vozlisce(Tocka tocka, int vrednost) : tocka(tocka), vrednost(vrednost), odprta(false) {}
};

bool operator==(const Vozlisce& vo1, const Vozlisce& vo2) {
	return vo1.tocka == vo2.tocka;
}
bool operator<(const Vozlisce& vo1, const Vozlisce& vo2) {
	if (vo1.odprta != vo2.odprta) return vo1.odprta > vo2.odprta;
	if (vo1.vrednost != vo2.vrednost) return vo1.vrednost < vo2.vrednost;
	return vo1.tocka < vo2.tocka;
}
bool operator>(const Vozlisce& vo1, const Vozlisce& vo2) {
	if (vo1.odprta != vo2.odprta) return vo1.odprta < vo2.odprta;
	if (vo1.vrednost != vo2.vrednost) return vo1.vrednost > vo2.vrednost;
	return vo1.tocka > vo2.tocka;
}

std::ostream& operator<<(std::ostream& os, Vozlisce vo) {
	return os << vo.odprta << " | " << -vo.vrednost << " | " << vo.tocka;
}


Tocka koncna(0, 0, 'z');

std::vector<Vozlisce> preberiPodatke(std::string pot, bool del1 = true) {

	std::vector<Vozlisce> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	int y(0);

	while (podatki >> vrstica) {

		int x(0);

		for (const char& c : vrstica) {

			if (del1) {
				if		(c == 'S') resitev.push_back(Vozlisce(Tocka(x, y, 'a'), 0));
				else if (c == 'E') { resitev.push_back(Vozlisce(Tocka(x, y, 'z'))); koncna = Tocka(x, y, 'z'); }
				else resitev.push_back(Vozlisce(Tocka(x, y, c)));
			}
			else {
				if		(c == 'S') resitev.push_back(Vozlisce(Tocka(x, y, 'a')));
				else if (c == 'E') resitev.push_back(Vozlisce(Tocka(x, y, 'z'), 0));
				else resitev.push_back(Vozlisce(Tocka(x, y, c)));
			}

			x++;
		}

		y++;
	}

	podatki.close();


	return resitev;
}


void dijkstra(std::vector<Vozlisce> seznamVozlisc) {

	const std::vector<Tocka> seznamSosed{ Tocka(1,0,1),Tocka(0,1,1),Tocka(-1,0,1),Tocka(0,-1,1) };
	
	while (!(seznamVozlisc.front().tocka == koncna)) {

		if (seznamVozlisc.front().odprta) {
			std::cout << "Konca ni bilo mogoce najti\n";
			return;
		}

		for (Tocka soseda : seznamSosed) {

			soseda += seznamVozlisc.front().tocka;

			auto it = std::find_if(seznamVozlisc.begin(), seznamVozlisc.end(), [&](Vozlisce& vo) {
				return vo.tocka == soseda && vo.tocka.visina <= soseda.visina;
				});

			if (it != seznamVozlisc.end() && seznamVozlisc.front().vrednost - 1 > it->vrednost) {
				it->vrednost = seznamVozlisc.front().vrednost - 1;
			}
		}

		seznamVozlisc.front().odprta = true;

		std::make_heap(seznamVozlisc.begin(), seznamVozlisc.end());
	}

	std::cout << seznamVozlisc.front() << '\n';
}


void dijkstra2(std::vector<Vozlisce> seznamVozlisc) {

	const std::vector<Tocka> seznamSosed{ Tocka(1,0,-1),Tocka(0,1,-1),Tocka(-1,0,-1),Tocka(0,-1,-1) };

	while (seznamVozlisc.front().tocka.visina != 'a') {

		if (seznamVozlisc.front().odprta) {
			std::cout << "Konca ni bilo mogoce najti\n";
			return;
		}
		//std::cout << "dela\n" << seznamVozlisc.front() << '\n';
		for (Tocka soseda : seznamSosed) {

			soseda += seznamVozlisc.front().tocka;

			auto it = std::find_if(seznamVozlisc.begin(), seznamVozlisc.end(), [&](Vozlisce& vo) {
				return vo.tocka == soseda && vo.tocka.visina >= soseda.visina;
				});

			if (it != seznamVozlisc.end() && seznamVozlisc.front().vrednost - 1 > it->vrednost) {
				it->vrednost = seznamVozlisc.front().vrednost - 1;
			}
		}

		seznamVozlisc.front().odprta = true;

		std::make_heap(seznamVozlisc.begin(), seznamVozlisc.end());
	}

	std::cout << seznamVozlisc.front() << '\n';
}


int main() {

	std::vector<Vozlisce> seznamVozlisc1(preberiPodatke("Advent22/12.txt"));
	std::make_heap(seznamVozlisc1.begin(), seznamVozlisc1.end());
	dijkstra(seznamVozlisc1);

	std::vector<Vozlisce> seznamVozlisc2(preberiPodatke("Advent22/12.txt", false));
	std::make_heap(seznamVozlisc2.begin(), seznamVozlisc2.end());
	dijkstra2(seznamVozlisc2);


	return 0;
}