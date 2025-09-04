/*
	precej lahka (zlo strojniska)
	zlo cudna, ampak lustna
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>



class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}
};

std::istream& operator>>(std::istream& is, Tocka& to) {
	char c;
	is >> c >> c >> to.x >> c >> to.y;
	return is;
}

std::ostream& operator<<(std::ostream& os, Tocka& to) {
	return os << '(' << to.x << ',' << to.y << ')';
}

Tocka operator+(Tocka& to1, Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}

Tocka& operator+=(Tocka& to1, Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	return to1;
}



class Robot {
public:
	Tocka pozicija;
	Tocka hitrost;

	Robot() : pozicija(), hitrost() {}
	Robot(Tocka pozicija, Tocka hitrost) : pozicija(pozicija), hitrost(hitrost) {}

	void pomik() {
		pozicija += hitrost;
	}
};

std::istream& operator>>(std::istream& is, Robot& ro) {
	is >> ro.pozicija >> ro.hitrost;
	return is;
}

std::ostream& operator<<(std::ostream& os, Robot& ro) {
	return os << "p=" << ro.pozicija << " v=" << ro.hitrost;
}


class EBHQ {
	int sirina, visina;
	std::list<Robot> seznamRobotov;

public:
	EBHQ() : sirina(101), visina(103), seznamRobotov({}) {}
	//EBHQ() : sirina(11), visina(7), seznamRobotov({}) {} // test


	void dodajRobota(Robot novRobot) {
		seznamRobotov.push_back(novRobot);
	}


	void simuliraj(int cas) {
		//int pon = 0;
		//std::cout << "-----------------------------------ZACETEK-----------------------------------\n";
		for (int t = 0; t < cas; t++) {
			//if (pon >= cas - 80) {
			//	std::cout << "Ponovitev: " << pon << '\n';
			//	prikaz();
			//}
			//pon++;
			for (Robot& robot : seznamRobotov) {
				robot.pomik();
				if (robot.pozicija.x < 0) robot.pozicija.x += sirina;
				else if (robot.pozicija.x >= sirina) robot.pozicija.x -= sirina;
				if (robot.pozicija.y < 0) robot.pozicija.y += visina;
				else if (robot.pozicija.y >= visina) robot.pozicija.y -= visina;
			}
		}
	}

	int vrniResitev() const {

		int kvadrantSZ = 0;
		int kvadrantSV = 0;
		int kvadrantJV = 0;
		int kvadrantJZ = 0;

		for (Robot robot : seznamRobotov) {
			Tocka& poz = robot.pozicija;
			int sredinaSirine = (sirina - 1) / 2;
			int sredinaVisine = (visina - 1) / 2;

			if		(poz.x < sredinaSirine && poz.y < sredinaVisine) kvadrantSZ++;
			else if (poz.x > sredinaSirine && poz.y < sredinaVisine) kvadrantSV++;
			else if (poz.x > sredinaSirine && poz.y > sredinaVisine) kvadrantJV++;
			else if (poz.x < sredinaSirine && poz.y > sredinaVisine) kvadrantJZ++;
		}

		//std::cout << kvadrantJV << ' ' << kvadrantJZ << ' ' << kvadrantSV << ' ' << kvadrantSZ << '\n';
		int resitev = kvadrantJV * kvadrantJZ * kvadrantSV * kvadrantSZ;

		return resitev;
	}

	void prikaz() const {
		std::vector<std::string> mapa(visina, std::string(sirina, '0'));

		for (Robot robot : seznamRobotov) mapa[robot.pozicija.y][robot.pozicija.x]++;
		
		for (std::string& vrsta : mapa)
			for (char& c : vrsta)
				if (c == '0')
					c = '.';

		for (auto& vrst : mapa) std::cout << vrst << '\n'; std::cout << '\n';
	}
};


void preberiPodatke(std::string pot, EBHQ& ebhq) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni bila najdena.\n";
		return;
	}

	Robot robot;
	while (podatki >> robot)
		ebhq.dodajRobota(robot);

	podatki.close();
}


int main() {

	EBHQ wc;
	preberiPodatke("Advent24-14.txt", wc);

	// 80 160 240 320 400 480 560 1200 1300 6450
	wc.simuliraj(100); // 80 na stran
	//wc.prikaz();

	std::cout << "Po 100 s je porazdelitev robotv " << wc.vrniResitev() << ".\n";


	/*
		Prikaz smrekice:
		* 60 163 266 369 472 575 678 781 884 987 1090 1193 1296
		* 83 184 285 386 487 588 689 790 891 992 1093 1194 1295
	*/

	int vodoravno = 60;
	int vodoravniKorak = 103;
	std::set<int> seznamVodoravnih;

	int navpicno = 83;
	int navpicniKorak = 101;

	int trenutnoVodoravno = vodoravno;
	int trenutnoNavpicno = navpicno;

	while (seznamVodoravnih.find(trenutnoNavpicno) == seznamVodoravnih.end()) {
		seznamVodoravnih.insert(trenutnoVodoravno);

		trenutnoVodoravno += vodoravniKorak;
		trenutnoNavpicno += navpicniKorak;
	}
	std::cout << "Stevilo sekund do easter egg-a (smrekice): " << trenutnoNavpicno << '\n';


	return 0;
}