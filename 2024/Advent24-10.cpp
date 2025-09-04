/*
	Lahka
	Neverjetno lahka (dobeseno samo zbrises 1 vrstico: 47)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


class Tocka {
public:
	short visina;
	short x, y;

	Tocka() : visina(-1), x(0), y(0) {}
	Tocka(short visina, short x, short y) : visina(visina), x(x), y(y) {}
};
const std::array<Tocka, 4> pomik = { Tocka(-1,1,0),Tocka(-1,0,-1), Tocka(-1,-1,0), Tocka(-1,0,1) };

bool operator==(const Tocka& lhs, const Tocka& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

Tocka operator+(const Tocka& lhs, const Tocka& rhs) {
	return Tocka(-1, lhs.x + rhs.x, lhs.y + rhs.y);
}

std::ostream& operator<<(std::ostream& os, Tocka to) {
	return os << to.visina << ':' << to.x << ',' << to.y;
}


class TopografskiZemljevid {
	std::vector<std::string> zemljevid;

	std::vector<Tocka> seznamTock{};
	int steviloPoti = 0;


	void isciIzTocke(Tocka izhodisce, bool enojnaPot) {
		
		if (izhodisce.visina == 9) {
			if (!enojnaPot || !std::any_of(seznamTock.begin(), seznamTock.end(), 
										   [&](Tocka to) {return izhodisce == to; }))
				seznamTock.push_back(izhodisce);
			return;
		}

		for (Tocka pomik : pomik) {
			
			Tocka to = izhodisce + pomik;
			if (to.x < 0 || to.y < 0 || to.x >= zemljevid[0].size() || to.y >= zemljevid.size()) continue;
			if (zemljevid[to.y][to.x] - '0' != izhodisce.visina + 1) continue;

			to.visina = izhodisce.visina + 1;
			isciIzTocke(to, enojnaPot);
		}
	}

public:
	TopografskiZemljevid() : zemljevid({}) {}
	TopografskiZemljevid(std::vector<std::string> zemljevid) : zemljevid(zemljevid) {}

	void preglejZemljevid(bool enojnaPot) {

		steviloPoti = 0;
		for (int y = 0; y < zemljevid.size(); y++)
			for (int x = 0; x < zemljevid[y].size(); x++)
				if (zemljevid[y][x] == '0') {
					isciIzTocke(Tocka(0, x, y), enojnaPot);
					steviloPoti += seznamTock.size();
					seznamTock.clear();
				}
	}

	int dobiSteviloPoti() const {
		return steviloPoti;
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
	while (std::getline(podatki, vrstica))
		resitev.push_back(vrstica);

	podatki.close();


	return resitev;
}


int main() {

	TopografskiZemljevid zemljevid(preberiPodatke("Advent24-10.txt"));

	zemljevid.preglejZemljevid(true);
	std::cout << "Stevilo poti je " << zemljevid.dobiSteviloPoti() << ".\n";

	zemljevid.preglejZemljevid(false);
	std::cout << "Stevilo poti je " << zemljevid.dobiSteviloPoti() << ".\n";


	return 0;
}
