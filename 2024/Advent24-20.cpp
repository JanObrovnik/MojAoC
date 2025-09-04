#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


class Tocka {
public:
	int x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(int x, int y) : x(x), y(y) {}
};

bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}

bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.x != to2.x) return to1.x < to2.x;
	return to1.y < to2.y;
}

bool operator>(const Tocka& to1, const Tocka& to2) {
	if (to1.x != to2.x) return to1.x > to2.x;
	return to1.y > to2.y;
}

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y;
}


class VozlisceD {
public:
	Tocka lokacija;
	int cena;
	Tocka predhodnik;
	bool odprta;

	VozlisceD(Tocka lokacija) : lokacija(lokacija), cena(100000), predhodnik(), odprta(false) {}
};

bool operator<(VozlisceD vo1, VozlisceD vo2) {
	if (vo1.odprta != vo2.odprta) return vo1.odprta < vo2.odprta;
	if (vo1.cena != vo2.cena) return vo1.cena < vo2.cena;
	return vo1.lokacija < vo2.lokacija;
}

bool operator>(VozlisceD vo1, VozlisceD vo2) {
	if (vo1.odprta != vo2.odprta) return vo1.odprta > vo2.odprta;
	if (vo1.cena != vo2.cena) return vo1.cena > vo2.cena;
	return vo1.lokacija > vo2.lokacija;
}

bool operator==(VozlisceD vo, Tocka to) {
	return vo.lokacija == to;
}

bool operator==(Tocka to, VozlisceD vo) {
	return vo.lokacija == to;
}

std::ostream& operator<<(std::ostream& os, VozlisceD vo) {
	return os << vo.cena << ':' << vo.lokacija << '|' << vo.predhodnik << '|' << vo.odprta;
}


class RaceCondition {
	std::set<Tocka> seznamTock;
	std::set<VozlisceD> seznamVozlisc;
	std::vector<Tocka> optimalnaPot;

public:
	RaceCondition(std::set<Tocka> seznamTock) : 
		seznamTock(seznamTock), seznamVozlisc(seznamTock.begin(), seznamTock.end()), optimalnaPot({}) {}

	
	void najdiPot() {

	}
};


void preberiPodatke(std::string pot) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce najti.\n";
		return;
	}

	while (podatki.peek() != EOF) {

	}

	podatki.close();


}

/**/
int main() {


	return 0;
}
/**/
