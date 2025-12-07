/*
	t = 43:14 min
	lahko
	t = 30:04 min
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>


class Tocka {
public:
	int x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(int x, int y) : x(x), y(y) {}

};
bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y < to2.y;
	return to1.x < to2.x;
}
bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.y == to2.y && to1.x == to2.x;
}
bool operator>(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y > to2.y;
	return to1.x > to2.x;
}
Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}


void preberiPodatke(const std::string& pot, Tocka& start, std::set<Tocka>& seznamRazdelilcev, int& yMax) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	int y = 0;
	std::string vrstica;

	while (podatki >> vrstica) {

		int x = 0;

		for (const char& c : vrstica) {

			if (c == 'S') start = Tocka(x, y);
			else if (c == '^') seznamRazdelilcev.insert(Tocka(x, y));

			x++;
		}
		y++;
	}
	yMax = y;

	podatki.close();

	return;
}


int simuliraj1(const Tocka& start, const std::set<Tocka>& seznamRazdelilcev, const int& yMax) {

	int resitev = 0;

	std::set<Tocka> seznamTrajektorij;
	seznamTrajektorij.insert(start);

	while (seznamTrajektorij.begin()->y < yMax) {

		std::set<Tocka> vmesniSeznamTrajektorij;

		for (Tocka trajektorija : seznamTrajektorij) {

			trajektorija.y++;

			if (seznamRazdelilcev.find(trajektorija) != seznamRazdelilcev.end()) {
				vmesniSeznamTrajektorij.insert(trajektorija + Tocka(1, 0));
				vmesniSeznamTrajektorij.insert(trajektorija + Tocka(-1, 0));
				resitev++;
			}
			else {
				vmesniSeznamTrajektorij.insert(trajektorija);
			}
		}

		seznamTrajektorij = vmesniSeznamTrajektorij;
	}

	return resitev;
}

long long simuliraj2(const Tocka& start, const std::set<Tocka>& seznamRazdelilcev, const int& yMax) {

	long long resitev = 0;

	std::map<Tocka, long long> seznamTrajektorij;
	seznamTrajektorij[start] = 1;

	while (seznamTrajektorij.begin()->first.y < yMax) {

		std::map<Tocka, long long> vmesniSeznamTrajektorij;

		for (std::pair<Tocka, long long> par : seznamTrajektorij) {

			Tocka& trajektorija = par.first;
			long long& stevilo = par.second;

			trajektorija.y++;

			if (seznamRazdelilcev.find(trajektorija) != seznamRazdelilcev.end()) {
				vmesniSeznamTrajektorij[trajektorija + Tocka(1, 0)] += stevilo;
				vmesniSeznamTrajektorij[trajektorija + Tocka(-1, 0)] += stevilo;
			}
			else {
				vmesniSeznamTrajektorij[trajektorija] += stevilo;
			}
		}

		seznamTrajektorij = vmesniSeznamTrajektorij;
	}

	for (const std::pair<Tocka, long long>& par : seznamTrajektorij)
		resitev += par.second;

	return resitev;
}


int main() {

	Tocka start;
	std::set<Tocka> seznamRazdelilcev;
	int yMax;

	preberiPodatke("2025/7.txt", start, seznamRazdelilcev, yMax);

	int resitev1 = simuliraj1(start, seznamRazdelilcev, yMax);
	std::cout << "Stevilo razdelitev je " << resitev1 << ".\n";

	long long resitev2 = simuliraj2(start, seznamRazdelilcev, yMax);
	std::cout << "Stevilo razlicnih poti je " << resitev2 << ".\n";


	return 0;
}
