/*
	t = 1:00:56 h
	Nekoliko lahko
	t = 14:25 min
	Zelo lahko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>


struct Prepogni {
	char os;
	short vrednost;

	Prepogni(char os, short vrednost) : os(os), vrednost(vrednost) {}
};

class Tocka {
public:
	short x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(short x, short y) : x(x), y(y) {}

	bool sePrepogne(const Prepogni& prep) const {
		if (prep.os == 'x')
			return x > prep.vrednost;
		if (prep.os == 'y')
			return y > prep.vrednost;
		return false;
	}

	Tocka prepogni(const Prepogni& prep) const {
		if (prep.os == 'x')
			return Tocka(prep.vrednost - (x - prep.vrednost), y);
		if (prep.os == 'y')
			return Tocka(x, prep.vrednost - (y - prep.vrednost));
		return *this;
	}
};

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y;
}
std::istream& operator>>(std::istream& is, Tocka& to) {
	is >> to.x;
	is.ignore(1);
	is >> to.y;
	return is;
}

bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y < to2.y;
	return to1.x < to2.x;
}
bool operator>(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y > to2.y;
	return to1.x > to2.x;
}
bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}


std::pair< std::set<Tocka>, std::vector<Prepogni>> preberiPodatke(const std::string& pot) {

	std::set<Tocka> seznamTock;
	std::vector<Prepogni> seznamPrepogibov;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { seznamTock,seznamPrepogibov };
	}

	std::string vrstica;
	Tocka tocka;

	while (std::getline(podatki, vrstica)) {
		
		if (vrstica.empty())
			break;

		std::stringstream ss(vrstica);
		ss >> tocka;
		seznamTock.insert(tocka);
	}

	char c;
	short i;

	while (podatki.peek() != EOF) {
		
		podatki.ignore(11);
		podatki >> c;
		podatki.ignore(1);
		podatki >> i;

		seznamPrepogibov.push_back(Prepogni(c, i));
	}

	podatki.close();

	return { seznamTock,seznamPrepogibov };
}


void izrisiTocke(const std::set<Tocka>& seznamTock) {

	int maxX = 0, maxY = 0;

	for (const Tocka& tocka : seznamTock) {
		if (tocka.x > maxX) maxX = tocka.x;
		if (tocka.y > maxY) maxY = tocka.y;
	}
	
	maxX++;
	maxY++;

	std::vector<std::string> mapa(maxY, std::string(maxX, '.'));

	for (const Tocka& tocka : seznamTock) {
		mapa[tocka.y][tocka.x] = '#';
	}

	for (const std::string& str : mapa)
		std::cout << str << '\n';
}

short simuliraj1(std::set<Tocka> seznamTock, const std::vector<Prepogni>& seznamPrepogibov, short stPrepogibov = -1) {

	if (stPrepogibov == -1)
		stPrepogibov = seznamPrepogibov.size();

	for (int i = 0; i < stPrepogibov; i++) {
		for (auto it = seznamTock.begin(); it != seznamTock.end(); it++) {
			if (it->sePrepogne(seznamPrepogibov[i])) {
				//Tocka staraTocka = *it;
				//Tocka novaTocka = it->prepogni(seznamPrepogibov[i]);
				seznamTock.insert(it->prepogni(seznamPrepogibov[i]));
				it = seznamTock.erase(it);
			}
		}
	}
	//for (auto it = seznamTock.begin(); it != seznamTock.end(); it++) std::cout << (*it) << '\n';

	return seznamTock.size();
}

short simuliraj2(std::set<Tocka> seznamTock, const std::vector<Prepogni>& seznamPrepogibov, short stPrepogibov = -1, const bool& izrisi = false) {

	if (stPrepogibov == -1)
		stPrepogibov = seznamPrepogibov.size();

	for (int i = 0; i < stPrepogibov; i++) {

		std::set<Tocka> novSeznamTock;

		for (auto it = seznamTock.begin(); it != seznamTock.end(); it++) {
			if (it->sePrepogne(seznamPrepogibov[i])) {
				novSeznamTock.insert(it->prepogni(seznamPrepogibov[i]));
			}
			else {
				novSeznamTock.insert(*it);
			}
		}

		seznamTock = novSeznamTock;
	}
	//for (auto it = seznamTock.begin(); it != seznamTock.end(); it++) std::cout << (*it) << '\n';

	if (izrisi)
		izrisiTocke(seznamTock);

	return seznamTock.size();
}


int main() {

	std::pair<std::set<Tocka>, std::vector<Prepogni>> navodila(preberiPodatke("2021/13.txt"));
	std::set<Tocka>& seznamTock = navodila.first;
	std::vector<Prepogni>& seznamPrepogibov = navodila.second;


	int resitev1 = simuliraj2(seznamTock, seznamPrepogibov, 1);

	std::cout << "Stevilo tock po 1. prepogibu je " << resitev1 << ".\n\n";


	simuliraj2(seznamTock, seznamPrepogibov, -1, true);


	return 0;
}
