/*
	t = 39:45 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


std::vector<std::string> preberiPodatke(const std::string& pot) {

	std::vector<std::string> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki.peek() != EOF) {
		podatki >> vrstica;
		resitev.push_back(vrstica);
	}

	podatki.close();

	return resitev;
}


bool pogoj1(std::string& podatek) {
	
	std::string samoglasniki = "aeiou";
	
	int ponovitev = 0;
	std::string::iterator it = podatek.begin();
	
	while (ponovitev < 3 && it != podatek.end()) {
		ponovitev += std::any_of(samoglasniki.begin(), samoglasniki.end(), [&](char c) {return c == *it; });
		it++;
	}

	return ponovitev == 3 ? true : false;
}
bool pogoj2(std::string& podatek) {

	if (podatek.size() < 2) return false;

	for (int i = 0; i < podatek.size() - 1; i++)
		if (podatek[i] == podatek[i + 1]) return true;

	return false;
}
bool pogoj3(std::string& podatek) {
	
	std::vector<std::string> seznamNezeljenih = { "ab","cd","pq","xy" };

	for (int i = 0; i < podatek.size() - 1; i++)
		if (std::any_of(seznamNezeljenih.begin(), seznamNezeljenih.end(), [&](std::string str) {return str == podatek.substr(i, 2); })) return false;

	return true;
}

bool pogojiVsi(std::string& podatek) {

	//std::cout << podatek << " : " << pogoj1(podatek) << "|" << pogoj2(podatek) << "|" << pogoj3(podatek) << '\n';
	return pogoj1(podatek) && pogoj2(podatek) && pogoj3(podatek);
}

int steviloDobrih(std::vector<std::string>& podatki) {

	int resitev = 0;

	for (std::string& podatek : podatki)
		resitev += pogojiVsi(podatek);

	return resitev;
}


int main() {

	std::vector<std::string> podatki(preberiPodatke("2015/5.txt"));


	int resitev1 = steviloDobrih(podatki);
	std::cout << "Stevilo dobrih je " << resitev1 << ".\n";


	return 0;
}
