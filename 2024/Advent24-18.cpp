/*
	lahko
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <array>
#include <map>
#include <algorithm>


class Tocka {
public:
	short x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(short x, short y) : x(x), y(y) {}
};
const std::array<Tocka, 4> sosede{ Tocka(0,-1),Tocka(1,0), Tocka(0,1), Tocka(-1,0)};

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}

bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}

bool operator!=(const Tocka& to1, const Tocka& to2) {
	return !(to1.x == to2.x && to1.y == to2.y);
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
	return os << to.x << ',' << to.y;
}

std::istream& operator>>(std::istream& is, Tocka& to) {
	is >> to.x;
	is.ignore(1);
	is >> to.y;
	return is;
}


class VozlisceA {
public:
	Tocka lokacija;
	int cena;
	int oddaljenost;
	Tocka predhodnik;
	bool zeOvrednotena;

	VozlisceA(Tocka lokacija) : 
		lokacija(lokacija), cena(10000), predhodnik(), zeOvrednotena(false) { nastaviOdd(); }
	VozlisceA(Tocka lokacija, int c, Tocka predhodnik) :
		lokacija(lokacija), cena(c), predhodnik(predhodnik), zeOvrednotena(false) { nastaviOdd(); }
	VozlisceA(Tocka lokacija, int c, bool odprt, Tocka predhodnik) :
		lokacija(lokacija), cena(c), predhodnik(predhodnik), zeOvrednotena(odprt) { nastaviOdd(); }

private:

	void nastaviOdd(Tocka koncna = Tocka(70,70)) { /// test (6)70
		oddaljenost = abs(lokacija.x - koncna.x) + abs(lokacija.y - koncna.y);
	}
};

bool operator<(const VozlisceA& vo1, const VozlisceA& vo2) {
	if (vo1.zeOvrednotena != vo2.zeOvrednotena) return vo1.zeOvrednotena < vo2.zeOvrednotena;
	if (vo1.cena != vo2.cena) return vo1.cena < vo2.cena;
	return vo1.lokacija < vo2.lokacija;
}

bool operator>(const VozlisceA& vo1, const VozlisceA& vo2) {
	if (vo1.zeOvrednotena != vo2.zeOvrednotena)
		return vo1.zeOvrednotena > vo2.zeOvrednotena;
	if (vo1.cena * vo1.oddaljenost != vo2.cena * vo2.oddaljenost)
		return vo1.cena * vo1.oddaljenost > vo2.cena * vo2.oddaljenost;
	return vo1.lokacija > vo2.lokacija;
}

bool operator==(const VozlisceA& vo, const Tocka& to) {
	return vo.lokacija == to;
}

bool operator==(const Tocka& to, const VozlisceA& vo) {
	return to == vo.lokacija;
}

bool operator<(const VozlisceA& vo, const Tocka& to) {
	return vo.lokacija < to;
}

bool operator<(const Tocka& to, const VozlisceA& vo) {
	return to < vo.lokacija;
}

std::ostream& operator<<(std::ostream& os, const VozlisceA& vo) {
	return os << vo.cena << '(' << vo.predhodnik << ") | " 
			  << vo.oddaljenost << '(' << vo.lokacija << ") | "
			  << vo.zeOvrednotena;
}


class Spomin {
	std::set<Tocka> prosteTocke;
	std::set<VozlisceA> seznamVozlisc; // bols bi blo 'std::vector' s 'std::make_heap' 
	std::set<Tocka> optimalnaPot;
	Tocka koncna;

public:
	bool resiAzvezdica() {

		optimalnaPot.clear();

		seznamVozlisc.erase(VozlisceA(Tocka(0, 0)));
		seznamVozlisc.insert(VozlisceA(Tocka(0, 0), 0, Tocka()));
		koncna = Tocka(70, 70); /// test (6)70

		auto it = seznamVozlisc.begin();
		while (it->lokacija != koncna ) {

			if (it->cena == 10000) return false;


			VozlisceA trenutnaVozlisce = *it;

			for (Tocka soseda : sosede) {

				soseda = soseda + trenutnaVozlisce.lokacija;

				auto it2 = seznamVozlisc.find(soseda);

				if (it2 != seznamVozlisc.end()) {

					int novaCena = trenutnaVozlisce.cena + 1;
					
					if (!(novaCena < it2->cena)) continue;

					VozlisceA novaSoseda(it2->lokacija, trenutnaVozlisce.cena + 1, trenutnaVozlisce.lokacija);
					seznamVozlisc.erase(it2);
					seznamVozlisc.insert(novaSoseda);
				}
			}


			VozlisceA novoVozlisce(it->lokacija, it->cena, true, it->predhodnik);

			seznamVozlisc.erase(it);
			seznamVozlisc.insert(novoVozlisce);

			it = seznamVozlisc.begin();
		}

		std::cout << "konec: " << *it << '\n';



		Tocka povratniPomik = seznamVozlisc.begin()->predhodnik;

		if (povratniPomik != Tocka()) {
			optimalnaPot.insert(seznamVozlisc.begin()->lokacija);
			optimalnaPot.insert(povratniPomik);

			while (povratniPomik != Tocka(0, 0)) {

				auto it = std::find(seznamVozlisc.rbegin(), seznamVozlisc.rend(), povratniPomik);

				povratniPomik = it->predhodnik;
				optimalnaPot.insert(povratniPomik);
			}
		}


		return true;
	}


	bool dodajTocko(Tocka dodatnaTocka) {

		prosteTocke.erase(dodatnaTocka);


		auto it = optimalnaPot.find(dodatnaTocka);

		if (it == optimalnaPot.end()) return true;

		//prikaz();
		seznamVozlisc = std::set<VozlisceA>(prosteTocke.begin(), prosteTocke.end());
		return resiAzvezdica();
	}


public:
	Spomin(std::set<Tocka> prosteTocke) : 
		prosteTocke(prosteTocke), seznamVozlisc(prosteTocke.begin(), prosteTocke.end()), optimalnaPot({}), koncna() {}



	void prikaz() const {

		std::vector<std::string> mapa(71, std::string(71, '#')); /// test (7)71


		for (auto it = seznamVozlisc.begin(); it != seznamVozlisc.end(); it++) {
			const Tocka& to = it->lokacija;
			mapa[to.y][to.x] = '.';
		}

		for (const Tocka& tocka : optimalnaPot)
			mapa[tocka.y][tocka.x] = 'O';


		std::cout << "Stevilo korakov: " << optimalnaPot.size() - 1 << '\n';

		for (std::string& vrstica : mapa) {
			std::cout << vrstica << '\n';
		}
	}
};



void preberiPodatke(std::string pot) {

	std::set<Tocka> resitev;
	for (int y = 0; y < 71; y++) /// test (7)71
		for (int x = 0; x < 71; x++) /// test (7)71
			resitev.insert(Tocka(x, y));


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce najti.\n";
		return;
	}

	int indeks = 1024;
	int steviloPonovitev = 0;
	while (steviloPonovitev < indeks) {
	//while (steviloPonovitev < 12) { /// test

		Tocka to;
		podatki >> to;

		resitev.erase(to);

		steviloPonovitev++;
	}


	Spomin spomin(resitev);
	spomin.prikaz(); std::cout << '\n';

	spomin.resiAzvezdica();
	spomin.prikaz(); std::cout << '\n';


	Tocka dodatnaTocka;
	podatki >> dodatnaTocka;
	while (spomin.dodajTocko(dodatnaTocka) && podatki.peek() != EOF) {
		
		podatki >> dodatnaTocka;

		indeks++;
	}
	spomin.prikaz();
	std::cout << "Pot se zapre na koraku " << indeks << ", s tocko " << dodatnaTocka << ".\n";


	podatki.close();
}



int main() {

	preberiPodatke("Advent24-18.txt");


	return 0;
}
