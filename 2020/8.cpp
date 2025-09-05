/*
	t = 36:55 min
	zelo lahko
	t = 24:42 min
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


enum Ukaz {
	ACC,
	NOP,
	JMP
};

struct Navodilo {

	Ukaz ukaz;
	int vrednost;

	Navodilo(std::string ukazStr, int vrednost) : ukaz(pretvoriUkaz(ukazStr)), vrednost(vrednost) {}

	Ukaz pretvoriUkaz(const std::string& ukazStr) const {
		if (ukazStr == "acc") return ACC;
		if (ukazStr == "nop") return NOP;
		if (ukazStr == "jmp") return JMP;
	}

	std::string pretvoriUkazStr() const {
		if (ukaz == ACC) return "acc";
		if (ukaz == NOP) return "nop";
		if (ukaz == JMP) return "jmp";
	}
};


std::vector<Navodilo> preberiPodatke(const std::string& pot) {

	std::vector<Navodilo> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string ukazStr;
	int vrednost;

	while (podatki.peek() != EOF) {

		podatki >> ukazStr >> vrednost;
		resitev.push_back(Navodilo(ukazStr, vrednost));
	}

	podatki.close();

	return resitev;
}


int najdiZanko(const std::vector<Navodilo>& seznamNavodil) {

	int resitev = 0;

	int index = 0;
	std::set<int> seznamIndexov;

	while (true) {

		if (seznamIndexov.find(index) != seznamIndexov.end())
			return resitev;

		seznamIndexov.insert(index);


		const Navodilo& navodilo = seznamNavodil[index];

		if (navodilo.ukaz == ACC) {
			resitev += navodilo.vrednost;
			index++;
			continue;
		}
		if (navodilo.ukaz == NOP) {
			index++;
			continue;
		}
		if (navodilo.ukaz == JMP) {
			index += navodilo.vrednost;
			continue;
		}
	}

	return resitev;
}


int najdiNapako(const std::vector<Navodilo>& seznamNavodil) {

	int resitev = 0;

	std::vector<int> seznamPotencialnihNapak;
	for (int i = 0; i < seznamNavodil.size(); i++)
		if (seznamNavodil[i].ukaz == NOP || seznamNavodil[i].ukaz == JMP)
			seznamPotencialnihNapak.push_back(i);

	int index = 0;
	std::set<int> seznamIndexov;

	for (const int& napaka : seznamPotencialnihNapak) {

		resitev = 0;
		index = 0;
		seznamIndexov.clear();

		while (index < seznamNavodil.size()) {

			if (seznamIndexov.find(index) != seznamIndexov.end())
				break;

			seznamIndexov.insert(index);

			const Navodilo& navodilo = seznamNavodil[index];
			//std::cout << navodilo.pretvoriUkazStr() << ' ' << navodilo.vrednost << '\n';

			if (navodilo.ukaz == ACC) {
				resitev += navodilo.vrednost;
				index++;
				continue;
			}
			if (navodilo.ukaz == NOP) {
				if (napaka == index)
					goto NAPAKA_JMP;
			NAPAKA_NOP:
				index++;
				continue;
			}
			if (navodilo.ukaz == JMP) {
				if (napaka == index)
					goto NAPAKA_NOP;
			NAPAKA_JMP:
				index += navodilo.vrednost;
				continue;
			}
		}
		//std::cout << '\n';

		if (index == seznamNavodil.size())
			return resitev;
	}

	return resitev;
}


int main() {

	std::vector<Navodilo> bazaPodatkov = preberiPodatke("2020/8.txt");


	int resitev1 = najdiZanko(bazaPodatkov);

	std::cout << "Vrednost za zanko je bila " << resitev1 << ".\n";


	int resitev2 = najdiNapako(bazaPodatkov);

	std::cout << "Vrednost po popravljeni napaki je " << resitev2 << ".\n";


	return 0;
}
