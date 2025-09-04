/*
	t + 562:59 min
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <algorithm>


std::vector<std::vector<char>> zaporednjeRotacij;
void onInit() {
	zaporednjeRotacij.push_back({ 'z' });
	zaporednjeRotacij.push_back({ 'z' });
	zaporednjeRotacij.push_back({ 'z' });
	zaporednjeRotacij.push_back({ 'z','x' });
	zaporednjeRotacij.push_back({ 'y' });
	zaporednjeRotacij.push_back({ 'y' });
	zaporednjeRotacij.push_back({ 'y' });
	zaporednjeRotacij.push_back({ 'y','z' });
	zaporednjeRotacij.push_back({ 'x' });
	zaporednjeRotacij.push_back({ 'x' });
	zaporednjeRotacij.push_back({ 'x' });
	zaporednjeRotacij.push_back({ 'x','y' });
	zaporednjeRotacij.push_back({ 'z' });
	zaporednjeRotacij.push_back({ 'z' });
	zaporednjeRotacij.push_back({ 'z' });
	zaporednjeRotacij.push_back({ 'z','x' });
	zaporednjeRotacij.push_back({ 'y' });
	zaporednjeRotacij.push_back({ 'y' });
	zaporednjeRotacij.push_back({ 'y' });
	zaporednjeRotacij.push_back({ 'y','z' });
	zaporednjeRotacij.push_back({ 'x' });
	zaporednjeRotacij.push_back({ 'x' });
	zaporednjeRotacij.push_back({ 'x' });
	zaporednjeRotacij.push_back({ 'x','y' });
}


class Tocka {
public:
	short x, y, z;
	short ogID;

	Tocka() : x(0), y(0), z(0), ogID(0) {}
	Tocka(short x, short y, short z) : x(x), y(y), z(z), ogID(0) {}
	Tocka(short x, short y, short z, short ogID) : x(x), y(y), z(z), ogID(ogID) {}

	void rotiraj(const std::vector<char>& seznamRotacijskihOsi) {

		for (auto it = seznamRotacijskihOsi.begin(); it != seznamRotacijskihOsi.end(); it++) {
			const char& os = *it;
			if (os == 'z') {
				int novX = x;
				x = -y;
				y = novX;
				continue;
			}
			if (os == 'y') {
				int novZ = z;
				z = -x;
				x = novZ;
				continue;
			}
			if (os == 'x') {
				int novY = y;
				y = -z;
				z = novY;
				continue;
			}
		}
	}
};

bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y && to1.z == to2.z;
}
bool operator!=(const Tocka& to1, const Tocka& to2) {
	return !(to1 == to2);
}
bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.x != to2.x) return to1.x < to2.x;
	if (to1.y != to2.y) return to1.y < to2.y;
	return to1.z < to2.z;
}
bool operator>(const Tocka& to1, const Tocka& to2) {
	if (to1.x != to2.x) return to1.x > to2.x;
	if (to1.y != to2.y) return to1.y > to2.y;
	return to1.z > to2.z;
}

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y, to1.z + to2.z, to1.ogID);
}
Tocka operator-(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x - to2.x, to1.y - to2.y, to1.z - to2.z, to1.ogID);
}
Tocka& operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	to1.z += to2.z;
	return to1;
}
Tocka& operator-=(Tocka& to1, const Tocka& to2) {
	to1.x -= to2.x;
	to1.y -= to2.y;
	to1.z -= to2.z;
	return to1;
}
Tocka operator-(const Tocka& to) {
	return Tocka(-to.x, -to.y, -to.z, to.ogID);
}

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y << ',' << to.z;
}
std::istream& operator>>(std::istream& is, Tocka& to) {
	is >> to.x;
	is.ignore(1);
	is >> to.y;
	is.ignore(1);
	is >> to.z;
	return is;
}

class Senzor {
public:
	short id;
	std::vector<Tocka> seznamRelativnihTock;

	Senzor(short id) : id(id), seznamRelativnihTock({}) {}

	void dodajTocka(const Tocka& tocka) { seznamRelativnihTock.push_back(tocka); }

	void rotiraj(const std::vector<char>& seznamRotacijskihOsi) {

		for (Tocka& to : seznamRelativnihTock) {
			for (auto it = seznamRotacijskihOsi.begin(); it != seznamRotacijskihOsi.end(); it++) {
				const char& os = *it;
				if (os == 'z') {
					int novX = to.x;
					to.x = -to.y;
					to.y = novX;
					continue;
				}
				if (os == 'y') {
					int novZ = to.z;
					to.z = -to.x;
					to.x = novZ;
					continue;
				}
				if (os == 'x') {
					int novY = to.y;
					to.y = -to.z;
					to.z = novY;
					continue;
				}
			}
		}
	}
	void nrotiraj(const std::vector<char>& seznamRotacijskihOsi) {

		for (Tocka& to : seznamRelativnihTock) {
			for (auto itr = seznamRotacijskihOsi.rbegin(); itr != seznamRotacijskihOsi.rend(); itr++) {
				const char& os = *itr;
				if (os == 'z') {
					int novX = to.x;
					to.x = to.y;
					to.y = -novX;
					continue;
				}
				if (os == 'y') {
					int novZ = to.z;
					to.z = to.x;
					to.x = -novZ;
					continue;
				}
				if (os == 'x') {
					int novY = to.y;
					to.y = to.z;
					to.z = -novY;
					continue;
				}
			}
		}
	}
};

struct Soleznost {
	short id1;
	short id2;
	Tocka zamik;
	std::pair<short, short> korakRotacije;

	Soleznost(short id1, short id2, Tocka zamik) : id1(id1), id2(id2), zamik(zamik), korakRotacije({ 0,0 }) {}
	Soleznost(short id1, short id2, Tocka zamik, std::pair<short, short> korakRotacije) : id1(id1), id2(id2), zamik(zamik), korakRotacije(korakRotacije) {}
};

Soleznost operator-(const Soleznost& so) {
	Soleznost nov(so.id2, so.id1, -so.zamik, { so.korakRotacije.second+1,23 });
	for (int korak = so.korakRotacije.first; korak <= so.korakRotacije.second; korak++) {
		nov.zamik.rotiraj(zaporednjeRotacij[korak]);
	}
	return nov;
}

void onInit2(std::vector<Soleznost>& seznamSoleznosti) {
	seznamSoleznosti.push_back(Soleznost(0, 2, Tocka(62, 83, 1085), { 0, 16 }));
	seznamSoleznosti.push_back(Soleznost(0, 3, Tocka(85, 1284, -8), { 0, 8 }));
	seznamSoleznosti.push_back(Soleznost(0, 22, Tocka(-1120, 73, -57), { 0, 5 }));
	seznamSoleznosti.push_back(Soleznost(0, 32, Tocka(68, 72, -1227), { 0, 2 }));
	seznamSoleznosti.push_back(Soleznost(1, 7, Tocka(-1163, 108, 65), { 0, 18 }));
	seznamSoleznosti.push_back(Soleznost(1, 11, Tocka(-9, 12, -1224), { 0, 1 }));
	seznamSoleznosti.push_back(Soleznost(1, 17, Tocka(1283, -2, -95), { 0, 5 }));
	seznamSoleznosti.push_back(Soleznost(1, 33, Tocka(139, -1088, 33), { 0, 10 }));
	seznamSoleznosti.push_back(Soleznost(1, 34, Tocka(141, 1191, 33), { 0, 16 }));
	seznamSoleznosti.push_back(Soleznost(2, 4, Tocka(1221, 42, -57), { 0, 17 }));
	seznamSoleznosti.push_back(Soleznost(2, 13, Tocka(-73, -39, -1224), { 0, 11 }));
	seznamSoleznosti.push_back(Soleznost(4, 29, Tocka(1292, -16, -144), { 0, 2 }));
	seznamSoleznosti.push_back(Soleznost(5, 26, Tocka(117, -128, -1166), { 0, 15 }));
	seznamSoleznosti.push_back(Soleznost(6, 19, Tocka(-9, -1101, 35), { 0, 3 }));
	seznamSoleznosti.push_back(Soleznost(7, 19, Tocka(26, -102, -1152), { 0, 19 }));
	seznamSoleznosti.push_back(Soleznost(8, 9, Tocka(99, 119, -1237), { 0, 17 }));
	seznamSoleznosti.push_back(Soleznost(8, 13, Tocka(83, -1151, -151), { 0, 20 }));
	seznamSoleznosti.push_back(Soleznost(8, 18, Tocka(108, 1330, -132), { 0, 4 }));
	seznamSoleznosti.push_back(Soleznost(8, 21, Tocka(44, 87, 1145), { 0, 2 }));
	seznamSoleznosti.push_back(Soleznost(9, 11, Tocka(19, -1179, -137), { 0, 9 }));
	seznamSoleznosti.push_back(Soleznost(9, 17, Tocka(1148, 113, -151), { 0, 13 }));
	seznamSoleznosti.push_back(Soleznost(9, 30, Tocka(66, -12, -1310), { 0, 7 }));
	seznamSoleznosti.push_back(Soleznost(10, 19, Tocka(-89, -154, -1171), { 0, 9 }));
	seznamSoleznosti.push_back(Soleznost(12, 15, Tocka(66, -57, -1234), { 0, 6 }));
	seznamSoleznosti.push_back(Soleznost(12, 25, Tocka(1162, -39, -101), { 0, 12 }));
	seznamSoleznosti.push_back(Soleznost(13, 16, Tocka(23, -2, 1200), { 0, 0 }));
	seznamSoleznosti.push_back(Soleznost(13, 29, Tocka(-63, 1310, 125), { 0, 7 }));
	seznamSoleznosti.push_back(Soleznost(13, 30, Tocka(40, -1152, -28), { 0, 6 }));
	seznamSoleznosti.push_back(Soleznost(14, 26, Tocka(52, -55, -1043), { 0, 13 }));
	seznamSoleznosti.push_back(Soleznost(15, 20, Tocka(15, -11, 1146), { 0, 0 }));
	seznamSoleznosti.push_back(Soleznost(15, 23, Tocka(-11, 1191, -61), { 0, 21 }));
	seznamSoleznosti.push_back(Soleznost(15, 31, Tocka(26, -1085, 0), { 0, 7 }));
	seznamSoleznosti.push_back(Soleznost(17, 24, Tocka(13, -32, 1056), { 0, 7 }));
	seznamSoleznosti.push_back(Soleznost(18, 35, Tocka(-1190, -172, 11), { 0, 18 }));
	seznamSoleznosti.push_back(Soleznost(19, 33, Tocka(-44, 70, 1328), { 0, 2 }));
	seznamSoleznosti.push_back(Soleznost(20, 26, Tocka(1390, 23, 93), { 0, 19 }));
	seznamSoleznosti.push_back(Soleznost(21, 29, Tocka(1175, -86, 14), { 0, 8 }));
	seznamSoleznosti.push_back(Soleznost(21, 35, Tocka(-1232, -108, -87), { 0, 15 }));
	seznamSoleznosti.push_back(Soleznost(22, 28, Tocka(44, 1241, -2), { 0, 5 }));
	seznamSoleznosti.push_back(Soleznost(22, 31, Tocka(1243, 38, 87), { 0, 16 }));
	seznamSoleznosti.push_back(Soleznost(23, 26, Tocka(-1300, -188, -3), { 0, 18 }));
	seznamSoleznosti.push_back(Soleznost(24, 30, Tocka(1114, 103, 112), { 0, 21 }));
	seznamSoleznosti.push_back(Soleznost(24, 33, Tocka(-96, 30, 1131), { 0, 14 }));
	seznamSoleznosti.push_back(Soleznost(25, 28, Tocka(-1188, -107, -96), { 0, 1 }));
	seznamSoleznosti.push_back(Soleznost(25, 31, Tocka(11, -18, 1107), { 0, 14 }));
	seznamSoleznosti.push_back(Soleznost(27, 28, Tocka(1242, 118, 99), { 0, 10 }));
	seznamSoleznosti.push_back(Soleznost(28, 32, Tocka(1232, -3, 71), { 0, 2 }));
	seznamSoleznosti.push_back(Soleznost(31, 33, Tocka(10, -80, -1252), { 0, 5 }));
}

Tocka preberiTocko(const std::string& vrstica) {

	Tocka resitev;

	std::stringstream ss(vrstica);
	ss >> resitev;

	return resitev;
}

std::list<Senzor> preberiPodatke(const std::string& pot) {

	std::list<Senzor> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	std::getline(podatki, vrstica);
	resitev.push_back(Senzor(0));

	while (podatki.peek() != EOF) {

		std::getline(podatki, vrstica);

		if (vrstica.empty()) {
			std::getline(podatki, vrstica);
			resitev.push_back(Senzor(resitev.size()));
			continue;
		}

		resitev.back().dodajTocka(preberiTocko(vrstica));
		resitev.back().seznamRelativnihTock.back().ogID = resitev.size() - 1;
	}

	podatki.close();

	return resitev;
}


bool zamakniSenzor(const Senzor& senzor1, const Senzor& senzor2, const Tocka& razlika,
				   std::vector<Soleznost>& zapisSoleznosti) {

	short stevilo = 0;

	for (const Tocka& tocka : senzor1.seznamRelativnihTock) {
		
		Tocka zamaknjenaTocka = tocka + razlika;
		
		if (std::find(senzor2.seznamRelativnihTock.begin(), senzor2.seznamRelativnihTock.end(), zamaknjenaTocka) != senzor2.seznamRelativnihTock.end())
			stevilo++;
		else continue;

		if (stevilo == 12) {
			zapisSoleznosti.push_back(Soleznost(senzor1.id, senzor2.id, -razlika));
			return true;
		}
	}

	return false;
}

bool primerjajParSenzorjev(const Senzor& senzor1, const Senzor& senzor2,
						   std::vector<Soleznost>& zapisSoleznosti) {

	for (const Tocka& tocka1 : senzor1.seznamRelativnihTock) {
		for (const Tocka& tocka2 : senzor2.seznamRelativnihTock) {

			const Tocka& razlika = tocka2 - tocka1;

			if (zamakniSenzor(senzor1, senzor2, razlika, zapisSoleznosti)) {
				std::cout << senzor1.id << '-' << senzor2.id << '\n';
				return true;
			}
		}
	}

	return false;
}

void isci(const std::list<Senzor>& seznamSenzorjev,
		  std::vector<Soleznost>& zapisSoleznosti) {

	for (std::list<Senzor>::const_iterator it1 = seznamSenzorjev.begin(); it1 != seznamSenzorjev.end(); it1++) {
		for (std::list<Senzor>::const_iterator  it2 = it1; it2 != seznamSenzorjev.end(); it2++) {

			if (it1 == it2)
				continue;

			const Senzor& senzor1 = *it1;
			Senzor senzor2 = *it2;

			//for (const std::vector<char>& rotacije : zaporednjeRotacij) {
			for (int korak = 0; korak < zaporednjeRotacij.size(); korak++) {

				senzor2.rotiraj(zaporednjeRotacij[korak]);

				if (primerjajParSenzorjev(senzor1, senzor2, zapisSoleznosti)) {
					zapisSoleznosti.back().korakRotacije = { 0,korak };
					break;
				}
			}
		}
	}
}


std::vector<Soleznost> postaviVDrevesnoStrukturo(std::vector<Soleznost> seznamSoleznosti) {

	std::vector<Soleznost> resitev;

	std::queue<short> cakalnaVrsta;
	cakalnaVrsta.push(0);

	while (!cakalnaVrsta.empty()) {
		for (auto it = seznamSoleznosti.begin(); it != seznamSoleznosti.end(); it++) {

			if (it->zamik == Tocka(0, 0, 0))
				continue;

			if (it->id1 == cakalnaVrsta.front()) {
				
				resitev.push_back(*it);
				
				cakalnaVrsta.push(it->id2);
				it->zamik = Tocka(0, 0, 0);
			}
			else if (it->id2 == cakalnaVrsta.front()) {
				
				resitev.push_back(-*it);
				
				cakalnaVrsta.push(it->id1);
				it->zamik = Tocka(0, 0, 0);
			}
		}
		cakalnaVrsta.pop();
	}

	return resitev;
}

std::vector<Soleznost> postaviVGlobalniKS(std::vector<Soleznost> seznamSoleznosti, std::vector<Senzor> seznamSenzorjev) {

	std::vector<Soleznost> resitev;

	////////////////////////////////////////
	

	for (auto itr = seznamSoleznosti.rbegin(); itr != seznamSoleznosti.rend(); itr++) {

		// op 1.2:
		for (int korak = itr->korakRotacije.first; korak <= itr->korakRotacije.second; korak++)
			seznamSenzorjev[itr->id2].rotiraj(zaporednjeRotacij[korak]);

		for (Tocka tocka : seznamSenzorjev[itr->id2].seznamRelativnihTock)
			seznamSenzorjev[itr->id1].seznamRelativnihTock.push_back(tocka + itr->zamik);
	}

	std::set<Tocka> vmesnik(seznamSenzorjev[0].seznamRelativnihTock.begin(), seznamSenzorjev[0].seznamRelativnihTock.end());

	for (const Tocka& tocka : vmesnik)
		std::cout << tocka << " (" << tocka.ogID << ")\n";

	std::cout << "Resitev je " << vmesnik.size() << '\n';


	return resitev;
}


void natisniSeznamSenzorjev(const std::list<Senzor>& seznamSenzorjev) {
	for (const Senzor& senzor : seznamSenzorjev) {
		std::cout << "\nSenzor " << senzor.id << ": velikosti " << senzor.seznamRelativnihTock.size() << '\n';
		for (const Tocka& tocka : senzor.seznamRelativnihTock)
			std::cout << tocka << '\n';
	}
}

int main() {

	onInit();


	std::list<Senzor> seznamSenzorjev(preberiPodatke("2021/19.txt"));
	std::vector<Senzor> kopijaSenzorjev(seznamSenzorjev.begin(),seznamSenzorjev.end());
	//natisniSeznamSenzorjev(seznamSenzorjev);
	

	std::vector<Soleznost> seznamSoleznosti;
	//onInit2(seznamSoleznosti);
	if (seznamSoleznosti.empty())
		isci(seznamSenzorjev, seznamSoleznosti);

	std::cout << std::endl;
	for (Soleznost& soleznost : seznamSoleznosti) {
		std::cout << soleznost.id1 << '-' << soleznost.id2 << ':' << soleznost.zamik  << '|' << soleznost.korakRotacije.first << ',' << soleznost.korakRotacije.second << '\n';
	} std::cout << std::endl;


	std::vector<Soleznost> urejenSeznamSoleznosti1 = postaviVDrevesnoStrukturo(seznamSoleznosti);
	std::cout << std::endl;
	for (const Soleznost& soleznost : urejenSeznamSoleznosti1) {
		std::cout << soleznost.id1 << '-' << soleznost.id2 << ':' << soleznost.zamik << '|' << soleznost.korakRotacije.first << ',' << soleznost.korakRotacije.second << '\n';
	} std::cout << std::endl;

	std::vector<Soleznost> urejenSeznamSoleznosti2 = postaviVGlobalniKS(urejenSeznamSoleznosti1, kopijaSenzorjev);
	std::cout << std::endl;
	for (const Soleznost& soleznost : urejenSeznamSoleznosti2) {
		std::cout << soleznost.id1 << '-' << soleznost.id2 << ':' << soleznost.zamik << '|' << soleznost.korakRotacije.first << ',' << soleznost.korakRotacije.second << '\n';
	} std::cout << std::endl;



	return 0;
}

// 1252 too high
// 1015 too high