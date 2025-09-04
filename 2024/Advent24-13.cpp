/*
	lahka
	tezka
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>


class Gumb {
public:
	unsigned long long x, y;

	Gumb() : x(0), y(0) {}
	Gumb(unsigned long long x, unsigned long long y) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& os, Gumb gu) {
	return os << '(' << gu.x << ',' << gu.y << ')';
}

unsigned long long operator/(Gumb gu1, Gumb gu2) {
	unsigned long long x = (unsigned long long)ceil((float)gu1.x / gu2.x);
	unsigned long long y = (unsigned long long)ceil((float)gu1.y / gu2.y);
	return std::max(x, y);
}

Gumb operator*(Gumb gu, unsigned long long i) {
	return Gumb(gu.x * i, gu.y * i);
}

Gumb operator*(unsigned long long i, Gumb gu) {
	return Gumb(gu.x * i, gu.y * i);
}

Gumb operator+(Gumb gu1, Gumb gu2) {
	return Gumb(gu1.x + gu2.x, gu1.y + gu2.y);
}

Gumb operator+(Gumb gu1, long long i) {
	return Gumb(gu1.x + i, gu1.y + i);
}

Gumb& operator+=(Gumb& gu1, long long i) {
	gu1 = gu1 + i;
	return gu1;
}
/*
bool operator%(Gumb gu1, Gumb gu2) {
	return gu1.x % gu2.x == 0 && gu1.y % gu2.y == 0;
}
*/
bool operator==(Gumb gu1, Gumb gu2) {
	return gu1.x == gu2.x && gu1.y == gu2.y;
}

bool operator>(Gumb gu1, Gumb gu2) {
	return gu1.x > gu2.x || gu1.y > gu2.y;
}

bool operator<(Gumb gu1, Gumb gu2) {
	return gu1.x < gu2.x || gu1.y < gu2.y;
}



class Masina {
	Gumb gumbA, gumbB;
	Gumb nagrada;

public:
	Masina() : gumbA(), gumbB(), nagrada() {}
	Masina(Gumb A, Gumb B, Gumb nagrada) : gumbA(A), gumbB(B), nagrada(nagrada) {}


	int racunaj1del(int prim = -1) {

		std::list<std::pair<int, int>> seznamResitev;

		int samoA = nagrada / gumbA;
		int samoB = nagrada / gumbB;

		//std::cout << samoA << ' ' << samoB << '\n';

		if (samoA > 100) samoA = 100;
		if (samoB > 100) samoB = 100;

		int stA = samoA;
		int stB = 0;
		while (stB <= samoB && stA >= 0) {

			if (stA * gumbA + stB * gumbB == nagrada)
				seznamResitev.push_back({ stA--,stB });

			while (stA * gumbA + stB * gumbB > nagrada) {
				stA--;
				if (stA < 0) break;
			}
			
			while (stA * gumbA + stB * gumbB < nagrada) {
				stB++;
				if (stB > samoB) break;
			}
		}


		if (seznamResitev.empty())
			return 0;

		//for (auto par : seznamResitev) std::cout << par.first << '|' << par.second << '\n';

		auto it = std::min_element(seznamResitev.begin(), seznamResitev.end(), [](auto& par1, auto& par2) {
			return 3 * par1.first + par1.second < 3 * par2.first + par2.second;
			});

		int resitev = 3 * it->first + it->second;


		return resitev;
	}

	/*

	po >2:	0
	po >5:	490550864530
	po >10:	33086626816008
	po >20: 93921372245508
	po >30: 95688837203288
	po >40: 95688837203288
	po >99: 95688837203288
	*1		95688837203288
	*2		95688837203288

	prim: 60
	prim: 103
	prim: 130
	prim: 141
	prim: 255

	*/
	unsigned long long racunaj2del(int prim = -1) {

		int maxPon = 30;
		int preveri = 174;

		std::list<std::pair<long long, long long>> seznamResitev;

		long long maxA;
		if (gumbA.x > gumbA.y) maxA = ceil((double)nagrada.x / gumbA.x);
		else maxA = ceil((double)nagrada.y / gumbA.y);

		long long maxB;
		if (gumbB.x > gumbB.y) maxB = ceil((double)nagrada.x / gumbB.x);
		else maxB = ceil((double)nagrada.y / gumbB.y);

		//std::cout << maxA * gumbA << ' ' << maxB * gumbB << '\n';
		//std::cout << maxA << ' ' << maxB << '\n';

		
		long long stA = maxA;
		long long stB = 0;

		long long razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
		long long razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);

