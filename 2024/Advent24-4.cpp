/*
	relativno enostavno
	zelo enostavno
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>



bool operator==(std::queue<char> lhs, std::string rhs);
bool operator==(std::string lhs, std::queue<char> rhs);
void izpisi(std::queue<char>& vrsta);

std::vector<std::string> preberiPodatke(std::string pot);


inline bool stejZQueue(std::string& beseda, std::queue<char>& vrsta);
int stejHorizontalno(std::string& beseda, std::vector<std::string>& seznam);
int stejVertikalno(std::string& beseda, std::vector<std::string>& seznam);
int stejDiagonalnoPlusPlus(std::string& beseda, std::vector<std::string>& seznam, int& x, int& y);
int stejDiagonalnoPlus(std::string& beseda, std::vector<std::string>& seznam);
int stejDiagonalnoMinusMinus(std::string& beseda, std::vector<std::string>& seznam, int& x, int& y);
int stejDiagonalnoMinus(std::string& beseda, std::vector<std::string>& seznam);

int stejBesedo(std::string& beseda, std::vector<std::string>& seznam);


int stejBesedoX(std::string& beseda, std::vector<std::string>& seznam);




int main() {

	std::vector<std::string> seznam(preberiPodatke("Advent24-4.txt"));
	//std::for_each(seznam.begin(), seznam.end(), [](std::string str) {std::cout << str << '\n'; });


	std::string beseda = "XMAS";
	int resitev1(stejBesedo(beseda, seznam));

	std::cout << "Stevilo ponovitev besede '" << beseda << "' v seznamu je " << resitev1 << ".\n";


	std::string beseda2 = "MAS";
	int resitev2(stejBesedoX(beseda2, seznam));
	
	std::cout << "Stevilo ponovitev besede '" << beseda << "' oblike 'X' v seznamu je " << resitev2 << ".\n";


	return 0;
}



bool operator==(std::queue<char> lhs, std::string rhs) {
	if (lhs.size() != rhs.size()) return false;
	for (int i = 0; i < rhs.size(); i++) {
		if (lhs.front() != rhs[i]) return false;
		lhs.pop();
	}
	return true;
}

bool operator==(std::string lhs, std::queue<char> rhs) {
	return rhs == lhs;
}

void izpisi(std::queue<char>& vrsta) {
	int velikost = vrsta.size();
	for (int i = 0; i < velikost; i++) {
		std::cout << vrsta.front();
		vrsta.pop();
	}
	std::cout << '\n';
}


std::vector<std::string> preberiPodatke(std::string pot) {

	std::vector<std::string> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);
	podatki >> std::skipws;


	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	while (std::getline(podatki, vrstica))
		resitev.push_back(vrstica);

	podatki.close();


	return resitev;
}


inline bool stejZQueue(std::string& beseda, std::queue<char>& vrsta) {

	bool resitev = false;
	
	std::string rbeseda(beseda);
	std::reverse(rbeseda.begin(), rbeseda.end());

	if (vrsta == beseda || vrsta == rbeseda)
		resitev = true;

	return resitev;
}

int stejHorizontalno(std::string& beseda, std::vector<std::string>& seznam) {
	
	int resitev = 0;
	
	for (std::string vrstica : seznam) {
		std::queue<char> vrsta({ '.','.','.','.' });
		
		for (char c : vrstica) {

			vrsta.pop();
			vrsta.push(c);

			resitev += stejZQueue(beseda, vrsta);
		}
	}

	return resitev;
}

int stejVertikalno(std::string& beseda, std::vector<std::string>& seznam) {
	
	int resitev = 0;
	
	for (int x = 0; x < seznam[0].size(); x++) {
		std::queue<char> vrsta({ '.','.','.','.' });

		for (int y = 0; y < seznam.size(); y++) {

			vrsta.pop();
			vrsta.push(seznam[y][x]);

			resitev += stejZQueue(beseda, vrsta);
		}
	}

	return resitev;
}

int stejDiagonalnoPlusPlus(std::string& beseda, std::vector<std::string>& seznam, int& x, int& y) {

	int resitev = 0;

	std::queue<char> vrsta({ '.','.','.','.' });
	while (y < seznam.size() && x < seznam[0].size()) {

		vrsta.pop();
		vrsta.push(seznam[y++][x++]);

		resitev += stejZQueue(beseda, vrsta);
	}

	return resitev;
}

int stejDiagonalnoPlus(std::string& beseda, std::vector<std::string>& seznam) {

	int resitev = 0;

	for (int i = 0; i < seznam.size(); i++) {

		int y = i;
		int x = 0;

		resitev += stejDiagonalnoPlusPlus(beseda, seznam, x, y);
	}

	for (int i = 1; i < seznam[0].size(); i++) {

		int y = 0;
		int x = i;

		resitev += stejDiagonalnoPlusPlus(beseda, seznam, x, y);
	}

	return resitev;
}

int stejDiagonalnoMinusMinus(std::string& beseda, std::vector<std::string>& seznam, int& x, int& y) {

	int resitev = 0;

	std::queue<char> vrsta({ '.','.','.','.' });
	while (y >= 0 && x < seznam[0].size()) {

		vrsta.pop();
		vrsta.push(seznam[y--][x++]);

		resitev += stejZQueue(beseda, vrsta);
	}

	return resitev;
}

int stejDiagonalnoMinus(std::string& beseda, std::vector<std::string>& seznam) {

	int resitev = 0;

	for (int i = 0; i < seznam.size(); i++) {

		int y = i;
		int x = 0;

		resitev += stejDiagonalnoMinusMinus(beseda, seznam, x, y);
	}

	for (int i = 1; i < seznam[0].size(); i++) {

		int y = seznam.size() - 1;
		int x = i;

		resitev += stejDiagonalnoMinusMinus(beseda, seznam, x, y);
	}

	return resitev;
}

int stejBesedo(std::string& beseda, std::vector<std::string>& seznam) {

	int resitev = 0;

	resitev += stejHorizontalno(beseda, seznam);
	resitev += stejVertikalno(beseda, seznam);
	resitev += stejDiagonalnoPlus(beseda, seznam);
	resitev += stejDiagonalnoMinus(beseda, seznam);

	return resitev;
}



int stejBesedoX(std::string& beseda, std::vector<std::string>& seznam) {
	
	int resitev = 0;

	char& M = beseda[0];
	char& A = beseda[1];
	char& S = beseda[2];
	std::vector<std::string>& sez = seznam;
	for (int y = 1; y < seznam.size() - 1; y++) 
		for (int x = 1; x < seznam[y].size(); x++) 
			if (sez[y][x] == A)
				if (sez[y - 1][x - 1] == M && sez[y + 1][x + 1] == S ||
					sez[y - 1][x - 1] == S && sez[y + 1][x + 1] == M)
					if (sez[y + 1][x - 1] == M && sez[y - 1][x + 1] == S ||
						sez[y + 1][x - 1] == S && sez[y - 1][x + 1] == M)
						resitev++;

	return resitev;
}
