/*
	t = 2:19:41 h
	nekoliko lahko
	t = 1:06:01 h
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <algorithm>


class Tocka {
public:
	short x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(short x, short y) : x(x), y(y) {}
};

bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y < to2.y;
	return to1.x < to2.x;
}
bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}
bool operator!=(const Tocka& to1, const Tocka& to2) {
	return !(to1 == to2);
}
bool operator>(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y > to2.y;
	return to1.x > to2.x;
}

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}
Tocka& operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	return to1;
}

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << '(' << to.x << ',' << to.y << ')';
}


class Vozlisce {
public:
	Tocka tocka;
	short nevarnost;
	short vrednost;
	bool odprta;

	Vozlisce() : nevarnost(-1), vrednost(0), odprta(false) {}
	Vozlisce(Tocka tocka, short nevarnost) : tocka(tocka), nevarnost(nevarnost), vrednost(10000), odprta(false) {}
	Vozlisce(Tocka tocka, short nevarnost, short vrednost, bool odprta) : tocka(tocka), nevarnost(nevarnost), vrednost(vrednost), odprta(odprta) {}
};

bool operator<(const Vozlisce& vo1, const Vozlisce& vo2) {
	if (vo1.odprta != vo2.odprta) return vo1.odprta < vo2.odprta;
	if (vo1.vrednost != vo2.vrednost) return vo1.vrednost < vo2.vrednost;
	if (vo1.nevarnost != vo2.nevarnost) return vo1.nevarnost < vo2.nevarnost;
	return vo1.tocka < vo2.tocka;
}
bool operator==(const Vozlisce& vo1, const Vozlisce& vo2) {
	return vo1.nevarnost == vo2.nevarnost && vo1.odprta == vo2.odprta && vo1.vrednost == vo2.vrednost && vo1.tocka == vo2.tocka;
}
bool operator>(const Vozlisce& vo1, const Vozlisce& vo2) {
	if (vo1.odprta != vo2.odprta) return vo1.odprta > vo2.odprta;
	if (vo1.vrednost != vo2.vrednost) return vo1.vrednost > vo2.vrednost;
	if (vo1.nevarnost != vo2.nevarnost) return vo1.nevarnost > vo2.nevarnost;
	return vo1.tocka > vo2.tocka;
}

std::ostream& operator<<(std::ostream& os, const Vozlisce& vo) {
	return os << vo.tocka << ' ' << vo.vrednost << '(' << vo.odprta << ") +" << vo.nevarnost;
}


void izrisi(const std::vector<std::vector<Vozlisce>>& seznamVozlisc) {

	for (const std::vector<Vozlisce>& vr : seznamVozlisc) {
		for (const Vozlisce& vo : vr)
			std::cout << vo.nevarnost;
		std::cout << '\n';
	}
}


std::vector<std::vector<Vozlisce>> preberiPodatke(const std::string& pot) {

	std::vector<std::vector<Vozlisce>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki.peek() != EOF) {

		resitev.push_back(std::vector<Vozlisce>());

		std::getline(podatki, vrstica);

		for (short x = 0; x < vrstica.size(); x++)
			resitev.back().push_back(Vozlisce(Tocka(x, resitev.size() - 1), vrstica[x] - '0'));
	}

	podatki.close();

	return resitev;
}


bool veljavnaTocka(const Tocka& tocka, const std::vector<std::vector<Vozlisce>>& seznamVozlisc) {

	const short& xMax = seznamVozlisc.front().size();
	const short& xMin = -1;
	const short& yMax = seznamVozlisc.size();
	const short& yMin = -1;

	const short& x = tocka.x;
	const short& y = tocka.y;

	return x > xMin && x < xMax && y > yMin && y < yMax;
}

short dijkstraAlgorithm(std::vector<std::vector<Vozlisce>> seznamVozlisc) {

	std::set<Vozlisce> setVozlisc;

	Tocka zacetek(0, 0);
	Tocka konec(seznamVozlisc.front().size() - 1, seznamVozlisc.size() - 1);

	Tocka tocka(zacetek);

	seznamVozlisc[zacetek.y][zacetek.x].vrednost = 0;

	const std::array<const Tocka, 4> seznamSosed{ Tocka(1,0), Tocka(0,1), Tocka(-1,0), Tocka(0,-1) };
		
	while (tocka != konec) {

		if (seznamVozlisc[tocka.y][tocka.x].odprta) {
			tocka = setVozlisc.begin()->tocka;
			setVozlisc.erase(setVozlisc.begin());
			continue;
		}

		seznamVozlisc[tocka.y][tocka.x].odprta = true;

		for (Tocka soseda : seznamSosed) {

			soseda += tocka;

			if (!veljavnaTocka(soseda, seznamVozlisc))
				continue;

			if (seznamVozlisc[soseda.y][soseda.x].odprta)
				continue;

			if (seznamVozlisc[soseda.y][soseda.x].nevarnost + seznamVozlisc[tocka.y][tocka.x].vrednost < seznamVozlisc[soseda.y][soseda.x].vrednost) {
				seznamVozlisc[soseda.y][soseda.x].vrednost = seznamVozlisc[soseda.y][soseda.x].nevarnost + seznamVozlisc[tocka.y][tocka.x].vrednost;
				setVozlisc.insert(seznamVozlisc[soseda.y][soseda.x]);
			}
		}

		tocka = setVozlisc.begin()->tocka;
		setVozlisc.erase(setVozlisc.begin());
	}

	return seznamVozlisc[konec.y][konec.x].vrednost;
}


std::vector<std::vector<Vozlisce>> razsiri(const std::vector<std::vector<Vozlisce>>& seznamVozlisc, const short& velikost) {

	std::vector<std::vector<Vozlisce>> resitev(seznamVozlisc);


	//for (const std::vector<Vozlisce>& vr : resitev)
	//	for (const Vozlisce& vo : vr)
	//		std::cout << vo << '\n';

	for (short i = 1; i < velikost; i++)
		for (int y = 0; y < seznamVozlisc.size(); y++)
			for (int x = 0; x < seznamVozlisc.front().size(); x++) {
		
				Vozlisce vozlisce = seznamVozlisc[y][x];

				vozlisce.nevarnost += i;
				while (vozlisce.nevarnost > 9)
					vozlisce.nevarnost -= 9;

				vozlisce.tocka.x += i * seznamVozlisc.front().size();

				resitev[y].push_back(vozlisce);
			}


	std::vector<std::vector<Vozlisce>> vmesni(resitev);

	for (short i = 1; i < velikost; i++)
		for (int y = 0; y < vmesni.size(); y++) {

			resitev.push_back(vmesni[y]);

			for (Vozlisce& vozlisce : resitev.back()) {

				vozlisce.nevarnost += i;
				while (vozlisce.nevarnost > 9)
					vozlisce.nevarnost -= 9;

				vozlisce.tocka.y += i * vmesni.size();
			}
		}

	//std::cout << "--------------------------------\n";
	//for (const std::vector<Vozlisce>& vr : resitev)
	//	for (const Vozlisce& vo : vr)
	//		std::cout << vo << '\n';


	return resitev;
}


int main() {

	std::vector<std::vector<Vozlisce>> seznamVozlisc1(preberiPodatke("2021/15.txt"));
	std::vector<std::vector<Vozlisce>> seznamVozlisc2(razsiri(seznamVozlisc1, 5));

	//izrisi(seznamVozlisc1);
	//std::cout << "\n\n";
	//izrisi(seznamVozlisc2);


	short resitev1(dijkstraAlgorithm(seznamVozlisc1));

	std::cout << "Vsota vseh nevarnosti je " << resitev1 << ".\n";


	short resitev2(dijkstraAlgorithm(seznamVozlisc2));

	std::cout << "Vsota vseh nevarnosti na razsirjenem je " << resitev2 << ".\n";



	return 0;
}
