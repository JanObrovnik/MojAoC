/*
	Enostavno
	Enostavno
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>



class Antena {
public:
	int x, y;
	char id;

	Antena() : x(0), y(0), id(0) {}
	Antena(int x, int y, char id) : x(x), y(y), id(id) {}
};


Antena operator+(const Antena& lhs, const Antena& rhs) {
	return Antena(lhs.x + rhs.x, lhs.y + rhs.y, lhs.id);
}

Antena operator-(const Antena& lhs, const Antena& rhs) {
	return Antena(lhs.x - rhs.x, lhs.y - rhs.y, lhs.id);
}

bool operator<(const Antena& lhs, const Antena& rhs) {
	return lhs.id < rhs.id;
}

bool operator>(const Antena& lhs, const Antena& rhs) {
	return lhs.id > rhs.id;
}

bool operator==(const Antena& lhs, const Antena& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const Antena& an, char id) {
	return an.id == id;
}

bool operator==(const Antena& an, const std::vector<Antena>& sez) {
	return std::find(sez.begin(), sez.end(), an) != sez.end();
}

bool operator!=(const Antena& an, const std::vector<Antena>& sez) {
	return !(an == sez);
}

std::ostream& operator<<(std::ostream& os, Antena an) {
	return os << '(' << an.id << ':' << an.x << ',' << an.y << ')';
}



class Zemljevid {
	std::vector<Antena> seznamAnten{};
	std::vector<Antena> seznamAntiNode{};
	int dolzinaX{0}, dolzinaY{ 0 };


	void sortiraj() {
		std::sort(seznamAnten.begin(), seznamAnten.end());
	}

	void sortID() {
		std::sort(seznamAntiNode.begin(), seznamAntiNode.end());
	}

	void sortY() {
		std::sort(seznamAntiNode.begin(), seznamAntiNode.end(), [](Antena an1, Antena an2) {return an1.y < an2.y; });
	}

	void sortX() {
		std::sort(seznamAntiNode.begin(), seznamAntiNode.end(), [](Antena an1, Antena an2) {return an1.x < an2.x; });
	}


	bool znotrajMeja(Antena& antena) const {
		return antena.x >= 0 && antena.y >= 0 && antena.x < dolzinaX && antena.y < dolzinaY;
	}

	std::vector<Antena> izsek(char& id) {
		auto it1 = std::find(seznamAnten.begin(), seznamAnten.end(), id);
		auto itR = std::find(seznamAnten.rbegin(), seznamAnten.rend(), id);

		auto it2 = seznamAnten.begin() + (seznamAnten.size() - (itR - seznamAnten.rbegin()));

		return std::vector<Antena>(it1, it2);
	}

	std::set<char> narediSeznamID() {
		std::set<char> resitev;
		for (Antena& an : seznamAnten)
			resitev.insert(an.id);
		return resitev;
	}


	std::pair<Antena, Antena> sosednjaAntiNode(Antena& lhs, Antena& rhs) {

		if (lhs.id != rhs.id)
			return std::pair<Antena, Antena>(Antena(-1, -1, '.'), Antena(-1, -1, '.'));

		int difX = rhs.x - lhs.x;
		int difY = rhs.y - lhs.y;

		std::pair<Antena, Antena> resitev;

		resitev.first = Antena(lhs.x - difX, lhs.y - difY, lhs.id);
		resitev.second = Antena(rhs.x + difX, rhs.y + difY, rhs.id);

		//std::cout << '>' << lhs << ' ' << rhs << ':' << '\n';
		//std::cout << '\t' << resitev.first << ' ' << resitev.second << '\n' << '\n';

		return resitev;
	}

	void zapisiAntiNode(std::vector<Antena>& seznamSorodnihAnten) {

		if (seznamSorodnihAnten.size() < 2) return;

		for (int i = 0; i < seznamSorodnihAnten.size() - 1; i++)
			for (int j = i + 1; j < seznamSorodnihAnten.size(); j++) {

				std::pair<Antena, Antena> tockiAntiNode(sosednjaAntiNode(seznamSorodnihAnten[i], seznamSorodnihAnten[j]));

				if (znotrajMeja(tockiAntiNode.first) && tockiAntiNode.first != seznamAntiNode)
					seznamAntiNode.push_back(tockiAntiNode.first);

				if (znotrajMeja(tockiAntiNode.second) && tockiAntiNode.second != seznamAntiNode)
					seznamAntiNode.push_back(tockiAntiNode.second);
			}
	}


	std::vector<Antena> frekvencniAntiNode(Antena& lhs, Antena& rhs) {

		if (lhs.id != rhs.id)
			return std::vector<Antena>();

		int difX = rhs.x - lhs.x;
		int difY = rhs.y - lhs.y;

		std::vector<Antena> resitev;

		Antena lhsAntiNode(lhs.x - difX, lhs.y - difY, lhs.id);
		while (znotrajMeja(lhsAntiNode)) {

			resitev.push_back(lhsAntiNode);

			lhsAntiNode = Antena(lhsAntiNode.x - difX, lhsAntiNode.y - difY, lhsAntiNode.id);
		}

		Antena rhsAntiNode(rhs.x + difX, rhs.y + difY, rhs.id);
		while (znotrajMeja(rhsAntiNode)) {

			resitev.push_back(rhsAntiNode);

			rhsAntiNode = Antena(rhsAntiNode.x + difX, rhsAntiNode.y + difY, rhsAntiNode.id);
		}


		return resitev;
	}

	void zapisiFrekvenceAntiNode(std::vector<Antena>& seznamSorodnihAnten) {

		if (seznamSorodnihAnten.size() < 2) return;

		for (Antena& antena : seznamAnten)
			if (antena != seznamAntiNode)
				seznamAntiNode.push_back(antena);

		for (int i = 0; i < seznamSorodnihAnten.size() - 1; i++)
			for (int j = i + 1; j < seznamSorodnihAnten.size(); j++) {

				std::vector<Antena> tockeAntiNode(frekvencniAntiNode(seznamSorodnihAnten[i], seznamSorodnihAnten[j]));

				for (Antena& antena : tockeAntiNode) {
					if (antena != seznamAntiNode)
						seznamAntiNode.push_back(antena);
				}
			}
	}


	void resi(bool frekvenca) {

		sortiraj();

		std::set<char> seznamID(narediSeznamID());

		for (char c : seznamID) {

			std::vector<Antena> seznamSorodnihAnten(izsek(c));

			if (frekvenca) zapisiFrekvenceAntiNode(seznamSorodnihAnten);
			else zapisiAntiNode(seznamSorodnihAnten);
		}
	}


public:
	Zemljevid() : seznamAnten({}), dolzinaX(0), dolzinaY(0) { resi(false); }
	Zemljevid(std::vector<Antena> seznam, int x, int y, bool frekvenca) :
		seznamAnten(seznam), dolzinaX(x), dolzinaY(y) { resi(frekvenca); }
	

	void izpisAnten() const {
		std::cout << "Antene:\n";
		for (Antena an : seznamAnten) std::cout << an << '\n';
	}

	void izpisAntiNode() const {
		std::cout << "Anti node:\n";
		for (Antena an : seznamAntiNode) std::cout << an << '\n';
	}

	int steviloAntiNode() const {
		return seznamAntiNode.size();
	}
};



Zemljevid preberiPodatke(std::string pot, bool frekvenca = false) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena.\n";
		return Zemljevid();
	}

	std::vector<Antena> seznamAnten;

	int stVrstice = 0;
	int maxKolone = 0;
	std::string vrstica;
	while (std::getline(podatki, vrstica)) {

		std::stringstream ss(vrstica);

		int stKolone = 0;
		char c;
		while (ss >> c) {
			if (c != '.')
				seznamAnten.push_back(Antena(stKolone, stVrstice, c));

			stKolone++;
			if (maxKolone < stKolone)
				maxKolone = stKolone;
		}

		stVrstice++;
	}

	podatki.close();


	return Zemljevid(seznamAnten, maxKolone, stVrstice, frekvenca);
}



int main() {

	Zemljevid zemljevid1(preberiPodatke("Advent24-8.txt"));
	//zemljevid1.izpisAnten(); std::cout << std::endl;
	//zemljevid1.izpisAntiNode(); std::cout << std::endl;
	std::cout << "Stevilo razlicnih antinode-ov je " << zemljevid1.steviloAntiNode() << "." << std::endl;

	Zemljevid zemljevid2(preberiPodatke("Advent24-8.txt", true));
	//zemljevid2.izpisAnten(); std::cout << std::endl;
	//zemljevid2.izpisAntiNode(); std::cout << std::endl;
	std::cout << "Stevilo razlicnih frekvencnih antinode-ov je " << zemljevid2.steviloAntiNode() << "." << std::endl;


	return 0;
}
