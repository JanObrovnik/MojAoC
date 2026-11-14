/*
	t = 1:06:05 h
	Nekoliko lahko
	t = 32 s
	Zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


class Tocka {
public:
	short x, y;

	Tocka() : x(0), y(0) {}
	Tocka(short x, short y) : x(x), y(y) {}

	bool operator==(const Tocka& to) const {
		return x == to.x && y == to.y;
	}
	bool operator!=(const Tocka& to) const {
		return x != to.x || y != to.y;
	}

	struct hashFunkcija {
		size_t operator()(const Tocka& to) const {
			size_t hashX = std::hash<short>()(to.x);
			size_t hashY = std::hash<short>()(to.y) << 1;
			return hashX ^ hashY;
		}
	};
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


class Oblak {
private:
	Tocka tocka1, tocka2;

public:
	Oblak() : tocka1(), tocka2() {}
	Oblak(Tocka tocka1, Tocka tocka2) : tocka1(tocka1), tocka2(tocka2) {}

	bool preveriHorVer() const { return tocka1.x == tocka2.x || tocka1.y == tocka2.y; }

	void dodajTocke(std::unordered_map<Tocka, short, Tocka::hashFunkcija>& dataBaza) const {

		short dx, dy;

		if (tocka1.x > tocka2.x) dx = 1;
		else if (tocka1.x < tocka2.x) dx = -1;
		else dx = 0;

		if (tocka1.y > tocka2.y) dy = 1;
		else if (tocka1.y < tocka2.y) dy = -1;
		else dy = 0;

		Tocka vmesnaTocka = tocka1;

		while (vmesnaTocka != tocka2) {

			if (dataBaza.find(vmesnaTocka) == dataBaza.end())
				dataBaza[vmesnaTocka] = 1;
			else
				dataBaza[vmesnaTocka]++;

			vmesnaTocka.x -= dx;
			vmesnaTocka.y -= dy;
		}
		if (dataBaza.find(vmesnaTocka) == dataBaza.end())
			dataBaza[vmesnaTocka] = 1;
		else
			dataBaza[vmesnaTocka]++;
	}


	friend std::ostream& operator<<(std::ostream& os, const Oblak& ob);
	friend std::istream& operator>>(std::istream& is, Oblak& ob);
};

std::ostream& operator<<(std::ostream& os, const Oblak& ob) {
	return os << ob.tocka1 << " -> " << ob.tocka2;
}
std::istream& operator>>(std::istream& is, Oblak& ob) {
	is >> ob.tocka1;
	is.ignore(4); 
	is >> ob.tocka2;
	return is;
}


std::unordered_map<Tocka, short, Tocka::hashFunkcija> preberiPodatke(const std::string& pot, const bool& del2 = false) {

	std::unordered_map<Tocka, short, Tocka::hashFunkcija> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo ogoce odpreti.\n";
		return resitev;
	}

	Oblak oblak;

	while (podatki.peek() != EOF) {

		podatki >> oblak;

		if (del2)
			oblak.dodajTocke(resitev);
		else if (oblak.preveriHorVer())
			oblak.dodajTocke(resitev);
	}

	podatki.close();

	return resitev;
}


short steviloVeckratnihTock(const std::unordered_map<Tocka, short, Tocka::hashFunkcija>& dataBaza) {

	short resitev = 0;

	for (const auto& it : dataBaza)
		if (it.second > 1)
			resitev++;

	return resitev;
}


int main() {

	std::unordered_map<Tocka, short, Tocka::hashFunkcija> dataBaza1(preberiPodatke("2021/5.txt", false));

	short resitev1 = steviloVeckratnihTock(dataBaza1);

	std::cout << "Stevilo veckratnih tock je " << resitev1 << ".\n";


	std::unordered_map<Tocka, short, Tocka::hashFunkcija> dataBaza2(preberiPodatke("2021/5.txt", true));

	short resitev2 = steviloVeckratnihTock(dataBaza2);

	std::cout << "Stevilo veckratnih tock je " << resitev2 << ".\n";



	return 0;
}