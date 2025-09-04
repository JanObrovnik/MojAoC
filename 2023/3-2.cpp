/*
	lahko
	dokaj lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <array>
#include <numeric>


class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}


	bool operator==(const Tocka& to) const {
		return x == to.x && y == to.y;
	}
	bool operator<(const Tocka& to) const {
		if (y != to.y) return y < to.y;
		return x < to.x;
	}
	bool operator>(const Tocka& to) const {
		if (y != to.y) return y > to.y;
		return x > to.x;
	}

	struct HashFunkcija {
		size_t operator()(const Tocka& to) const {
			size_t xHash = std::hash<int>()(to.x);
			size_t yHash = std::hash<int>()(to.y) << 1;
			return xHash ^ yHash;
		}
	};
};

void operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
}


std::vector<std::string> preberiPodatke(const std::string& pot) {

	std::vector<std::string> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti\n";

		resitev = { "467..114..",
					"...*......",
					"..35..633.",
					"......#...",
					"617*......",
					".....+.58.",
					"..592.....",
					"......755.",
					"...$.*....",
					".664.598.."
		};
	}

	std::string vrstica;
	while (std::getline(podatki, vrstica))
		resitev.push_back(vrstica);

	podatki.close();


	return resitev;
}


class Kos {
	std::vector<Tocka> seznamTock;
	int vrednost;
	bool veljaven;

	const std::array<Tocka, 8> seznamSosed{ Tocka(1,0), Tocka(1,1), Tocka(0,1), Tocka(-1,1),
											Tocka(-1,0), Tocka(-1,-1), Tocka(0,-1), Tocka(1,-1) };

public:
	Kos(std::vector<Tocka> seznamTock, int vrednost) : seznamTock(seznamTock), vrednost(vrednost), veljaven(false) {}
	Kos(std::vector<Tocka> seznamTock, int vrednost, const std::unordered_set<Tocka,Tocka::HashFunkcija>& seznamZnakov) :
		seznamTock(seznamTock), vrednost(vrednost), veljaven(false) { preveriVeljavnost(seznamZnakov); }


	void preveriVeljavnost(const std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamZnakov) {

		for (const Tocka& tocka : seznamTock) {
			for (Tocka soseda : seznamSosed) {

				soseda += tocka;

				if (seznamZnakov.find(soseda) != seznamZnakov.end()) {
					veljaven = true;
					return;
				}
			}
		}

		veljaven = false;
	}


	int dobiVrednost() const {
		return veljaven * vrednost;
	}


	friend std::unordered_map<Tocka, const Kos*, Tocka::HashFunkcija> narediZemljevidKosov(
		const std::vector<Kos>& seznamKosov);
};


void zapisKosov(const std::vector<std::string>& shema,
				std::vector<Kos>& seznamKosov, 
				std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamZnakov) {


	std::string stevilo;
	std::vector<Tocka> seznamTock;

	for (int y = 0; y < shema.size(); y++) {
		for (int x = 0; x < shema[y].size(); x++) {
			
			const char& simbol = shema[y][x];
			
			if (simbol >= '0' && simbol <= '9') {
				stevilo.push_back(simbol);
				seznamTock.push_back(Tocka(x, y));
			}
			else {
				if (!stevilo.empty()) {
					seznamKosov.push_back(Kos(seznamTock, std::stoi(stevilo)));

					stevilo.clear();
					seznamTock.clear();
				}

				if (simbol != '.') {
					seznamZnakov.insert(Tocka(x, y));
				}
			}
		}
	}
}


std::vector<Tocka> narediSeznamZobnikov(const std::vector<std::string>& shema,
										const std::unordered_set<Tocka, Tocka::HashFunkcija>& seznamZnakov) {

	std::vector<Tocka> resitev;

	for (const Tocka& tocka : seznamZnakov) {
		if (shema[tocka.y][tocka.x] == '*')
			resitev.push_back(tocka);
	}

	return resitev;
}

std::unordered_map<Tocka, const Kos*, Tocka::HashFunkcija> narediZemljevidKosov(const std::vector<Kos>& seznamKosov) {

	std::unordered_map<Tocka, const Kos*, Tocka::HashFunkcija> resitev;

	for (const Kos& kos : seznamKosov) {
		for (const Tocka& tocka : kos.seznamTock) {
			resitev[tocka] = &kos;
		}
	}

	return resitev;
}

int prestavnoRazmerjeZobnika(const Tocka& tocka,
							 const std::unordered_map<Tocka, const Kos*, Tocka::HashFunkcija>& zemljevidKosov) {

	int resitev = 0;

	static const std::array<Tocka, 8> seznamSosed{ Tocka(1,0), Tocka(1,1), Tocka(0,1), Tocka(-1,1),
												   Tocka(-1,0), Tocka(-1,-1), Tocka(0,-1), Tocka(1,-1) };

	std::set<const Kos*> seznamSosednjihKosov;

	for (Tocka soseda : seznamSosed) {
		soseda += tocka;
		if (zemljevidKosov.find(soseda) != zemljevidKosov.end())
			seznamSosednjihKosov.insert(zemljevidKosov.at(soseda));
	}

	if (seznamSosednjihKosov.size() == 2)
		resitev = (*seznamSosednjihKosov.begin())->dobiVrednost() * (*seznamSosednjihKosov.rbegin())->dobiVrednost();

	//std::cout << "st: " << seznamSosednjihKosov.size() << '\t';
	//std::cout << "res: " << resitev << '\n';

	return resitev;
}


int main(int argc, char* argv[]) {

	std::vector<std::string> shema(preberiPodatke("Advent23/3.txt"));

	std::vector<Kos> seznamKosov;
	std::unordered_set<Tocka, Tocka::HashFunkcija> seznamZnakov;

	zapisKosov(shema, seznamKosov, seznamZnakov);


	for (Kos& kos : seznamKosov) {
		kos.preveriVeljavnost(seznamZnakov);
	}

	int resitev(std::accumulate(seznamKosov.begin(), seznamKosov.end(), 0, [](int resitev, const Kos& kos) {
		return resitev + kos.dobiVrednost(); 
		}));


	std::cout << "Vsota vseh kosov je " << resitev << ".\n";



	std::vector<Tocka> seznamZobnikov(narediSeznamZobnikov(shema, seznamZnakov));

	std::unordered_map<Tocka, const Kos*, Tocka::HashFunkcija> zemljevidKosov(narediZemljevidKosov(seznamKosov));

	
	int resitev2 = 0;

	for (const Tocka& zobnik : seznamZobnikov)
		resitev2 += prestavnoRazmerjeZobnika(zobnik, zemljevidKosov);


	std::cout << "Vsota vseh prestavnih razmerij je " << resitev2 << ".\n";



	return 0;
}
