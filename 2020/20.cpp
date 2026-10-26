/*
	t ~ 2:11 ur
	zelo lahko (bliznjica)
	t + 25 + 26
*/

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>


class Tocka {
public:
	short x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(short x, short y) : x(x), y(y) {}
};


enum SMER {
	SEVER,
	VZHOD,
	JUG,
	ZAHOD,
	NEVELJAVNO
};

std::bitset<10> zrcali(const std::bitset<10> vhod) {
	
	std::bitset<10> resitev;

	for (int i = 0; i < 10; i++) {
		resitev[i] = vhod[9 - i];
	}

	return resitev;
}

class Plosca {

	short id;
	std::vector<std::string> osnovniPodatek;
	std::map<SMER, std::bitset<10>> seznamRobov;

public:

	Plosca() : id(-1), osnovniPodatek({}), seznamRobov({}) {}
	Plosca(short id) : id(id), osnovniPodatek({}), seznamRobov({}) {}

	void push_back(const std::string& podatek) { osnovniPodatek.push_back(podatek); }

	void dolociRobove() {

		std::bitset<10> robSever;
		std::bitset<10> robVzhod;
		std::bitset<10> robJug;
		std::bitset<10> robZahod;

		for (int i = 0; i < 10; i++) {
			robSever[i] = (osnovniPodatek[0][i] == '#');
			robVzhod[i] = (osnovniPodatek[i][9] == '#');
			robJug[i]   = (osnovniPodatek[9][i] == '#');
			robZahod[i] = (osnovniPodatek[i][0] == '#');
		}

		seznamRobov[SEVER] = robSever;
		seznamRobov[VZHOD] = robVzhod;
		seznamRobov[JUG]   = robJug;
		seznamRobov[ZAHOD] = robZahod;
	}


	short dobiID() const { return id; }
	std::bitset<10> dobiRob(const SMER& smer) const { return seznamRobov.at(smer); }


	std::pair<SMER,bool> seUjema(const std::bitset<10>& iskanRob) const {

		for (auto it = seznamRobov.cbegin(); it != seznamRobov.cend(); it++) {
			if (it->second == iskanRob) std::cout << " - " << id << ": " << it->first << " false\n";
			if (it->second == zrcali(iskanRob)) std::cout << " - " << id << ": " << it->first << " true\n";
		}
		return { NEVELJAVNO,false };
	}

	bool steviloUjemanj(const std::bitset<10>& iskanRob) const {

		short resitev = 0;

		for (auto it = seznamRobov.cbegin(); it != seznamRobov.cend(); it++)
			if ((it->second == iskanRob) || (it->second == zrcali(iskanRob)))
				return true;

		return false;
	}


	void print(const bool& izpisID = true) const {
		if (izpisID)
			std::cout << "Plosca: " << id << '\n';
		for (const std::string& vrstica : osnovniPodatek)
			std::cout << vrstica << '\n';
		std::cout << '\n';
	}
};


std::map<short, Plosca> preberiPodatke(const std::string& pot) {

	std::map<short, Plosca> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}


	short id;
	std::string vrstica;

	while (podatki.peek() != EOF) {

		podatki.ignore(5);

		podatki >> id;
		Plosca novaPlosca(id);

		podatki.ignore(1);
		for (int i = 0; i < 10; i++) {
			podatki >> vrstica;
			novaPlosca.push_back(vrstica);
		}
		podatki.ignore(2);

		novaPlosca.dolociRobove();

		resitev[id] = novaPlosca;
	}


	podatki.close();


	return resitev;
}


long long najdiZmnozekRobov(std::vector<Plosca*> seznamKazalcev) {

	long long resitev = 1;

	for (int j = 0; j < seznamKazalcev.size(); j++) {
		
		int steviloUjemanj = 0;

		for (int i = 0; i < seznamKazalcev.size(); i++) {

			if (i == j) continue;

			steviloUjemanj += seznamKazalcev[i]->steviloUjemanj(seznamKazalcev[j]->dobiRob(SEVER));
			steviloUjemanj += seznamKazalcev[i]->steviloUjemanj(seznamKazalcev[j]->dobiRob(VZHOD));
			steviloUjemanj += seznamKazalcev[i]->steviloUjemanj(seznamKazalcev[j]->dobiRob(JUG));
			steviloUjemanj += seznamKazalcev[i]->steviloUjemanj(seznamKazalcev[j]->dobiRob(ZAHOD));
		}

		if (steviloUjemanj == 2)
			resitev *= (long long)seznamKazalcev[j]->dobiID();
	}

	return resitev;
}


std::map<short, std::vector<short>> najdiPovezavoPlosc(std::vector<Plosca*> seznamKazalcev) {

	std::map<short, std::vector<short>> resitev;

	for (int j = 0; j < seznamKazalcev.size() - 1; j++) {
		for (int i = j + 1; i < seznamKazalcev.size(); i++) {

			if (seznamKazalcev[j]->steviloUjemanj(seznamKazalcev[i]->dobiRob(SEVER))) { ///////////// ali vse ostale
				resitev[seznamKazalcev[j]->dobiID()].push_back(seznamKazalcev[i]->dobiID());
				resitev[seznamKazalcev[i]->dobiID()].push_back(seznamKazalcev[j]->dobiID());
			}
		}
	}

	return resitev;
}


void printSeznamPlosc(const std::vector<Plosca>& seznam) {
	for (const Plosca& plosca : seznam)
		plosca.print();
}
std::vector<Plosca*> narediSeznamKazalcev(std::map<short, Plosca>& seznamPlosc) {
	std::vector<Plosca*> resitev;
	//for (Plosca& plosca : seznamPlosc)
	//	resitev.push_back(&plosca);
	for (auto it = seznamPlosc.begin(); it != seznamPlosc.end(); it++)
		resitev.push_back(&(it->second));
	return resitev;
}

int main() {

	std::map<short, Plosca> seznamPlosc = preberiPodatke("2020/20.txt");
	std::vector<Plosca*> seznamKazalcev = narediSeznamKazalcev(seznamPlosc);
	//printSeznamPlosc(seznamPlosc);


	long long resitev1 = najdiZmnozekRobov(seznamKazalcev);
	std::cout << "Zmnozek ID robov je " << resitev1 << ".\n";


	std::map<short, std::vector<short>> testMap = najdiPovezavoPlosc(seznamKazalcev);
	std::cout << testMap.size() << '\n';


	return 0;
}
