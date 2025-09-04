/*
	dokaj lahka - Zelo lustna
	dokaj lahka - Zelo lustna
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>


struct Meje {
	int minX = 500;
	int maxX = 500;
	int minY = 0;
	int maxY = 0;

	void natisni() {
		std::cout << "minX: " << minX << '\n';
		std::cout << "maxX: " << maxX << '\n';
		std::cout << "minY: " << minY << '\n';
		std::cout << "maxY: " << maxY << '\n';
	}
};

class Tocka {
public:
	int x, y;
	char tip;

	Tocka() : x(-1), y(-1), tip('#') {}
	Tocka(int x, int y) : x(x), y(y), tip('#') {}
	Tocka(int x, int y, char tip) : x(x), y(y), tip(tip) {}

	void operator+=(const Tocka& to) {
		x += to.x;
		y += to.y;
	}

	bool operator==(const Tocka& to) const {
		return x == to.x && y == to.y;
	}

	struct HashFunkcija {
		size_t operator()(const Tocka& tocka) const {
			size_t xHash = std::hash<int>()(tocka.x);
			size_t yHash = std::hash<int>()(tocka.y) << 1;
			return xHash ^ yHash;
		}
	};
};

std::istream& operator>>(std::istream& is, Tocka& to) {
	char c;
	is >> to.x >> c >> to.y;
	return is;
}
std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y;
}


void natisniJamo(const std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamTock, const Meje& meje, const bool del2 = false) {
	
	if (!del2) {
		std::vector<std::string> mapa(meje.maxY - meje.minY + 1, std::string(meje.maxX - meje.minX + 1, '.'));

		for (const Tocka& to : seznamTock)
			mapa[to.y][to.x - meje.minX] = to.tip;

		for (const std::string& vrstica : mapa)
			std::cout << vrstica << '\n';
	}

	if (del2) {
		std::vector<std::string> mapa(meje.maxY - meje.minY + 1 + 1, std::string(meje.maxX - meje.minX + 1 + 40, '.'));

		for (const Tocka& to : seznamTock)
			mapa[to.y][to.x - meje.minX + 20] = to.tip;

		for (const std::string& vrstica : mapa)
			std::cout << vrstica << '\n';
	}
}


void popisMed2Tockama(std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamTock, const Tocka& to1, const Tocka& to2) {

	if		(to1.x < to2.x) for (int x = to1.x; x <= to2.x; x++) seznamTock.insert(Tocka(x, to1.y));
	else if (to1.x > to2.x) for (int x = to1.x; x >= to2.x; x--) seznamTock.insert(Tocka(x, to1.y));
	else if (to1.y < to2.y) for (int y = to1.y; y <= to2.y; y++) seznamTock.insert(Tocka(to1.x, y));
	else if (to1.y > to2.y) for (int y = to1.y; y >= to2.y; y--) seznamTock.insert(Tocka(to1.x, y));
	else std::cout << "Napaka - posevna crta\n";
}

void popisTock(std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamTock, const std::vector<Tocka>& seznamRobov) {

	for (int i = 1; i < seznamRobov.size(); i++)
		popisMed2Tockama(seznamTock, seznamRobov[i - 1], seznamRobov[i]);
}

std::unordered_set<Tocka, Tocka::HashFunkcija> preberiPodatke(const std::string pot, Meje& meje) {

	std::unordered_set<Tocka, Tocka::HashFunkcija> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	int x, y;
	char c;

	while (std::getline(podatki,vrstica)) {

		std::vector<Tocka> seznamRobov{};

		std::stringstream ss(vrstica);
		while (ss.peek() != EOF) {

			ss >> x >> c >> y >> c >> c;
			seznamRobov.push_back(Tocka(x, y));

			if		(x < meje.minX) meje.minX = x;
			else if	(x > meje.maxX) meje.maxX = x;
			if		(y > meje.maxY) meje.maxY = y;
			else if (y < meje.minY) meje.minY = y;
		}

		popisTock(resitev, seznamRobov);
	}

	podatki.close();


	return resitev;
}


bool pomikPeska(std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamTock, Tocka& pesek, const Meje& meje, const bool& del2 = false) {

	const std::vector<Tocka> seznamSosed{ Tocka(0,1,'o'),Tocka(-1,1,'o'),Tocka(1,1,'o') };

	for (Tocka soseda : seznamSosed) {

		soseda += pesek;

		auto it(seznamTock.find(soseda));

		if (it == seznamTock.end()) {
			pesek = soseda;
			//if (del2 && soseda.y == meje.maxY + 1)
			//	return false;
			return true;
		}
	}

	return false;
}

const Tocka virPeska(500, 0, '+');
void simuliraj(std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamTock, const Meje& meje, const bool del2 = false) {

	Tocka pesek(virPeska);

	if (!del2) {
		while (true) {
			while (pomikPeska(seznamTock, pesek, meje))
				if (pesek.y > meje.maxY)
					return;

			seznamTock.insert(pesek);

			pesek = virPeska;
		}
	}

	if (del2) {
		while (true) {
			while (pomikPeska(seznamTock, pesek, meje, del2)) {
				if (pesek.y == meje.maxY + 1)
					break;
			}

			seznamTock.insert(pesek);
			if (pesek == virPeska)
				return;

			pesek = virPeska;
		}
	}
}


int main() {

	Meje meje;
	std::unordered_set<Tocka, Tocka::HashFunkcija> seznamTock(preberiPodatke("Advent22/14.txt", meje));
	natisniJamo(seznamTock, meje);
	//meje.natisni();

	int st11(seznamTock.size());
	simuliraj(seznamTock, meje);
	int st12(seznamTock.size());

	std::cout << "Stevilo peska pred prelivanjem je " << st12 - st11 << ".\n";
	natisniJamo(seznamTock, meje);


	simuliraj(seznamTock, meje, true);
	int st22(seznamTock.size());

	std::cout << "Stevilo peska pred prelivanjem je " << st22 - st11 << ".\n";
	//natisniJamo(seznamTock, meje, true);


	return 0;
}