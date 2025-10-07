/*
	t = 27:15 min
	zelo lahko
	t = +124:50 min
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::pair<int, std::vector<std::pair<int, int>>> preberiPodatke(const std::string& pot) {

	int resitev1;
	std::vector<std::pair<int,int>> resitev2;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev1, resitev2 };
	}

	std::string vmesnik;

	podatki >> resitev1;

	int index = 0;
	while (std::getline(podatki, vmesnik, ',')) {
		if (vmesnik != "x")
			resitev2.push_back({ std::stoi(vmesnik), index });
		index++;
	}

	podatki.close();

	return { resitev1,resitev2 };
}


int najdiNajhitrejsiPrevoz(const int& cas, const std::vector<std::pair<int, int>>& vozniRed) {

	int id = 0;
	int min = 2 * cas;

	for (const std::pair<int, int>& bus : vozniRed) {

		if (cas % bus.first == 0)
			return 0;

		int stObhodov = cas / bus.first;
		int naslednjiObhod = bus.first * (stObhodov + 1);

		if (naslednjiObhod < min) {
			min = naslednjiObhod;
			id = bus.first;
		}
	}

	return (min - cas) * id;
}


int najdiIstiKorak(std::vector<std::vector<int>> vzorec) {

	int max = 0;
	int id = 0;

	for (int i = 0; i < vzorec.size(); i++) {
		if (vzorec[i][0] == 1)
			id = i;
		if (vzorec[i][1] > max)
			max = vzorec[i][1];
	}

	//vzorec.erase(vzorec.begin() + id);

	std::vector<int> trenutnoStanje(vzorec.size());

	while (true) {
		
		bool koncnik = true;
		
		for (int i = 0; i < trenutnoStanje.size(); i++) {
			while (trenutnoStanje[i] < max) {
				trenutnoStanje[i] += (vzorec[i][1] - vzorec[i][0]);
			}
			if (trenutnoStanje[i] > max)
				max = trenutnoStanje[i];
		}

		const int& prva = trenutnoStanje.front();
		std::cout << trenutnoStanje[0] << '\t';
		for (int i = 1; i < trenutnoStanje.size(); i++) {
			std::cout << trenutnoStanje[i] << '\t';
			if (prva != trenutnoStanje[i])
				koncnik = false;
		}
		std::cout << '\n';

		if (koncnik)
			break;
	}

	return trenutnoStanje.front();
}
int najdiPrvoUreditev(const std::vector<std::pair<int, int>>& vozniRed) {

	std::vector<int> urejen;
	std::vector<int> razdelitev;
	std::pair<int, int> max = { 0,0 };

	for (const std::pair<int, int>& bus : vozniRed) {
		
		urejen.push_back(bus.first);
		razdelitev.push_back(bus.second);

		if (bus.first > max.first)
			max = bus;

		//std::cout << bus.first << ' ' << bus.second << '\n';
	}
	for (int& i : razdelitev)
		i -= max.second;

	std::vector<std::vector<int>> najdeneNicle(urejen.size());

	int faktor = 1;
	while (true) {
		for (int i = 0; i < razdelitev.size(); i++) {

			urejen[i] = max.first * faktor + razdelitev[i];

			if (urejen[i] % vozniRed[i].first == 0)
				najdeneNicle[i].push_back(faktor);

			//std::cout << urejen[i] << ":\t" << vozniRed[i].first << "\t" << urejen[i] % vozniRed[i].first << '\n';
		}
		faktor++;

		bool koncnik = true;

		for (const std::vector<int>& nicle : najdeneNicle)
			if (nicle.size() < 2) {
				koncnik = false;
				break;
			}
		
		if (koncnik)
			break;
	}

	for (int i = 0; i < najdeneNicle.size(); i++) {
		najdeneNicle[i].resize(2);
		for (int j = 0; j < najdeneNicle[i].size(); j++)
			std::cout << najdeneNicle[i][j] << '\t';
		std::cout << '\n';
	}
	

	return max.first * najdiIstiKorak(najdeneNicle) - max.second;
}

int najdiPrvoUreditev2(const std::vector<std::pair<int, int>>& vozniRed) {

	return -1;
}


int main() {

	std::pair<int, std::vector<std::pair<int,int>>> podatki(preberiPodatke("2020/13.txt"));
	int& cas = podatki.first;
	std::vector<std::pair<int, int>>& vozniRed = podatki.second;

	int resitev1 = najdiNajhitrejsiPrevoz(cas, vozniRed);
	std::cout << "Ocena idealnega prevoza je " << resitev1 << ".\n";

	int resitev2 = najdiPrvoUreditev2(vozniRed);
	std::cout << "Prva ureditev voznega je na casu " << resitev2 << ".\n";

	return 0;
}
