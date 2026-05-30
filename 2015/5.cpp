/*
	t = 39:45 min
	lahko
	t = 9:17 min
	zelo lahko
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


bool pogoj11(std::string& podatek) {
	
	std::string samoglasniki = "aeiou";
	
	int ponovitev = 0;
	std::string::iterator it = podatek.begin();
	
	while (ponovitev < 3 && it != podatek.end()) {
		ponovitev += std::any_of(samoglasniki.begin(), samoglasniki.end(), [&](char c) {return c == *it; });
		it++;
	}

	return ponovitev == 3 ? true : false;
}
bool pogoj12(std::string& podatek) {

	if (podatek.size() < 2) return false;

	for (int i = 0; i < podatek.size() - 1; i++)
		if (podatek[i] == podatek[i + 1]) return true;

	return false;
}
bool pogoj13(std::string& podatek) {
	
	std::vector<std::string> seznamNezeljenih = { "ab","cd","pq","xy" };

	for (int i = 0; i < podatek.size() - 1; i++)
		if (std::any_of(seznamNezeljenih.begin(), seznamNezeljenih.end(), [&](std::string str) {return str == podatek.substr(i, 2); })) return false;

	return true;
}

bool pogojiVsi1(std::string& podatek) {

	//std::cout << podatek << " : " << pogoj11(podatek) << "|" << pogoj12(podatek) << "|" << pogoj13(podatek) << '\n';
	return pogoj11(podatek) && pogoj12(podatek) && pogoj13(podatek);
}

int steviloDobrih1(std::vector<std::string>& podatki) {

	int resitev = 0;

	for (std::string& podatek : podatki)
		resitev += pogojiVsi1(podatek);

	return resitev;
}


bool pogoj21(std::string& podatek) {

	for (int i = 0; i < podatek.size() - 1; i++)
		for (int j = i + 2; j < podatek.size() - 1; j++)
			if (podatek.substr(i, 2) == podatek.substr(j, 2)) return true;

	return false;
}
bool pogoj22(std::string& podatek) {

	for (int i = 0; i < podatek.size() - 2; i++)
		if (podatek[i] == podatek[i + 2]) return true;

	return false;
}

bool pogojiVsi2(std::string& podatek) {

	//std::cout << podatek << " : " << pogoj21(podatek) << "|" << pogoj22(podatek) << '\n';
	return pogoj21(podatek) && pogoj22(podatek);
}

int steviloDobrih2(std::vector<std::string>& podatki) {

	int resitev = 0;

	for (std::string& podatek : podatki)
		resitev += pogojiVsi2(podatek);

	return resitev;
}


int main() {

	std::vector<std::string> podatki(preberiPodatke("2015/5.txt"));


	int resitev1 = steviloDobrih1(podatki);
	std::cout << "Stevilo dobrih po prvih pogojih je " << resitev1 << ".\n";

	int resitev2 = steviloDobrih2(podatki);
	std::cout << "Stevilo dobrih po drugih pogojih je " << resitev2 << ".\n";


	return 0;
}
