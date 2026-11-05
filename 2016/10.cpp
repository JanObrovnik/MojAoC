/*
	t = 1:40:43 ur
	nekoliko lahko
	t = 14:57 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class Robot {
	std::vector<int> chip;

public:
	Robot() : chip({}) {}

	void dodaj(const int& chip) {
		this->chip.push_back(chip);
	}
	int velikost() const {
		return chip.size();
	}
	const std::vector<int>& celotenSeznam() const {
		return chip;
	}

	int max() const {
		return std::max(chip.front(), chip.back());
	}
	int min() const {
		return std::min(chip.front(), chip.back());
	}
	void pocisti() {
		chip.clear();
	}

	bool imaIskana(const int& chip1, const int& chip2) {
		if (chip.size() != 2)
			return false;
		return (chip1 == chip.front() || chip1 == chip.back()) && (chip2 == chip.front() || chip2 == chip.back());
	}
};

struct Zacetek {
	int chip, robot;
};

struct Zamenjava {
	int robotID, manjID, vecID;
};
bool operator==(const Zamenjava& za1, const Zamenjava& za2) {
	return za1.robotID == za2.robotID;
}
bool operator<(const Zamenjava& za1, const Zamenjava& za2) {
	return za1.robotID < za2.robotID;
}
bool operator>(const Zamenjava& za1, const Zamenjava& za2) {
	return za1.robotID > za2.robotID;
}


void preberiPodatke(const std::string& pot, std::vector<Zacetek>& seznamZacetkov, std::vector<Zamenjava>& seznamZamenjav) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	std::string ukaz;

	while (podatki.peek() != EOF) {

		podatki >> ukaz;

		if (ukaz == "value") {
			
			seznamZacetkov.push_back(Zacetek());
			podatki >> seznamZacetkov.back().chip;
			podatki.ignore(13);
			podatki >> seznamZacetkov.back().robot;
		}
		else if (ukaz == "bot") {
			
			seznamZamenjav.push_back(Zamenjava());
			
			podatki >> seznamZamenjav.back().robotID;
			
			podatki.ignore(14);
			podatki >> ukaz;
			podatki >> seznamZamenjav.back().manjID;
			if (ukaz == "output") { seznamZamenjav.back().manjID *= -1; seznamZamenjav.back().manjID--; }
			
			podatki.ignore(13);
			podatki >> ukaz;
			podatki >> seznamZamenjav.back().vecID;
			if (ukaz == "output") { seznamZamenjav.back().vecID *= -1; seznamZamenjav.back().vecID--; }
		}
	}

	podatki.close();

	std::sort(seznamZamenjav.begin(), seznamZamenjav.end());

	//std::cout << "Zacetek:\n";
	//for (const Zacetek& zac : seznamZacetkov) {
	//	std::cout << "chip " << zac.chip << "  \tgre robotu " << zac.robot << ".\n";
	//} std::cout << "size: " << seznamZacetkov.size() << '\n' << '\n';
	//
	//std::cout << "Zamenjava:\n";
	//for (const Zamenjava& zam : seznamZamenjav) {
	//	std::cout << "robot " << zam.robotID << " \tda manj " << zam.manjID << "\tda vec " << zam.vecID << ".\n";
	//} std::cout << "size: " << seznamZamenjav.size() << '\n' << '\n';

	return;
}


std::pair<int, int> najdiIskanaChipa(const std::vector<Zacetek>& seznamZacetkov, const std::vector<Zamenjava>& seznamZamenjav, const int& chip1, const int& chip2) {

	int resitev1 = -1;
	int resitev2 = 1;

	std::vector<std::pair<int, int>> seznamOutputov;


	std::vector<Robot> seznamRobotov(seznamZamenjav.size());

	for (const Zacetek& zacetek : seznamZacetkov)
		seznamRobotov[zacetek.robot].dodaj(zacetek.chip);

	int steviloChipov = seznamZacetkov.size();


	while (steviloChipov > 0) {

		//for (int i = 0; i < seznamRobotov.size(); i++) {
		//	std::cout << "Robot " << i << ":\t";
		//	for (const int& i : seznamRobotov[i].celotenSeznam())
		//		std::cout << i << ' ';
		//	std::cout << '\n';
		//} std::cout << '\n';

		int ID = -1;
		for (int i = 0; i < seznamRobotov.size(); i++)
			if (seznamRobotov[i].velikost() == 2) {
				ID = i;
				break;
			}

		if (seznamRobotov[ID].imaIskana(chip1, chip2))
			resitev1 = ID;


		const int& manjID = seznamZamenjav[ID].manjID;
		const int& vecID = seznamZamenjav[ID].vecID;

		if (manjID >= 0)
			seznamRobotov[manjID].dodaj(seznamRobotov[ID].min());
		else { seznamOutputov.push_back({ (-manjID) - 1,seznamRobotov[ID].min() }); steviloChipov--; }

		if (vecID >= 0)
			seznamRobotov[vecID].dodaj(seznamRobotov[ID].max());
		else { seznamOutputov.push_back({ (-vecID) - 1,seznamRobotov[ID].max() }); steviloChipov--; }

		seznamRobotov[ID].pocisti();
	}


	for (const std::pair<int, int>& par : seznamOutputov) {
		if (par.first >= 0 && par.first <= 2)
			resitev2 *= par.second;
		//std::cout << "Output: " << par.first << " \tChip: " << par.second << '\n';
	}


	return { resitev1,resitev2 };
}


int main() {

	std::vector<Zacetek> seznamZacetkov;
	std::vector<Zamenjava> seznamZamenjav;

	preberiPodatke("2016/10.txt", seznamZacetkov, seznamZamenjav);

	
	std::pair<int, int> resitev = najdiIskanaChipa(seznamZacetkov, seznamZamenjav, 61, 17);

	std::cout << "Robot " << resitev.first << " ima iskane chipe.\n";
	std::cout << "Zmnozek chipov je " << resitev.second << ".\n";


	return 0;
}
