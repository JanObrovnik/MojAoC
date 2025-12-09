/*
	t = 2:22:11 ur
	malo tezko
	t = 14:11 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


void nastavitve() {
	std::cout.precision(3);
	std::cout << std::fixed;
}


class Tocka {
public:
	float x, y, z;

	Tocka() : x(-1), y(-1), z(-1) {}
	Tocka(float x, float y, float z) : x(x), y(y), z(z) {}
};
std::istream& operator>>(std::istream& is, Tocka& to) {
	is >> to.x;
	is.ignore(1);
	is >> to.y;
	is.ignore(1);
	is >> to.z;
	return is;
}
std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y << ',' << to.z;
}
float razdaljaTock(const Tocka& to1, const Tocka& to2) {
	return sqrt((to1.x - to2.x) * (to1.x - to2.x) + (to1.y - to2.y) * (to1.y - to2.y) + (to1.z - to2.z) * (to1.z - to2.z));
}

struct Razdalja {
	float razdalja;
	const Tocka* kazalec1, *kazalec2;

	Razdalja(float razdalja, const Tocka* kazalec1, const Tocka* kazalec2) : razdalja(razdalja), kazalec1(kazalec1), kazalec2(kazalec2) {}
};
std::ostream& operator<<(std::ostream& os, const Razdalja& ra) {
	return os << ra.razdalja << ":\t" << ra.kazalec1 << ',' << ra.kazalec2;
}
bool operator<(const Razdalja& ra1, const Razdalja& ra2) {
	return ra1.razdalja < ra2.razdalja;
}
bool operator==(const Razdalja& ra1, const Razdalja& ra2) {
	return ra1.razdalja == ra2.razdalja;
}
bool operator>(const Razdalja& ra1, const Razdalja& ra2) {
	return ra1.razdalja > ra2.razdalja;
}


std::vector<Tocka> preberiPodatke(const std::string& pot) {

	std::vector<Tocka> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	Tocka tocka;

	while (podatki >> tocka)
		resitev.push_back(tocka);

	podatki.close();

	return resitev;
}


int poveziTocka(const std::vector<Tocka>& seznamTock, const int& steviloPovezav, const int& steviloZmnozka = 3) {

	std::vector<Razdalja> seznamRazdalj;

	for (int i = 0; i < seznamTock.size() - 1; i++)
		for (int j = i + 1; j < seznamTock.size(); j++)
			seznamRazdalj.push_back(Razdalja(razdaljaTock(seznamTock[i], seznamTock[j]), &seznamTock[i], &seznamTock[j]));

	std::sort(seznamRazdalj.begin(), seznamRazdalj.end());


	std::vector<std::set<const Tocka*>> seznamTokokrogov;

	for (int i = 0; i < steviloPovezav; i++) {

		const Razdalja& razdalja = seznamRazdalj[i];


		std::pair<int, int> indeks{ -1,-1 };

		for (int j = 0; j < seznamTokokrogov.size(); j++) {

			std::set<const Tocka*>& tokokrog = seznamTokokrogov[j];

			if (tokokrog.contains(razdalja.kazalec1)) {
				indeks.first = j;
			}
			if (tokokrog.contains(razdalja.kazalec2)) {
				indeks.second = j;
			}
		}


		if (indeks.first == -1 && indeks.second == -1) {
			seznamTokokrogov.push_back(std::set<const Tocka*>{razdalja.kazalec1, razdalja.kazalec2});
		}
		else if (indeks.first != -1 && indeks.second == -1) {
			seznamTokokrogov[indeks.first].insert(razdalja.kazalec2);
		}
		else if (indeks.first == -1 && indeks.second != -1) {
			seznamTokokrogov[indeks.second].insert(razdalja.kazalec1);
		}
		else if (indeks.first != -1 && indeks.second != -1 && indeks.first != indeks.second) {
			for (const Tocka* tocka : seznamTokokrogov[indeks.second]) {
				seznamTokokrogov[indeks.first].insert(tocka);
			}
			seznamTokokrogov.erase(seznamTokokrogov.begin() + indeks.second);
		}
	}

	std::sort(seznamTokokrogov.begin(), seznamTokokrogov.end(),
		[](const std::set<const Tocka*>& sez1, const std::set<const Tocka*>& sez2) {
			return sez1.size() > sez2.size();
		});

	
	int resitev = 1;

	for (int i = 0; i < steviloZmnozka; i++)
		resitev *= seznamTokokrogov[i].size();

	return resitev;
}

long long poveziVseTocka(const std::vector<Tocka>& seznamTock) {

	std::vector<Razdalja> seznamRazdalj;

	for (int i = 0; i < seznamTock.size() - 1; i++)
		for (int j = i + 1; j < seznamTock.size(); j++)
			seznamRazdalj.push_back(Razdalja(razdaljaTock(seznamTock[i], seznamTock[j]), &seznamTock[i], &seznamTock[j]));

	std::sort(seznamRazdalj.begin(), seznamRazdalj.end());


	std::vector<std::set<const Tocka*>> seznamTokokrogov;

	for (const Razdalja& razdalja : seznamRazdalj) {

		
		std::pair<int, int> indeks{ -1,-1 };

		for (int j = 0; j < seznamTokokrogov.size(); j++) {

			std::set<const Tocka*>& tokokrog = seznamTokokrogov[j];

			if (tokokrog.contains(razdalja.kazalec1)) {
				indeks.first = j;
			}
			if (tokokrog.contains(razdalja.kazalec2)) {
				indeks.second = j;
			}
		}


		if (indeks.first == -1 && indeks.second == -1) {
			seznamTokokrogov.push_back(std::set<const Tocka*>{razdalja.kazalec1, razdalja.kazalec2});
		}
		else if (indeks.first != -1 && indeks.second == -1) {
			seznamTokokrogov[indeks.first].insert(razdalja.kazalec2);
		}
		else if (indeks.first == -1 && indeks.second != -1) {
			seznamTokokrogov[indeks.second].insert(razdalja.kazalec1);
		}
		else if (indeks.first != -1 && indeks.second != -1 && indeks.first != indeks.second) {
			for (const Tocka* tocka : seznamTokokrogov[indeks.second]) {
				seznamTokokrogov[indeks.first].insert(tocka);
			}
			seznamTokokrogov.erase(seznamTokokrogov.begin() + indeks.second);
		}


		if (seznamTokokrogov.front().size() == seznamTock.size())
			return long long(razdalja.kazalec1->x) * long long(razdalja.kazalec2->x);
	}


	std::cout << "NAPAKA.\n";
	return -1;
}


int main() {

	nastavitve();
	std::vector<Tocka> seznamTock = preberiPodatke("2025/8.txt");

	int resitev1 = poveziTocka(seznamTock, 1000);
	std::cout << "Zmnozek vseh tokokrogov je " << resitev1 << ".\n";

	long long resitev2 = poveziVseTocka(seznamTock);
	std::cout << "Oddaljenost od stene je " << resitev2 << ".\n";


	return 0;
}
