#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>



enum Smer {
	desno,
	dol,
	levo,
	gor
};

Smer nasprotnaSmer(Smer smer) {
	smer = Smer(smer + 2);
	if (smer > 3)
		smer = Smer(smer - 4);
	return smer;
}


class Tocka {
public:
	int x;
	int y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}

	bool operator==(const Tocka& tocka) const {
		return x == tocka.x && y == tocka.y;
	}

	struct HashFunkcija {
		size_t operator()(const Tocka& tocka) const {
			size_t hashX = std::hash<int>()(tocka.x);
			size_t hashY = std::hash<int>()(tocka.y) << 1;
			return hashX ^ hashY;
		}
	};


	void pomik(const Smer& smer) {
		switch (smer) {
		case (levo):
			x--;
			break;
		case (desno):
			x++;
			break;
		case dol:
			y++;
			break;
		case gor:
			y--;
			break;
		default:
			break;
		}
	}
};

Tocka& operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	return to1;
}
std::vector<Tocka>& operator+=(std::vector<Tocka>& sez, const Tocka& tocka) {
	for (Tocka& to : sez)
		to += tocka;
	return sez;
}
std::vector<Tocka> operator+(std::vector<Tocka> sez, const Tocka& tocka) {
	sez += tocka;
	return sez;
}

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y;
}


class Lik {
public:
	std::vector<Tocka> seznamTock;
	Smer smer;

	Lik(std::vector<Tocka> seznamTock) : seznamTock(seznamTock), smer(dol) {}


	bool padi(const Smer& smer, std::unordered_set<Tocka, Tocka::HashFunkcija>& bazaTock, Tocka& izhodisce) {

		if (pomik(smer, bazaTock))
			pomik(nasprotnaSmer(smer), bazaTock);

		if (pomik(dol, bazaTock)) {
			pomik(gor, bazaTock);
			dopolniBazoTock(bazaTock, izhodisce);
			return false;
		}

		return true;
	}

	bool pomik(const Smer& smer, std::unordered_set<Tocka, Tocka::HashFunkcija>& bazaTock) {

		this->smer = smer;

		std::for_each(seznamTock.begin(), seznamTock.end(), [&](Tocka& tocka) {tocka.pomik(smer); });

		bool trk(std::any_of(seznamTock.begin(), seznamTock.end(), [&](const Tocka& tocka) {
			return bazaTock.find(tocka) != bazaTock.end() || tocka.x < 0 || tocka.x > 6;
			}));

		//bool trk = false;
		//for (const Tocka& tocka : seznamTock) {
		//	if (bazaTock.find(tocka) != bazaTock.end() || tocka.x < 0 || tocka.x > 6)
		//		trk = true;
		//}


		return trk;
	}


	void dopolniBazoTock(std::unordered_set<Tocka, Tocka::HashFunkcija>& bazaTock, Tocka& izhodisce) {

		for (const Tocka& tocka : seznamTock)
			bazaTock.insert(tocka);

		izhodisce.y = std::min(izhodisce.y, najvisjaTocka() - 4);
	}

	int najvisjaTocka() {
		
		const auto it = std::min_element(seznamTock.begin(), seznamTock.end(), [](const Tocka& to1, const Tocka& to2) {
			return to1.y < to2.y;
			});

		return it->y;
	}
};



std::vector<Smer> preberiPodatke(const std::string pot) {

	std::vector<Smer> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti\n";
		return resitev;
	}

	std::string vrstica;
	podatki >> vrstica;

	podatki.close();


	for (const char& ch : vrstica)
		resitev.push_back((ch == '<') ? levo : desno);


	return resitev;
}



void izrisi(const std::unordered_set<Tocka, Tocka::HashFunkcija>& bazaTock, const Tocka& izhodisce) {

	for (int y = izhodisce.y; y < 0; y++) {
		std::string vrstica;
		vrstica.reserve(7);
		for (int x = 0; x < 7; x++) {
			vrstica.push_back((bazaTock.find(Tocka(x, y)) != bazaTock.end()) ? '#' : '.');
		}
		std::cout << vrstica << '\n';
	}
}


int main(int argv, char* argc[]) {

	std::unordered_map<int, std::vector<Tocka>> seznamOblikLikov;
	seznamOblikLikov[0] = { Tocka(0,0), Tocka(1,0) ,Tocka(2,0) ,Tocka(3,0) };
	seznamOblikLikov[1] = { Tocka(1,0), Tocka(0,-1), Tocka(1,-1) ,Tocka(2,-1) ,Tocka(1,-2) };
	seznamOblikLikov[2] = { Tocka(0,0), Tocka(1,0), Tocka(2,0) ,Tocka(2,-1) ,Tocka(2,-2) };
	seznamOblikLikov[3] = { Tocka(0,0), Tocka(0,-1) ,Tocka(0,-2) ,Tocka(0,-3) };
	seznamOblikLikov[4] = { Tocka(0,0), Tocka(1,0) ,Tocka(0,-1) ,Tocka(1,-1) };
	int steviloOblik(seznamOblikLikov.size());

	std::unordered_set<Tocka, Tocka::HashFunkcija> bazaTock;
	bazaTock.insert(Tocka(0, 0));
	bazaTock.insert(Tocka(1, 0));
	bazaTock.insert(Tocka(2, 0));
	bazaTock.insert(Tocka(3, 0));
	bazaTock.insert(Tocka(4, 0));
	bazaTock.insert(Tocka(5, 0));
	bazaTock.insert(Tocka(6, 0));

	Tocka izhodisce(2, -4);


	std::vector<Smer> podatki(preberiPodatke("Advent22/17.txt"));
	int steviloUkazov(podatki.size());
	int indexSmeri(0);


	for (int korak = 0; korak < 2022; korak++) {

		Lik lik(seznamOblikLikov[korak % steviloOblik] + izhodisce);

		while (lik.padi(podatki[indexSmeri++], bazaTock, izhodisce)) {
			if (indexSmeri == steviloUkazov)
				indexSmeri = 0;
		}
		if (indexSmeri == steviloUkazov)
			indexSmeri = 0;
	}


	int resitev = -(izhodisce.y + 4);
	std::cout << "Visina stolpa je " << resitev << '\n';

	//izrisi(bazaTock, izhodisce);
	









	return 0;
}
