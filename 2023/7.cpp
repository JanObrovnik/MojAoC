/*
	Relativno tezka
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>


static int evalChar(char c) {
	if (c == 'A') return 12;
	if (c == 'K') return 11;
	if (c == 'Q') return 10;
	if (c == 'J') return 9;
	if (c == 'T') return 8;
	if (c == '9') return 7;
	if (c == '8') return 6;
	if (c == '7') return 5;
	if (c == '6') return 4;
	if (c == '5') return 3;
	if (c == '4') return 2;
	if (c == '3') return 1;
	if (c == '2') return 0;

	return 0;
}

static int evalJokerChar(char c) {
	if (c == 'A') return 12;
	if (c == 'K') return 11;
	if (c == 'Q') return 10;
	if (c == 'J') return -1;
	if (c == 'T') return 8;
	if (c == '9') return 7;
	if (c == '8') return 6;
	if (c == '7') return 5;
	if (c == '6') return 4;
	if (c == '5') return 3;
	if (c == '4') return 2;
	if (c == '3') return 1;
	if (c == '2') return 0;

	return 0;
}

static int evalRank(std::vector<int> seznam) {
	if (seznam.size() == 5) return 0;
	if (seznam.size() == 4) return 1;
	if (seznam.size() == 3) {
		if (seznam.end() == std::find(seznam.begin(), seznam.end(), 3)) return 2;
		else return 3;
	}
	if (seznam.size() == 2) {
		if (seznam.end() == std::find(seznam.begin(), seznam.end(), 4)) return 4;
		else return 5;
	}
	if (seznam.size() == 1) return 6;

	return 0;
}


class Karte {
public:
	std::map<char, int> karte;

	Karte() {
		karte['A'] = 0;
		karte['K'] = 0;
		karte['Q'] = 0;
		karte['J'] = 0;
		karte['T'] = 0;
		karte['9'] = 0;
		karte['8'] = 0;
		karte['7'] = 0;
		karte['6'] = 0;
		karte['5'] = 0;
		karte['4'] = 0;
		karte['3'] = 0;
		karte['2'] = 0;
	}

	void dodaj(char oznaka) { karte[oznaka]++; }

	bool optimizirajJokerja() {
		const int steviloJokerjev = karte['J'];

		if (steviloJokerjev == 0 || steviloJokerjev == 5)
			return false;

		karte['J'] = 0;

		char maxChar = 'J';
		int maxKolicina = 0;
		for (auto it = karte.begin(); it != karte.end(); it++) {
			const int& kolicina = it->second;
			if (kolicina > maxKolicina) {
				maxChar = it->first;
				maxKolicina = kolicina;
			}
		}

		karte[maxChar] += steviloJokerjev;

		return true;
	}
};

static bool primerjava(std::string in1, std::string in2, bool del2) {

	Karte karta1, karta2;

	std::for_each(in1.begin(), in1.end(), [&karta1](char c) {karta1.dodaj(c); });
	std::for_each(in2.begin(), in2.end(), [&karta2](char c) {karta2.dodaj(c); });

	if (del2) {
		karta1.optimizirajJokerja();
		karta2.optimizirajJokerja();
	}


	std::vector<int> stPonovitev1, stPonovitev2;

	for (auto& karta : karta1.karte)
		if (karta.second > 0)
			stPonovitev1.push_back(karta.second);

	for (auto& karta : karta2.karte)
		if (karta.second > 0)
			stPonovitev2.push_back(karta.second);


	if (evalRank(stPonovitev1) == evalRank(stPonovitev2)) {
		for (int i = 0; i < in1.size(); i++)
			if (in1[i] != in2[i]) {
				if (del2)
					return evalJokerChar(in1[i]) < evalJokerChar(in2[i]);
				else
					return evalChar(in1[i]) < evalChar(in2[i]);
			}
	}
	else return evalRank(stPonovitev1) < evalRank(stPonovitev2);
}


class IgraKamel {
private:
	std::vector<std::string> roka;
	std::map<std::string, int> stava;

public:
	void dodajIgralca(std::string roka, int stava) { this->roka.push_back(roka); this->stava[roka] = stava; }
	void ovrednoti(bool del2) { std::sort(roka.begin(), roka.end(), [&](std::string str1, std::string str2) {return primerjava(str1, str2, del2); }); }
	int skupnaZmaga() { int resitev = 0; for (int i = 0; i < roka.size(); i++) resitev += (i + 1) * stava[roka[i]]; return resitev; }

	void preberi() const { std::for_each(roka.begin(), roka.end(), [](std::string str) {std::cout << str << '\n'; }); }
};



int main() {

	IgraKamel igra;


	std::fstream podatki;
	podatki.open("Advent23/7.txt", std::ios::in);

	if (podatki.is_open()) {
		while (podatki.peek() != EOF) {

			std::string str;
			int i;

			podatki >> str >> i;

			igra.dodajIgralca(str, i);
		}
		podatki.close();
	}


	igra.ovrednoti(false);
	//igra.preberi();

	std::cout << "Skupna zmaga je " << igra.skupnaZmaga() << ".\n";



	igra.ovrednoti(true);
	//igra.preberi();

	std::cout << "Skupna joker zmaga je " << igra.skupnaZmaga() << ".\n";
	std::cout << std::endl;



	return 0;
}