#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>
#include <map>
#include <algorithm>


enum Smer {
	GOR = '^',
	DOL = 'v',
	LEVO = '<',
	DESNO = '>',
	ISTO = 1,
	NIC = -1
};


class Tocka {
public:
	short x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(short x, short y) : x(x), y(y) {}
};

std::map<char, Tocka> numPad;
std::map<char, Tocka> arrPad;




Smer enosmernaNumber(char kor1, char kor2) {

	if (kor1 == kor2) return ISTO;

	Tocka to1 = numPad[kor1];
	Tocka to2 = numPad[kor2];

	if (to1.x != to2.x && to1.y == to2.y) {
		if (to1.x < to2.x) return DESNO;
		return LEVO;
	}
	if (to1.x == to2.x && to1.y != to2.y) {
		if (to1.y < to2.y) return GOR;
		else return DOL;
	}

	return NIC;
}





std::list<std::string> preberiPodatke(std::string pot) {

	std::list<std::string> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	while (podatki >> vrstica)
		resitev.push_back(vrstica);

	podatki.close();


	return resitev;
}



int main() {

	numPad['7'] = Tocka(0, 0); numPad['8'] = Tocka(1, 0); numPad['9'] = Tocka(2, 0);
	numPad['4'] = Tocka(0, 1); numPad['5'] = Tocka(1, 1); numPad['6'] = Tocka(2, 1);
	numPad['1'] = Tocka(0, 2); numPad['2'] = Tocka(1, 2); numPad['3'] = Tocka(2, 2);
							   numPad['0'] = Tocka(1, 3); numPad['A'] = Tocka(2, 3);

							   arrPad['^'] = Tocka(1, 0); arrPad['A'] = Tocka(2, 0);
	arrPad['<'] = Tocka(1, 1); arrPad['v'] = Tocka(1, 1); arrPad['>'] = Tocka(2, 1);




	std::list<std::string> seznamKod(preberiPodatke("Advent24-21.txt"));



	

	std::string test11("5126418625");
	std::set<std::string::iterator> testSet;

	for (std::string::iterator it = test11.begin(); it != test11.end() - 1; it++) {
		if (*it % 2 == *(it + 1) % 2) testSet.insert(it);
	}


	for (std::string::iterator it : testSet)
		std::cout << *it << '-' << it - test11.begin() << '\n';



	return 0;
}
