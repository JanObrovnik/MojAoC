/*
	lahka
	lahka
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>



void prestaviZalogovnike1(std::vector<std::stack<char>>& zalogovniki, short pon, short iz, short na) {

	iz--;
	na--;

	for (int i = 0; i < pon; i++) {
		zalogovniki[na].push(zalogovniki[iz].top());
		zalogovniki[iz].pop();
	}
}

void prestaviZalogovnike2(std::vector<std::stack<char>>& zalogovniki, short pon, short iz, short na) {

	iz--;
	na--;

	std::stack<char> vmesnik;

	for (int i = 0; i < pon; i++) {
		vmesnik.push(zalogovniki[iz].top());
		zalogovniki[iz].pop();
	}
	for (int i = 0; i < pon; i++) {
		zalogovniki[na].push(vmesnik.top());
		vmesnik.pop();
	}
}


std::vector<std::stack<char>> preberiPrvotnoStanje(std::istream& podatki) {

	std::vector<std::stack<char>> resitev;


	std::string vrstica;
	std::vector<std::string> zapis;

	while (std::getline(podatki, vrstica)) {
		if (vrstica[1] == '1') break;
		zapis.push_back(vrstica);
	}

	resitev.resize((vrstica.size() + 1) / 4);
	for (auto it = zapis.rbegin(); it != zapis.rend(); it++)
		for (int i = 0; i < it->size(); i++)
			if ((*it)[i] >= 'A' && (*it)[i] <= 'Z')
				resitev[i / 4].push((*it)[i]);


	return resitev;
}

std::pair<std::string,std::string> preberiUkaze(std::istream& podatki, std::vector<std::stack<char>>& zalogovniki) {

	std::string resitev1 = "";
	std::string resitev2 = "";


	std::vector<std::stack<char>> zalogovniki1(zalogovniki);
	std::vector<std::stack<char>> zalogovniki2(zalogovniki);
	std::string str;
	short pon, iz, na;

	while (podatki.peek() != EOF) {

		podatki >> str >> pon >> str >> iz >> str >> na;

		prestaviZalogovnike1(zalogovniki1, pon, iz, na);
		prestaviZalogovnike2(zalogovniki2, pon, iz, na);
	}

	for (const auto& postavitev : zalogovniki1)
		resitev1.push_back(postavitev.top());
	for (const auto& postavitev : zalogovniki2)
		resitev2.push_back(postavitev.top());


	return { resitev1,resitev2 };
}

std::pair<std::string, std::string> preberiPodatke(std::string pot) {

	std::pair<std::string, std::string> resitev{ "","" };


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::vector<std::stack<char>>zalogovniki(preberiPrvotnoStanje(podatki));
	resitev = preberiUkaze(podatki, zalogovniki);

	podatki.close();


	return resitev;
}


int main(int argc, char* argv[]) {

	std::pair<std::string, std::string> resitev(preberiPodatke("Advent22/5.txt"));
	std::cout << "Vrhnji zabojniki so (CrateMover 9000): " << resitev.first << ".\n";
	std::cout << "Vrhnji zabojniki so (CrateMover 9001): " << resitev.second << ".\n";


	return 0;
}