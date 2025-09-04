/*
	relativno lahka
	nekoliko tezka
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}
};

int manhattanRazdalja(const Tocka& to1, const Tocka& to2) {
	return abs(to1.x - to2.x) + abs(to1.y - to2.y);
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

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << '(' << to.x << ',' << to.y << ')';
}


class Senzor {
public:
	Tocka senzor;
	Tocka svetilnik;

	Senzor(Tocka senzor, Tocka svetilnik) : senzor(senzor), svetilnik(svetilnik) {}

	bool blizjeOdSvetilnika(const Tocka& tocka) const {
		return manhattanRazdalja(senzor, tocka) <= manhattanRazdalja(senzor, svetilnik);
	}

	std::vector<Tocka> dobiSeznamRobnihTock() const {
		
		std::vector<Tocka> resitev{};

		int razdalja(manhattanRazdalja(senzor, svetilnik));
		razdalja++;

		for (int x = 0; x < razdalja; x++)
			resitev.push_back(Tocka(senzor.x + x, senzor.y - razdalja + x));
		for (int y = 0; y < razdalja; y++)
			resitev.push_back(Tocka(senzor.x + razdalja - y, senzor.y + y));
		for (int x = 0; x > -razdalja; x--)
			resitev.push_back(Tocka(senzor.x + x, senzor.y + razdalja + x));
		for (int y = 0; y > -razdalja; y--)
			resitev.push_back(Tocka(senzor.x - razdalja - y, senzor.y + y));

		return resitev;
	}

	std::pair<int, int> rangeX(const int yNivo) const {
		
		int razdalja(manhattanRazdalja(senzor, svetilnik));
		razdalja -= abs(senzor.y - yNivo);
		
		if (razdalja < 0)
			return { 1,-1 }; // neveljavna vrednost
		
		int xL = senzor.x - razdalja;
		int xD = senzor.x + razdalja;
		return { xL,xD };
	}
};



std::vector<std::pair<int, int>> preberiPodatke(std::string pot, const int yNivo, std::vector<Senzor>& seznamSenzorjev) {

	std::vector<std::pair<int, int>> resitev{};
	seznamSenzorjev.clear();


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	int xSen, ySen;
	int xSve, ySve;

	while (podatki.peek() != EOF) {

		podatki.ignore(12);
		podatki >> xSen;
		podatki.ignore(4);
		podatki >> ySen;
		podatki.ignore(25);
		podatki >> xSve;
		podatki.ignore(4);
		podatki >> ySve;
		podatki.ignore(1);

		Senzor senzor(Tocka(xSen, ySen), Tocka(xSve, ySve));

		resitev.push_back(senzor.rangeX(yNivo));
		seznamSenzorjev.push_back(senzor);
	}

	podatki.close();


	return resitev;
}


int steviloZasedenihMest(const std::vector<std::pair<int, int>>& seznamParov, const int yNivo, const std::vector<Senzor>& seznamSenzorjev) {
	
	/*/ poskus optimizacije
	std::pair<int, int> range(1,-1);
	for (const std::pair<int, int>& par : seznamParov) {
	
		if (par == std::pair<int, int>(1, -1))
			continue;
	
		if (range == std::pair<int, int>(1, -1))
			range = par;
	}
	/**/

	std::set<Tocka> seznamTock{};

	for (const std::pair<int, int>& par : seznamParov) {

		for (int x = par.first; x <= par.second; x++)
			seznamTock.insert(Tocka(x, yNivo));
	}

	for (const Senzor& senzor : seznamSenzorjev) {
		
		auto itSen(seznamTock.find(senzor.senzor));
		if (itSen != seznamTock.end())
			seznamTock.erase(itSen);
	
		auto itSve(seznamTock.find(senzor.svetilnik));
		if (itSve != seznamTock.end())
			seznamTock.erase(itSve);
	}

	return seznamTock.size();
	/**/
}


Tocka najdiIskaniSignal(const std::vector<Senzor>& seznamSenzorjev, const int meja = 4000000) {


	for (int i = 0; i < seznamSenzorjev.size(); i++) {

		std::vector<Tocka> seznamRobnihTock(seznamSenzorjev[i].dobiSeznamRobnihTock());

		auto it(std::remove_if(seznamRobnihTock.begin(), seznamRobnihTock.end(), [&](const Tocka& tocka) {
			return tocka.x < 0 || tocka.x > meja || tocka.y < 0 || tocka.y > meja;
			}));
		
		seznamRobnihTock.erase(it, seznamRobnihTock.end());

		for (const Tocka& tocka : seznamRobnihTock) {

			bool veljaven = true;

			for (int j = 0; j < seznamSenzorjev.size(); j++) {

				if (j == i)
					continue;

				if (seznamSenzorjev[j].blizjeOdSvetilnika(tocka)) {
					veljaven = false;
					break;
				}
			}

			if (veljaven)
				return tocka;
		}
	}

	return Tocka(-1, -1);
}



int main() {

	const int yNivo(2000000);

	std::vector<Senzor> seznamSenzorjev{};
	std::vector<std::pair<int, int>> seznamParov(preberiPodatke("Advent22/15.txt", yNivo, seznamSenzorjev));
	
	int resitev1(steviloZasedenihMest(seznamParov, yNivo, seznamSenzorjev));
	std::cout << "Stevilo zasedenih mest je " << resitev1 << ".\n";


	Tocka koncnaLokacija(najdiIskaniSignal(seznamSenzorjev));
	long long resitev2 = 4000000LL * koncnaLokacija.x + koncnaLokacija.y;
	std::cout << "Koncna lokacija: " << koncnaLokacija << " ima vrednost " << resitev2 << ".\n";



	return 0;
}
