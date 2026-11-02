/*
	t = 45:44 min
	lahko
	t = 21:00 min
	precej lahko (malo cudna)
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


struct Soba {

	std::map<char, int> seznamImen;
	int ID;
	std::string checksum;
	std::string vrstica;

	Soba() : seznamImen({}), ID(-1), checksum("") {}
	Soba(std::string vrstica) {

		this->vrstica = vrstica;

		size_t konec = vrstica.find_last_of('-');

		for (int i = 0; i < konec; i++) {
			const char& c = vrstica[i];

			if (c == '-')
				continue;

			seznamImen[c]++;
		}

		ID = std::stoi(vrstica.substr(konec + 1, vrstica.size()));

		size_t oklepaj1 = vrstica.find_last_of('[');
		size_t oklepaj2 = vrstica.find_last_of(']');

		checksum = vrstica.substr(oklepaj1 + 1, oklepaj2 - oklepaj1 - 1);
	}

	void print() const {

		for (auto it = seznamImen.begin(); it != seznamImen.end(); it++)
			std::cout << it->first << ": " << it->second << '\n';

		std::cout << "ID: " << ID << '\n';

		std::cout << "CheckSum: " << checksum << '\n' << '\n';
	}
};


std::vector<Soba> preberiPodatke(const std::string& pot) {

	std::vector<Soba> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki.peek() != EOF) {

		podatki >> vrstica;

		resitev.push_back(Soba(vrstica));
		//resitev.back().print();
	}

	podatki.close();

	return resitev;
}


int preveriResnicnostSobe(const Soba& soba) {

	std::vector<std::pair<char, int>> seznamImen(soba.seznamImen.begin(), soba.seznamImen.end());

	std::sort(seznamImen.begin(), seznamImen.end(), [](std::pair<char, int> par1, std::pair<char, int> par2) {
		return par1.second > par2.second;
		});
	//for (auto par : seznamImen) std::cout << par.first << ": " << par.second << '\n'; std::cout << '\n';

	for (const char& c : soba.checksum) {

		int max = seznamImen.front().second;

		std::vector<std::pair<char, int>>::iterator it;

		for (auto itPar = seznamImen.begin(); itPar != seznamImen.end(); itPar++) {
			const std::pair<char, int>& par = *itPar;

			if (par.second != max) {
				return 0;
			}
			if (par.first == c) {
				it = itPar;
				break;
			}
		}

		seznamImen.erase(it);
	}

	return soba.ID;
}

std::string caesarCipher(const Soba& soba) {

	size_t konec = soba.vrstica.find_last_of('-');
	std::string resitev(soba.vrstica.begin(), soba.vrstica.begin() + konec);

	int velikostAbecede = 'z' - 'a' + 1;
	int zamik = soba.ID % velikostAbecede;

	for (char& c : resitev) {

		if (c == '-') {
			c = ' ';
			continue;
		}
		
		if (int(c) + zamik > 'z')
			c -= velikostAbecede;

		c += zamik;
	}
	//std::cout << soba.ID << ": " << resitev << '\n';

	return resitev;
}

std::pair<int, int> preveriVseSobe(const std::vector<Soba>& seznamSob, const std::string& iskanaSoba) {

	int resitev1 = 0;
	int resitev2 = 0;

	for (const Soba& soba : seznamSob) {

		const int nova = preveriResnicnostSobe(soba);
		
		resitev1 += nova;

		if (nova != 0)
			if (caesarCipher(soba) == iskanaSoba)
				resitev2 = soba.ID;
	}

	return { resitev1,resitev2 };
}


int main() {

	std::vector<Soba> seznamSob = preberiPodatke("2016/4.txt");
	std::string iskanaSoba = "northpole object storage";


	std::pair<int, int> resitev = preveriVseSobe(seznamSob, iskanaSoba);
	
	std::cout << "Vsota vseh ID-jev obstajajocih sob je " << resitev.first << ".\n";
	std::cout << "ID sobe \"" << iskanaSoba << "\" je " << resitev.second << ".\n";
	

	return 0;
}
