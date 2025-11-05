/*
	t ~ 2:11 ur
	zelo lahko (bliznjica)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <bitset>


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


std::vector<Plosca> preberiPodatke(const std::string& pot) {

	std::vector<Plosca> resitev;


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
		resitev.push_back(Plosca(id));
		podatki.ignore(1);

		for (int i = 0; i < 10; i++) {
			podatki >> vrstica;
			resitev.back().push_back(vrstica);
		}
		podatki.ignore(2);
	}


	podatki.close();


	for (Plosca& plosca : resitev)
		plosca.dolociRobove();


	return resitev;
}


long long najdiRobove(std::vector<Plosca*> seznamKazalcev) {

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


void printSeznamPlosc(const std::vector<Plosca>& seznam) {
	for (const Plosca& plosca : seznam)
		plosca.print();
}
std::vector<Plosca*> narediSeznamKazalcev(std::vector<Plosca>& seznamPlosc) {
	std::vector<Plosca*> resitev;
	for (Plosca& plosca : seznamPlosc)
		resitev.push_back(&plosca);
	return resitev;
}

int main() {

	std::vector<Plosca> seznamPlosc = preberiPodatke("2020/20.txt");
	std::vector<Plosca*> seznamKazalcev = narediSeznamKazalcev(seznamPlosc);
	//printSeznamPlosc(seznamPlosc);


	long long resitev1 = najdiRobove(seznamKazalcev);
	std::cout << "Zmnozek ID robov je " << resitev1 << ".\n";



	return 0;
}
