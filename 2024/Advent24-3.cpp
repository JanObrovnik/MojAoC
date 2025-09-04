/*
	zelo tezko (ugotovit mors stt::noskipws)
	neverjetno enostavno
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>


class Mnozi {
public:
	int st1, st2;

	const static std::string naziv;

	Mnozi() : st1(0), st2(0) {}
	Mnozi(short st1, short st2) : st1(st1), st2(st2) {}

	int resitev() const {
		return st1 * st2;
	}

	friend std::ostream& operator<<(std::ostream& os, Mnozi mn);
	//friend bool operator==(Mnozi mn1, Mnozi2 mn);
};
const std::string Mnozi::naziv = "mul";

std::ostream& operator<<(std::ostream& os, Mnozi mn) {
	return os << '(' << mn.st1 << ',' << mn.st2 << ')';
}


/// <summary>
/// 
/// </summary>
class Mnozi2 {
public:
	int st1, st2;

	const static std::string naziv;

	Mnozi2() : st1(0), st2(0) {}
	Mnozi2(short st1, short st2) : st1(st1), st2(st2) {}

	int resitev() const {
		return st1 * st2;
	}

	friend std::ostream& operator<<(std::ostream& os, Mnozi2& mn);
	//friend bool operator==(Mnozi mn1, Mnozi2 mn);
};
const std::string Mnozi2::naziv = "mul";

std::ostream& operator<<(std::ostream& os, Mnozi2& mn) {
	return os << "mul(" << mn.st1 << ',' << mn.st2 << ')';
}

std::istream& operator>>(std::istream& is, Mnozi2& mn) {
	char c;
	int i1, i2;
	is >> c >> c >> c >> c;
	is >> i1 >> c >> i2 >> c;
	mn = Mnozi2(i1, i2);
	return is;
}

//bool operator==(Mnozi mn1, Mnozi2 mn2) {
//	return mn2.st1 == mn1.st1 && mn2.st2 == mn1.st1;
//}
//
//bool operator==(Mnozi2 mn1, Mnozi mn2) {
//	return mn2 == mn1;
//}


std::list<Mnozi2> prebiriPodatke2(std::string pot) {

	std::list<Mnozi2> resitev{};

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena\n";
		return resitev;
	}

	while (podatki.peek() != EOF) {

		Mnozi2 mnozi;

		podatki >> mnozi;

		resitev.push_back(mnozi);
	}

	podatki.close();


	return resitev;
}
/// <summary>
/// 
/// </summary>


std::list<Mnozi> prebiriPodatke(std::string pot) {

	std::list<Mnozi> resitev{};

	std::fstream podatki;
	podatki.open(pot, std::ios::in);
	podatki >> std::noskipws; // Vzel pol dneva - brez tega ne najde 'mul(47,151)'

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena\n";
		return resitev;
	}

	char koda;

	while (podatki.peek() != EOF) {

		podatki >> koda;


		bool pravilnost = true;
		for (char c : Mnozi::naziv) {

			if (koda == c) { podatki >> koda; continue; }
			else { pravilnost = false; break; }
		}

		if (!pravilnost) continue;

		if (koda == '(') podatki >> koda;
		else continue;


		std::string stringSt1 = "", stringSt2 = "";

		for (int i = 0; i < 3; i++) {
			if (i == 0) {
				if (!(koda >= '0' && koda <= '9')) { pravilnost = false; break; }
				stringSt1.push_back(koda);
				podatki >> koda;
			}
			else {
				if (koda == ',') { podatki >> koda; break; }
				else if (koda >= '0' && koda <= '9') { stringSt1.push_back(koda); podatki >> koda; }
				else { pravilnost = false; break; }
			}
		}

		if (!pravilnost) continue;

		if (koda == ',') podatki >> koda; // mankalo

		for (int i = 0; i < 3; i++) {
			if (i == 0) {
				if (!(koda >= '0' && koda <= '9')) { pravilnost = false; break; }
				stringSt2.push_back(koda);
				podatki >> koda;
			}
			else {
				if (koda == ')') { break; } // mankalo
				else if (koda >= '0' && koda <= '9') { stringSt2.push_back(koda); podatki >> koda; }
				else { pravilnost = false; break; }
			}
		}

		if (!pravilnost) continue;


		if (koda != ')') continue;
		else {
			short st1 = std::stoi(stringSt1);
			short st2 = std::stoi(stringSt2);

			resitev.push_back(Mnozi(st1, st2));
		}
	}

	podatki.close();


	return resitev;
}



std::list<Mnozi> prebiriPodatke2del(std::string pot) {

	std::list<Mnozi> resitev{};

	std::fstream podatki;
	podatki.open(pot, std::ios::in);
	podatki >> std::noskipws; // Vzel pol dneva - brez tega ne najde 'mul(47,151)'

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena\n";
		return resitev;
	}

	char koda;

	bool pisi = true;
	std::string seznamZadnjihUkazov = "xxxxxxx";

	while (podatki.peek() != EOF) {

		podatki >> koda;

		
		seznamZadnjihUkazov.erase(seznamZadnjihUkazov.begin());
		seznamZadnjihUkazov.push_back(koda);

		if (seznamZadnjihUkazov == "don't()") {
			pisi = false;
			//std::cout << seznamZadnjihUkazov << '\n';
		}
		std::string seznamZadnjihZadnjihUkazov(seznamZadnjihUkazov.begin() + 3, seznamZadnjihUkazov.end());
		if (seznamZadnjihZadnjihUkazov == "do()") {
			pisi = true;
			//std::cout << seznamZadnjihZadnjihUkazov << '\n';
		}


		bool pravilnost = true;
		for (char c : Mnozi::naziv) {

			if (koda == c) { podatki >> koda; continue; }
			else { pravilnost = false; break; }
		}

		if (!pravilnost) continue;

		if (koda == '(') podatki >> koda;
		else continue;


		std::string stringSt1 = "", stringSt2 = "";

		for (int i = 0; i < 3; i++) {
			if (i == 0) {
				if (!(koda >= '0' && koda <= '9')) { pravilnost = false; break; }
				stringSt1.push_back(koda);
				podatki >> koda;
			}
			else {
				if (koda == ',') { podatki >> koda; break; }
				else if (koda >= '0' && koda <= '9') { stringSt1.push_back(koda); podatki >> koda; }
				else { pravilnost = false; break; }
			}
		}

		if (!pravilnost) continue;

		if (koda == ',') podatki >> koda; // mankalo

		for (int i = 0; i < 3; i++) {
			if (i == 0) {
				if (!(koda >= '0' && koda <= '9')) { pravilnost = false; break; }
				stringSt2.push_back(koda);
				podatki >> koda;
			}
			else {
				if (koda == ')') { break; } // mankalo
				else if (koda >= '0' && koda <= '9') { stringSt2.push_back(koda); podatki >> koda; }
				else { pravilnost = false; break; }
			}
		}

		if (!pravilnost) continue;


		if (koda != ')') continue;
		else {
			if (pisi) {
				short st1 = std::stoi(stringSt1);
				short st2 = std::stoi(stringSt2);

				resitev.push_back(Mnozi(st1, st2));
			}
		}
	}

	podatki.close();


	return resitev;
}


void test(std::string pot);
void test2(std::string pot);
void test3(std::string pot);

int main() {
	int i = 0;
	std::list<Mnozi> seznamUkazov = prebiriPodatke("Advent24-3.txt");
	//std::for_each(seznamUkazov.begin(), seznamUkazov.end(), [&](Mnozi ukaz) {std::cout << ++i << ':' << Mnozi::naziv << ukaz << '\n'; });

	unsigned long long resitev = 0;
	std::for_each(seznamUkazov.begin(), seznamUkazov.end(), [&resitev](Mnozi ukaz) { resitev += ukaz.resitev(); });

	std::cout << "Vsota vseh ukazov je " << resitev << ".\n";


	/// test
	//test("Advent24-3.txt");
	//test2("test.txt");
	//test3("test2.txt");
	//
	//std::list<Mnozi2> seznam = prebiriPodatke2("test3.txt");
	//seznam.erase(--seznam.end());
	////std::for_each(seznam.begin(), seznam.end(), [](Mnozi2 ukaz) {std::cout << ukaz << '\n'; });
	//
	//unsigned long long resitev2 = 0;
	//std::for_each(seznam.begin(), seznam.end(), [&resitev2](Mnozi2 ukaz) { resitev2 += ukaz.resitev(); });
	//
	//std::cout << "Vsota vseh ukazov je " << resitev2 << ".\n";
	/////
	//
	//
	//std::cout << "-----------------------\n";
	//for (Mnozi ukaz : seznamUkazov) {
	//	
	//	auto it = std::find_if(seznam.begin(), seznam.end(), [&ukaz](Mnozi2 ukaz2) {return ukaz.st1 == ukaz2.st1 && ukaz.st2 == ukaz2.st2; });
	//
	//	if (it == seznam.end())
	//		std::cout << ukaz << '\n';
	//}
	//std::cout << "-----------------------\n";
	//for (Mnozi2 ukaz : seznam) {
	//
	//	auto it = std::find_if(seznamUkazov.begin(), seznamUkazov.end(), [&ukaz](Mnozi ukaz2) {return ukaz.st1 == ukaz2.st1 && ukaz.st2 == ukaz2.st2; });
	//
	//	if (it == seznamUkazov.end())
	//		std::cout << ukaz << '\n';
	//}
	//std::cout << "-----------------------\n";

	//std::for_each(seznamUkazov.begin(), seznamUkazov.end(), [&](Mnozi ukaz) {std::cout << ++i << ':' << Mnozi::naziv << ukaz << '\n'; });


	/// 2 del
	std::list<Mnozi> seznamUkazov2del = prebiriPodatke2del("Advent24-3.txt");

	unsigned long long resitev2del = 0;
	std::for_each(seznamUkazov2del.begin(), seznamUkazov2del.end(), [&resitev2del](Mnozi ukaz) { resitev2del += ukaz.resitev(); });

	std::cout << "Vsota vseh ukazov 2. dela je " << resitev2del << ".\n";




	return 0;
}

void test(std::string pot) {

	std::fstream input;
	input.open(pot, std::ios::in);

	std::fstream output;
	output.open("test.txt", std::ios::out);

	char c;

	if (!input.is_open() || !output.is_open()) {
		std::cout << "testa ni mogoce narediti\n";
		return;
	}

	while (input.peek() != EOF) {

		input >> c;
		bool pravi = false;
		for (char ch : {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'm', 'u', 'l', '(', ')', ',' }) {
			if (c == ch) { pravi = true; break; }
		}

		if (!pravi) continue;

		//if (c == 'm') std::cout << '\n';
		//std::cout << c;
		
		if (c == 'm') output << '\n';
		output << c;
	}

	input.close();
	output.close();
}

void test2(std::string pot) {

	std::fstream input;
	input.open(pot, std::ios::in);

	std::fstream output;
	output.open("test2.txt", std::ios::out);


	std::string vrstica;

	while (std::getline(input, vrstica)) {

		if (vrstica.size() < 8)
			continue;

		if (vrstica[1] != 'u' || vrstica[2] != 'l' || vrstica[3] != '(')
			continue;

		auto it = std::find(vrstica.begin(), vrstica.end(), ')');

		if (it != vrstica.end())
			vrstica.erase(++it, vrstica.end());


		output << vrstica << '\n';
	}


	input.close();
	output.close();
}

void test3(std::string pot) {

	std::fstream input;
	input.open(pot, std::ios::in);

	std::fstream output;
	output.open("test3.txt", std::ios::out);


	std::string vrstica;

	int i = 0;
	while (std::getline(input, vrstica)) {

		std::stringstream ss(vrstica);

		char c;
		int i1 = 0, i2 = 0;

		ss >> c >> c >> c >> c;
		ss >> i1 >> c >> i2 >> c;


		if (c != ')')
			continue;

		if (i1 > 0 && i2 > 0) {
			output << vrstica << '\n';

			//i++;
			//std::cout << i << ':' << i1 << '|' << i2 << '\n';
		}
	}


	input.close();
	output.close();
}