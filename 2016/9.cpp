/*
	t = 15:42 min
	zelo lahko
	t = 2:06:14 ur
	srednja tezavnost
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


bool dekompresiraj(std::string& resitev) {

	bool zamenjava = false;

	std::stringstream ss(resitev);

	resitev.clear();

	char c;
	int vrednost1, vrednost2;

	while (ss.peek() != EOF) {
		ss >> c;

		if (c == '(') {
			zamenjava = true;

			ss >> vrednost1;
			ss.ignore(1);
			ss >> vrednost2;
			ss.ignore(1);

			std::string vmesna;

			for (int i = 0; i < vrednost1; i++) {
				ss >> c;
				vmesna.push_back(c);
			}
			for (int i = 0; i < vrednost2; i++) {
				resitev += vmesna;
			}
			continue;
		}

		resitev.push_back(c);
	}

	return zamenjava;
}

int preberiPodatke1(const std::string& pot) {

	std::string resitev = "";

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return -1;
	}

	podatki >> resitev;

	podatki.close();

	dekompresiraj(resitev);
	int resitev1 = resitev.size();
	//std::cout << "resitev: " << resitev << '\n';

	return resitev1;
}


void printVecPar(const std::vector<std::pair<char, long long>>& seznam) {
	for (const std::pair<char, long long>& par : seznam) {
		std::cout << par.first << ' ' << par.second << '\n';
	}
}

struct Veckratnik {
	int zivljenskaDoba, mnozitelj;
};

void prehod(std::vector<std::pair<char, unsigned long long>>& seznam) {
	
	std::vector<Veckratnik> seznamVeckratnikov;

	for (int i = 0; i < seznam.size(); i++) {
		std::pair<char, unsigned long long>& par = seznam[i];

		if (par.first == '(') {

			int j = find(seznam.begin() + i, seznam.end(), std::pair<char, unsigned long long>{ ')', 0ull }) - seznam.begin();

			int dolzina = j - i + 1;
			for (Veckratnik& veckratnik : seznamVeckratnikov)
				veckratnik.zivljenskaDoba -= dolzina;

			auto itRem = std::remove_if(seznamVeckratnikov.begin(), seznamVeckratnikov.end(), [](const Veckratnik& vec) {
				return vec.zivljenskaDoba <= 0; 
				});
			seznamVeckratnikov.erase(itRem, seznamVeckratnikov.end());


			seznamVeckratnikov.push_back(Veckratnik());
			
			std::string vmesniStr;
			for (auto it = seznam.begin() + i + 1; it != seznam.begin() + j; it++)
				vmesniStr.push_back(it->first);
			
			std::stringstream ss(vmesniStr);
			ss >> seznamVeckratnikov.back().zivljenskaDoba;
			ss.ignore(1);
			ss >> seznamVeckratnikov.back().mnozitelj;

			i = j;
			continue;
		}


		for (Veckratnik& veckratnik : seznamVeckratnikov) {
			par.second *= veckratnik.mnozitelj;
			veckratnik.zivljenskaDoba--;
		}

		auto itRem = std::remove_if(seznamVeckratnikov.begin(), seznamVeckratnikov.end(), [](const Veckratnik& vec) {
			return vec.zivljenskaDoba <= 0;
			});
		seznamVeckratnikov.erase(itRem, seznamVeckratnikov.end());
	}
}

unsigned long long preberiPodatke2(const std::string& pot) {

	unsigned long long resitev = 0;

	std::vector<std::pair<char, unsigned long long>> zapis;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	char c;
	bool oklepaj = false;

	while (podatki.peek() != EOF) {
		
		podatki >> c;
		
		if (c == '(') {
			oklepaj = true;
			zapis.push_back({ c,!oklepaj });
			continue;
		}
		if (c == ')') {
			zapis.push_back({ c,!oklepaj });
			oklepaj = false;
			continue;
		}
		zapis.push_back({ c,!oklepaj });
	}

	podatki.close();

	prehod(zapis);

	for (const std::pair<char, unsigned long long>& par : zapis) {
		resitev += par.second;
	}

	return resitev;
}


int main() {

	int resitev1 = preberiPodatke1("2016/9.txt");
	std::cout << "Dolzina dekompresirane datoteke je " << resitev1 << ".\n";

	unsigned long long resitev2 = preberiPodatke2("2016/9.txt");
	std::cout << "Dolzina dekompresirane^2 datoteke je " << resitev2 << ".\n";

	return 0;
}
