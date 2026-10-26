/*
	t = 2:55:25 ur
	nekoliko lahko
	t = 18:53 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


int preberiPodatke(const std::string& pot) {

	int resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	podatki >> resitev;

	podatki.close();

	return resitev;
}


std::vector<int> najdiDeljitelje(const int& hisnaSt) {

	if (hisnaSt < 1)
		return {};

	std::vector<int> resitev = { 1 };

	if (hisnaSt == 1)
		return resitev;

	if (hisnaSt == 2) {
		resitev.push_back(2);
		return resitev;
	}

	int st = 2;

	while (st <= hisnaSt / 2) {
		if (hisnaSt % st == 0)
			resitev.push_back(st);
		st++;
	}

	resitev.push_back(hisnaSt);

	return resitev;
}

int dariloNaHiso(const int& hisnaSt, const int& steviloDaril) {

	int resitev = 0;

	std::vector<int> seznamDeljiteljev = najdiDeljitelje(hisnaSt);

	for (const int& i : seznamDeljiteljev)
		resitev += i;

	return resitev * steviloDaril;
}
int dariloNaHisoOmejitve(const int& hisnaSt, const int& steviloDaril, const int& omejitev) {

	int resitev = 0;

	std::vector<int> seznamDeljiteljev = najdiDeljitelje(hisnaSt);

	for (const int& i : seznamDeljiteljev) {
		if (hisnaSt / i > omejitev)
			continue;
		resitev += i;
	}

	return resitev * steviloDaril;
}


std::vector<int> razsiriInt(const int& vrednost) {

	std::set<int> resitev;

	for (int i = 1; i < vrednost + 1; i++) {
		for (int j = i; j < vrednost + 1; j++) {
			resitev.insert(i * j);
		}
	}

	return std::vector<int>(resitev.begin(), resitev.end());
}

std::vector<int> razsiriSeznam(std::vector<int> seznam) {

	std::set<int> resitev;

	for (int i = 1; i < seznam.size() - 1; i++) {
		for (int j = i + 1; j < seznam.size(); j++) {
			resitev.insert(seznam[i] * seznam[j]);
		}
 	}

	return std::vector<int>(resitev.begin(), resitev.end());
}

int hisaIzDaril(int vrednost) {

	vrednost /= 10;

	std::vector<int> seznamSkratov;

	int i = 1;

	while (true) {

		seznamSkratov.push_back(i);

		std::vector<int> razsirjenSeznamSkratov = razsiriSeznam(seznamSkratov);

		i++;
	}

	return -1;
}


bool ovrednostiPrastevili(const int& st) {

	if (st < 2)
		return false;
	if (st == 2)
		return true;
	if (st % 2 == 0)
		return false;
	for (int i = 3; i <= st / 2; i += 2)
		if (st % i == 0)
			return false;

	return true;
}

std::vector<std::pair<int, int>> razdeliNaPrafaktorje(int st) {

	std::vector<std::pair<int, int>> resitev;

	std::vector<int> raz = najdiDeljitelje(st);

	std::vector<int> prastevila;
	for (int i : raz)
		if (ovrednostiPrastevili(i)) {
			prastevila.push_back(i);
			resitev.push_back({ i,0 });
		}


	for (int i = 0; i < prastevila.size(); i++) {
		if (st % prastevila[i] == 0) {
			st /= prastevila[i];
			resitev[i].second++;
			i--;
		}
	}


	return resitev;
}



int main() {

	int iskanaVrednost = preberiPodatke("2015/20.txt");


	int resitev = 0;

	while (dariloNaHiso(resitev, 10) < iskanaVrednost) {
		if (resitev % 1000 == 0 && false)
			std::cout << resitev << '\n';
		resitev += 5040;
	}

	std::cout << "Prva hisa z " << iskanaVrednost << " stevilom daril je " << resitev << ".\n";

	if (false) {
		std::cout << "D: " << dariloNaHiso(resitev, 10) << '\n';

		std::vector<int> del = najdiDeljitelje(resitev);
		for (int i : del) std::cout << i << ' ';
		std::cout << '\n';

		std::vector<std::pair<int, int>> raz = razdeliNaPrafaktorje(resitev);
		for (auto par : raz) std::cout << par.first << "**" << par.second << ' ';
		std::cout << '\n';

		int res = 1;
		for (auto par : raz) {
			res *= pow(par.first, par.second);
		}
		std::cout << res << '\n' << '\n' << '\n';
	}


	while (dariloNaHisoOmejitve(resitev, 11, 50) < iskanaVrednost) {
		if (resitev % 1000 == 0 && false)
			std::cout << resitev << '\n';
		resitev += 5040;
	}

	std::cout << "Prva hisa z omejitvijo z " << iskanaVrednost << " stevilom daril je " << resitev << ".\n";

	if (false) {
		std::cout << "D: " << dariloNaHiso(resitev, 11) << '\n';

		std::vector<int> del = najdiDeljitelje(resitev);
		for (int i : del) std::cout << i << ' ';
		std::cout << '\n';

		std::vector<std::pair<int, int>> raz = razdeliNaPrafaktorje(resitev);
		for (auto par : raz) std::cout << par.first << "**" << par.second << ' ';
		std::cout << '\n';

		int res = 1;
		for (auto par : raz) {
			res *= pow(par.first, par.second);
		}
		std::cout << res << '\n' << '\n' << '\n';
	}


	return 0;
}