		while (razlikaX < 0 && razlikaY < 0) {
			stA--;
			stB--;
		
			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);
		}

		while (razlikaX > 0 && razlikaY > 0) {
			stA++;
			stB++;

			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);
		}

		int pon = 0;

		//if (prim == 60 || prim == 86 || prim == 103) {
			//std::cout << "dela\n";
		//if (prim != -1)
		//	std::cout << razlikaX << '|' << razlikaY << '\n';
		//}
		if (prim == preveri) {
			std::cout << *this << '\n';
		}
		
		if (razlikaY > 0 /* && razlikaX < 0*/) while (true) {

			bool niSpremembe = true;

			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);

			if (prim == preveri) {
				std::cout << razlikaX << '\n';
				std::cout << razlikaY << '\n';
			}

			if (razlikaY > 0) {
				//if (prim == preveri) std::cout << "dela "<< razlikaY <<'|' << gumbB.y <<'='<< ceil((double)razlikaY / gumbB.y) << " stB " << stB << "\n";
				stB += (long long)ceil((double)razlikaY / gumbB.y);
				//if (prim == preveri) std::cout << "-stB " << stB+ (long long)ceil((double)razlikaY / gumbB.y) << "\n";
				niSpremembe = false;
			}


			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);

			if (prim == preveri) {
				std::cout << razlikaX << '\n';
				std::cout << razlikaY << '\n';
			}


			if (razlikaX < 0) {
				stA -= (long long)ceil((double)abs(razlikaX) / gumbA.x);
				niSpremembe = false;
			}


			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);

			if (prim == preveri) {
				std::cout << razlikaX << '\n';
				std::cout << razlikaY << '\n' << niSpremembe << '\n' << '\n';
			}

			if (niSpremembe) break;
			if (pon > maxPon) { /*std::cout << "prim: " << prim << '\n';*/ break; }
			pon++;
		}
		else if (razlikaX > 0 /* && razlikaY < 0*/) while (true) {

			bool niSpremembe = true;

			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);

			if (prim == preveri) {
				std::cout << razlikaX << '\n';
				std::cout << razlikaY << '\n';
			}


			if (razlikaX > 0) {
				stB += (long long)ceil((double)razlikaX / gumbB.x);
				niSpremembe = false;
			}


			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);

			if (prim == preveri) {
				std::cout << razlikaX << '\n';
				std::cout << razlikaY << '\n';
			}


			if (razlikaY < 0) {
				stA -= (long long)ceil((double)abs(razlikaY) / gumbA.y);
				niSpremembe = false;
			}


			razlikaX = nagrada.x - (gumbA.x * stA + gumbB.x * stB);
			razlikaY = nagrada.y - (gumbA.y * stA + gumbB.y * stB);

			if (prim == preveri) {
				std::cout << razlikaX << '\n';
				std::cout << razlikaY << '\n'  << niSpremembe << '\n';
			}

			if (niSpremembe) break;
			if (pon > maxPon) { /*std::cout << "prim: " << prim << '\n';*/ break; }
			pon++;
		}


		if (razlikaX != 0 || razlikaY != 0) return 0;


		unsigned long long resitev = 3 * stA + stB;


		return resitev;
	}


	
	friend std::ostream& operator<<(std::ostream& os, Masina ma);
};

std::ostream& operator<<(std::ostream& os, Masina ma) {
	return os << ma.nagrada << ':' << ' ' << ma.gumbA << ' ' << ma.gumbB;
}



std::list<Masina> preberiPodatke(std::string pot, bool del2 = false) {

	std::list<Masina> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena.\n";
		return resitev;
	}

	while (podatki.peek() != EOF) {

		char c;
		unsigned long long x, y;

		podatki.ignore(10);
		podatki >> c >> x >> c >> c >> y;
		Gumb gumbA(x,y);

		podatki.ignore(10);
		podatki >> c >> x >> c >> c >> y;
		Gumb gumbB(x, y);

		podatki.ignore(7);
		podatki >> c >> c >> x >> c >> c >> c >> y;
		Gumb nagrada(x, y);

		if (del2) nagrada += 10000000000000;

		resitev.push_back(Masina(gumbA, gumbB, nagrada));

		podatki.ignore(1);
	}

	podatki.close();


	return resitev;
}


int main() {

	std::list<Masina> seznamMasin = preberiPodatke("Advent24-13.txt");

	int prim = 0;
	unsigned long long resitev = 0;
	for (Masina& masina : seznamMasin) {
		//std::cout << masina << '\n';
		//std::cout << '\n';
		//std::cout << prim << ':' << masina.racunaj1del(prim) << '|' << masina.racunaj2del(prim) << '\n';
		//std::cout << masina << '\n' << '\n';
		//if (masina.racunaj1del() != masina.racunaj2del()) {
		//	std::cout << prim << '\n';
		//	std::cout << '>' << masina.racunaj1del(prim) << '|' << masina.racunaj2del(prim) << '\n';
		//	std::cout << '>' << masina << '\n' << '\n';
		//
		//}
		resitev += masina.racunaj2del();
		prim++;
	}

	std::cout << "Vsota vseh resljivih masin je " << resitev << ".\n";
	std::cout << "\n";

	seznamMasin = preberiPodatke("Advent24-13.txt", true);

	resitev = 0;
	prim = 0;
	for (Masina& masina : seznamMasin) {
		//std::cout << masina << '\n';
		//std::cout << '\n';
		//std::cout << '\n' << prim++ << '|' << masina.racunaj2del() << '\n';
		//std::cout << masina << '\n';
		int deb = -1;
		//if (masina.racunaj2del() == 0) deb = -2;
		//else deb = -1;
		resitev += masina.racunaj2del();
		prim++;
	}

	std::cout << "Vsota vseh resljivih masin po korekciji enot je " << resitev << ".\n";


	return 0;
}
