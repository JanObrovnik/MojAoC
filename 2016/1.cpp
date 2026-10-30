/*
	t = 16:08 min
	nekoliko lahko
	t = 23:19 min
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>


enum Smer {
	SEVER,
	VZHOD,
	JUG,
	ZAHOD,
	size
};

Smer& operator++(Smer& sm) {
	int i = (int)sm + 1;
	if (i == size) sm = SEVER;
	else sm = (Smer)i;
	return sm;
}
Smer& operator--(Smer& sm) {
	int i = (int)sm - 1;
	if (i == -1) sm = ZAHOD;
	else sm = (Smer)i;
	return sm;
}

class Koordinate {
	int x, y;
	Smer smer;

public:
	Koordinate() : x(0), y(0), smer(SEVER) {}

	void korak(const char& obrnitev, const int& kolicina) {
		
		if		(obrnitev == 'R') ++smer;
		else if (obrnitev == 'L') --smer;

		if		(smer == SEVER) y += kolicina;
		else if (smer == VZHOD) x += kolicina;
		else if (smer == JUG)   y -= kolicina;
		else if (smer == ZAHOD) x -= kolicina;
	}
	bool korakPostopno(const char& obrnitev, const int& kolicina, std::set<Koordinate>& seznamKoordinat) {

		if		(obrnitev == 'R') ++smer;
		else if (obrnitev == 'L') --smer;

		if		(smer == SEVER) for (int i = 0; i < kolicina; i++) { y++; if (seznamKoordinat.find(*this) != seznamKoordinat.end()) return true; seznamKoordinat.insert(*this); }
		else if (smer == VZHOD) for (int i = 0; i < kolicina; i++) { x++; if (seznamKoordinat.find(*this) != seznamKoordinat.end()) return true; seznamKoordinat.insert(*this); }
		else if (smer == JUG)   for (int i = 0; i < kolicina; i++) { y--; if (seznamKoordinat.find(*this) != seznamKoordinat.end()) return true; seznamKoordinat.insert(*this); }
		else if (smer == ZAHOD) for (int i = 0; i < kolicina; i++) { x--; if (seznamKoordinat.find(*this) != seznamKoordinat.end()) return true; seznamKoordinat.insert(*this); }

		return false;
	}

	int manhatenRazdalja() const {
		return abs(x) + abs(y);
	}

	void print() const {
		std::cout << x << ", " << y << '\n';
	}

	friend bool operator==(const Koordinate& ko1, const Koordinate& ko2);
	friend bool operator<(const Koordinate& ko1, const Koordinate& ko2);
};

bool operator==(const Koordinate& ko1, const Koordinate& ko2) {
	return (ko1.x == ko2.x && ko1.y == ko2.y);
}
bool operator<(const Koordinate& ko1, const Koordinate& ko2) {
	if (ko1.y != ko2.y) return ko1.y < ko2.y;
	return ko1.x < ko2.x;
}


std::pair<int, int> preberiPodatke(const std::string& pot) {

	int resitev1 = -1;
	int resitev2 = -1;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	Koordinate koordinate;
	Koordinate koordinate2;
	char obrnitev;
	int kolicina;

	std::set<Koordinate> seznamKoordinat;
	seznamKoordinat.insert(koordinate);

	while (podatki.peek() != EOF) {

		podatki >> obrnitev >> kolicina;
		podatki.ignore(1);

		koordinate.korak(obrnitev, kolicina);

		//koordinate.print();

		if (resitev2 == -1) {
			if (koordinate2.korakPostopno(obrnitev, kolicina, seznamKoordinat)) {
				resitev2 = koordinate2.manhatenRazdalja();
			}
		}
	}

	podatki.close();

	resitev1 = koordinate.manhatenRazdalja();

	return { resitev1,resitev2 };
}


int main() {

	std::pair<int, int> resitev = preberiPodatke("2016/1.txt");
	
	std::cout << "Oddaljeni smo za " << resitev.first << " blokov.\n";
	std::cout << "V resnici smo oddaljeni za " << resitev.second << " blokov.\n";


	return 0;
}
