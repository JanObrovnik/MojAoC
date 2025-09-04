/*
	Dokaj lahko
	malo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <unordered_set>
#include <bitset>


enum Smer {
	Sever,
	Vzhod,
	Jug,
	Zahod
};


class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}

	void pomik(const Smer& smer) {
		switch (smer) {
		case Sever: y--; break;
		case Vzhod: x++; break;
		case Jug:   y++; break;
		case Zahod: x--; break;
		default:    y--; break;
		}
	}


	struct HashFunkcija {
		size_t operator()(const Tocka& to) const {
			size_t hashX = std::hash<int>()(to.x);
			size_t hashY = std::hash<int>()(to.y) << 1;
			return hashX ^ hashY;
		}
	};

	bool operator==(const Tocka& to) const {
		return x == to.x && y == to.y;
	}
	bool operator<(const Tocka& to) const {
		if (y != to.y) return y < to.y;
		return x < to.x;
	}
	bool operator>(const Tocka& to) const {
		if (y != to.y) return y > to.y;
		return x > to.x;
	}
};

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ' ' << to.y;
}


class Kamen {
public:
	Tocka tocka;

	Kamen(Tocka tocka) : tocka(tocka) {}

	bool pomik(const Smer& smer, std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamLokacijKamnov, const int& maxX, const int& maxY) {

		Tocka osnovnaTocka = tocka;
		Tocka naslednjaTocka = tocka;

		naslednjaTocka.pomik(smer);
		if (seznamLokacijKamnov.find(naslednjaTocka) != seznamLokacijKamnov.end() ||
			naslednjaTocka.x < 0 || naslednjaTocka.y < 0 || naslednjaTocka.x >= maxX || naslednjaTocka.y >= maxY)
			return false;

		while (seznamLokacijKamnov.find(naslednjaTocka) == seznamLokacijKamnov.end()
			&& naslednjaTocka.x >= 0 && naslednjaTocka.y >= 0 && naslednjaTocka.x < maxX && naslednjaTocka.y < maxY) {
			tocka = naslednjaTocka;
			naslednjaTocka.pomik(smer);
		}

		seznamLokacijKamnov.erase(osnovnaTocka);
		seznamLokacijKamnov.insert(tocka);

		return true;
	}

	bool operator==(const Tocka& tocka) {
		return this->tocka == tocka;
	}
	bool operator<(const Tocka& tocka) {
		return this->tocka < tocka;
	}
	bool operator>(const Tocka& tocka) {
		return this->tocka > tocka;
	}
};


std::vector<Kamen> preberiPodatke(const std::string& pot, std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamLokacijKamnov, int& maxX, int& maxY) {

	std::vector<Kamen> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti\n";
		return resitev;
	}

	char c;
	int y = 0;
	int x = 0;

	podatki >> std::noskipws;

	while (podatki.peek() != EOF) {

		podatki >> c;

		if (c == '\n') {
			y++;
			x = 0;
			continue;
		}
		
		if (c == 'O') {
			seznamLokacijKamnov.insert(Tocka(x, y));
			resitev.push_back(Tocka(x, y));
		}
		else if (c == '#') {
			seznamLokacijKamnov.insert(Tocka(x, y));
		}

		x++;
	}
	maxX = x;
	maxY = ++y;

	podatki.close();


	return resitev;
}


int simuliraj(std::vector<Kamen>& kamni, std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamLokacijKamnov, const int& maxX, const int& maxY) {

	static std::list<std::array<std::bitset<100>, 100>*> dataBaza;

	const static std::array<Smer, 4> seznamSmeri{ Sever,Zahod,Jug,Vzhod };

	for (const Smer& smer : seznamSmeri) {
		bool sprememba = true;
		while (sprememba) {
			sprememba = false;
			if (smer == Sever || smer == Zahod) {
				for (auto kamenIt = kamni.begin(); kamenIt != kamni.end(); kamenIt++)
					if (kamenIt->pomik(smer, seznamLokacijKamnov, maxX, maxY))
						sprememba = true;
			}
			else if (smer == Jug || smer == Vzhod) {
				for (auto kamenItr = kamni.rbegin(); kamenItr != kamni.rend(); kamenItr++)
					if (kamenItr->pomik(smer, seznamLokacijKamnov, maxX, maxY))
						sprememba = true;
			}
		}
	}


	std::array<std::bitset<100>, 100>* novPodatek = new std::array<std::bitset<100>, 100>();

	for (Kamen& kamen : kamni)
		(*novPodatek)[kamen.tocka.y].set(kamen.tocka.x);

	int steviloNazaj = 0;
	auto it = std::find_if(dataBaza.rbegin(), dataBaza.rend(), [&](std::array<std::bitset<100>, 100>*& shranjenPodatek) {
		steviloNazaj++;
		return (*shranjenPodatek)== (*novPodatek);
		});
	
	if (it != dataBaza.rend()) {
		//std::cout << "najdeno na iteraciji " << dataBaza.size() << '|' << steviloNazaj << '\n';
		//int resitev2 = 0;
		//for (Kamen& kamen : kamni)
		//	resitev2 += (maxY - kamen.tocka.y);
		//std::cout << "Obremenitev je " << resitev2 << ".\n";
	}
	else {
		//std::cout << "ni najdeno\n";
		steviloNazaj = 0;
	}

	dataBaza.push_back(novPodatek);


	return steviloNazaj;
}

void prikazi(const std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamLokacijKamnov, const int& maxX, const int& maxY) {

	for (int y = 0; y < maxY; y++) {
		for (int x = 0; x < maxX; x++) {
			std::cout << ((seznamLokacijKamnov.find(Tocka(x, y)) == seznamLokacijKamnov.end()) ? '.' : '#');
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

int main() {

	std::unordered_set<Tocka, Tocka::HashFunkcija> seznamLokacijKamnov;
	int maxX = 0;
	int maxY = 0;

	std::vector<Kamen> kamni(preberiPodatke("Advent23/14.txt", seznamLokacijKamnov, maxX, maxY));

	
	// Del 1
	bool sprememba = true;
	while (sprememba) {
		sprememba = false;
		for (auto kamenIt = kamni.begin(); kamenIt != kamni.end(); kamenIt++)
			if (kamenIt->pomik(Sever, seznamLokacijKamnov, maxX, maxY))
				sprememba = true;
	}

	int resitev1 = 0;
	for (Kamen& kamen : kamni)
		resitev1 += (maxY - kamen.tocka.y);

	std::cout << "Obremenitev na severni steber je " << resitev1 << ".\n";


	// Del 2
	const int steviloIteracij = 1000000000;
	bool preskok = false;

	for (int i = 0; i < steviloIteracij; i++) {
		int steviloNazaj = simuliraj(kamni, seznamLokacijKamnov, maxX, maxY);
		if (steviloNazaj > 0 && preskok == false) {
			i = steviloIteracij - ((steviloIteracij - i) % steviloNazaj) - steviloNazaj;
			preskok = true;
		}
	}

	int resitev2 = 0; 
	for (Kamen& kamen : kamni)
		resitev2 += (maxY - kamen.tocka.y);

	std::cout << "Obremenitev na severni steber po 1000000000 zasukih je " << resitev2 << ".\n";



	return 0;
}
