/*
	t ~ 1:09 ur
	srednja tezavnost
	t = 13:24 min
	enostavno
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


void preberiPodatke(const std::string& pot, std::map<std::pair<std::string, std::string>, int>& povezave, std::vector<std::string>& seznamLokacij) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	std::string lokacija1, lokacija2;
	int razdalja;
	std::string praznina;

	while (podatki.peek() != EOF) {
		
		podatki >> lokacija1 >> praznina >> lokacija2 >> praznina >> razdalja;
		
		povezave[{lokacija1, lokacija2}] = razdalja;
		povezave[{lokacija2, lokacija1}] = razdalja;
		
		if (std::find(seznamLokacij.begin(), seznamLokacij.end(), lokacija1) == seznamLokacij.end()) seznamLokacij.push_back(lokacija1);
		if (std::find(seznamLokacij.begin(), seznamLokacij.end(), lokacija2) == seznamLokacij.end()) seznamLokacij.push_back(lokacija2);
	}

	podatki.close();

	return;
}


unsigned int izracunajRazdaljo(const std::map<std::pair<std::string, std::string>, int>& povezave, const std::vector<std::string>& seznamObiskanihLokacij) {

	unsigned int resitev = 0;

	std::cout << seznamObiskanihLokacij.front();
	
	for (int i = 1; i < seznamObiskanihLokacij.size(); i++) {

		std::cout << " -> " << seznamObiskanihLokacij[i];
		resitev += povezave.at({ seznamObiskanihLokacij[i - 1], seznamObiskanihLokacij[i] });
	}

	std::cout << " | " << resitev << '\n';

	return resitev;
}

void najdiNajkrajsoPot(const std::map<std::pair<std::string, std::string>, int>& povezave, const std::vector<std::string>& seznamLokacij, unsigned int& min, unsigned int& max, std::vector<std::string> seznamObiskanihLokacij = {}, int globina = 0) {

	if (seznamLokacij.size() == seznamObiskanihLokacij.size()) {
		
		unsigned int novaResitev = izracunajRazdaljo(povezave, seznamObiskanihLokacij);
		
		if (novaResitev < min)
			min = novaResitev;
		if (novaResitev > max)
			max = novaResitev;

		return;
	}

	for (const std::string& lokacija : seznamLokacij) {

		if (std::find(seznamObiskanihLokacij.begin(), seznamObiskanihLokacij.end(), lokacija) != seznamObiskanihLokacij.end())
			continue;
		
		seznamObiskanihLokacij.push_back(lokacija);

		najdiNajkrajsoPot(povezave, seznamLokacij, min, max, seznamObiskanihLokacij, globina + 1);

		seznamObiskanihLokacij.pop_back();
	}

	return;
}


int main() {

	std::map<std::pair<std::string, std::string>, int> povezave;
	std::vector<std::string> seznamLokacij;

	preberiPodatke("2015/9.txt", povezave, seznamLokacij);


	unsigned int resitev1 = -1;
	unsigned int resitev2 = 0;
	najdiNajkrajsoPot(povezave, seznamLokacij, resitev1, resitev2);
	
	std::cout << std::endl;
	std::cout << "Najkrajsa pot je dolga " << resitev1 << ".\n";
	std::cout << "Najdalsa pot je dolga " << resitev2 << ".\n";


	return 0;
}
