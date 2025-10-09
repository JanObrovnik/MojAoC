/*
	t ~ 1 h
	lahko
	t = 1:13:19 h
	relativno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <map>


std::bitset<36> maskiranje(const int& vrednost, const std::string& maska) {

	std::bitset<36> resitev(vrednost);

	int index = 0;

	for (auto it = maska.rbegin(); it != maska.rend(); it++) {
		if (*it != 'X')
			resitev.set(index, *it - '0');
		index++;
	}

	return resitev;
}

std::map<int, std::bitset<36>> preberiPodatke(const std::string& pot) {

	std::map<int, std::bitset<36>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string prepoznava;
	std::string maska;
	int mesto;
	int vrednost;

	while (podatki.peek() != EOF) {

		podatki >> prepoznava;

		if (prepoznava == "mask") {
			podatki.ignore(2);
			podatki >> maska;
		}
		else {
			mesto = std::stoi(prepoznava.substr(4, prepoznava.size() - 5));
			podatki.ignore(2);
			podatki >> vrednost;

			resitev[mesto] = maskiranje(vrednost, maska);
		}
	}

	return resitev;
}

unsigned long long vsota(const std::map<int, std::bitset<36>>& podatki) {

	unsigned long long resitev = 0;

	for (const auto& i : podatki)
		resitev += i.second.to_ullong();

	return resitev;
}


long long razberiMesto(std::string mestoBit, const int& moznost) {

	const std::bitset<36> moznostBit(moznost);

	int index = 0;
	for (auto it = mestoBit.rbegin(); it != mestoBit.rend(); it++) {
		if (*it == 'X') {
			*it = moznostBit[index] + '0';
			index++;
		}
	}

	return std::stoll(mestoBit, NULL, 2);
}
void zapisiVSpomin(std::map<long long, long long>& spomin, 
				   const std::string& maska, const long long& mesto, const long long& vrednost) {

	std::string mestoBit = std::bitset<36>(mesto).to_string();

	int index = 0;
	for (const char& c : maska) {
		if (c != '0')
			mestoBit[index] = c;
		index++;
	}

	const int& velikost = std::count(mestoBit.begin(), mestoBit.end(), 'X');
	int steviloMoznosti = pow(2, velikost);

	for (int i = 0; i < steviloMoznosti; i++) {

		spomin[razberiMesto(mestoBit, i)] = vrednost;
	}
}

std::map<long long, long long> preberiPodatke2(const std::string& pot) {

	std::map<long long, long long> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string prepoznava;
	std::string maska;
	long long mesto;
	long long vrednost;

	while (podatki.peek() != EOF) {

		podatki >> prepoznava;

		if (prepoznava == "mask") {
			podatki.ignore(2);
			podatki >> maska;
		}
		else {
			mesto = std::stoi(prepoznava.substr(4, prepoznava.size() - 5));
			podatki.ignore(2);
			podatki >> vrednost;

			zapisiVSpomin(resitev, maska, mesto, vrednost);
		}
	}

	return resitev;
}

unsigned long long vsota2(const std::map<long long, long long>& podatki) {

	unsigned long long resitev = 0;
	
	for (const auto& i : podatki)
		resitev += i.second;

	return resitev;
}


int main() {

	std::map<int, std::bitset<36>> podatki(preberiPodatke("2020/14.txt"));

	unsigned long long resitev1 = vsota(podatki);
	std::cout << "Vsota vseh vrednosti po verziji 1.0. je " << resitev1 << ".\n";

	std::map<long long, long long> podatki2(preberiPodatke2("2020/14.txt"));

	unsigned long long resitev2 = vsota2(podatki2);
	std::cout << "Vsota vseh vrednosti po verziji 2.0. je " << resitev2 << ".\n";

	if (_MSVC_LANG == 202302L) std::cout << "C++23";
	else if (_MSVC_LANG == 202002L) std::cout << "C++20";
	else if (_MSVC_LANG == 201703L) std::cout << "C++17";
	else if (_MSVC_LANG == 201402L) std::cout << "C++14";
	else if (_MSVC_LANG == 201103L) std::cout << "C++11";
	else if (_MSVC_LANG == 199711L) std::cout << "C++98";

	return 0;
}