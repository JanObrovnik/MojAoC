#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


std::istream& operator>>(std::istream& is, std::map<std::string, bool>& ma) {
	std::string str;
	bool b;
	is >> str >> b;
	str.erase(str.end() - 1);
	ma[str] = b;
	return is;
}


enum Operator {
	AND,
	OR,
	XOR
};

class LogicnaVrata {
	std::string vhod1;
	Operator op;
	std::string vhod2;
	std::string izhod;

public:
	LogicnaVrata() = default;
	LogicnaVrata(std::string vhod1, Operator op, std::string vhod2, std::string izhod) :
		vhod1(vhod1), op(op), vhod2(vhod2), izhod(izhod) {}

	std::string dobiIzhodStrig() {
		return izhod;
	}

	bool ovrednosti(std::map<std::string, bool>& ma) {

		if (ma.find(vhod1) == ma.end() || ma.find(vhod2) == ma.end())
			return 0;

		switch (op) {
		case AND:
			ma[izhod] = ma[vhod1] & ma[vhod2];
			break;

		case OR:
			ma[izhod] = ma[vhod1] | ma[vhod2];
			break;

		case XOR:
			ma[izhod] = ma[vhod1] ^ ma[vhod2];
			break;


		default:
			break;
		}

		return 1;
	}

	friend std::istream& operator>>(std::istream& is, LogicnaVrata& lv);
	friend std::ostream& operator<<(std::ostream& os, LogicnaVrata& lv);
};

std::istream& operator>>(std::istream& is, LogicnaVrata& lv) {
	std::string str;
	is >> lv.vhod1 >> str;
	if		(str == "AND") lv.op = AND;
	else if (str == "OR")  lv.op = OR;
	else if (str == "XOR") lv.op = XOR;
	is >> lv.vhod2 >> str >> lv.izhod;
	return is;
}

std::ostream& operator<<(std::ostream& os, LogicnaVrata& lv) {
	os << lv.vhod1;
	if		(lv.op == AND) os << " AND ";
	else if (lv.op == OR)  os << " OR  ";
	else if (lv.op == XOR) os << " XOR ";
	os << lv.vhod2 << " -> " << lv.izhod;
	return os;
}


void preberiPodatke(std::string pot, 
	std::map<std::string, bool>& vhodniPodatki, std::map<std::string, LogicnaVrata>& seznamLogicnihVrat) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni bila najdena.\n";
		return;
	}

	std::string vrstica;
	std::getline(podatki, vrstica);

	while (!vrstica.empty()) {

		std::stringstream ss(vrstica);
		ss >> vhodniPodatki;

		std::getline(podatki, vrstica);
	}

	LogicnaVrata log;

	while (podatki >> log)
		seznamLogicnihVrat[log.dobiIzhodStrig()] = log;

	podatki.close();
}


int main(int argc, char* argv[]) {

	std::map<std::string, bool> vhodniPodatki;
	std::map<std::string, LogicnaVrata> seznamLogicnihVrat;

	preberiPodatke("Advent24-24.txt", vhodniPodatki, seznamLogicnihVrat);


	//for (auto it = vhodniPodatki.begin(); it != vhodniPodatki.end(); it++)
	//	std::cout << it->first << ":\t" << it->second << '\n';
	//std::cout << '\n';
	//
	//for (auto it = seznamLogicnihVrat.begin(); it != seznamLogicnihVrat.end(); it++)
	//	std::cout << it->first << ":\t" << it->second << '\n';
	//std::cout << '\n';

	
	// 1 del:
skokNazaj:
	bool neuspesniIzracun(false);
	for (auto it = seznamLogicnihVrat.begin(); it != seznamLogicnihVrat.end(); it++) {
		bool izracunano(it->second.ovrednosti(vhodniPodatki));
		if (!izracunano) neuspesniIzracun = true;
		std::cout << izracunano << ' ';
	}
	std::cout << '\n' << '\n';

	if (neuspesniIzracun) goto skokNazaj;
	std::cout << '\n';


	for (auto it = vhodniPodatki.begin(); it != vhodniPodatki.end(); it++)
		if (it->first[0] == 'z')
			std::cout << it->first << ":\t" << it->second << '\n';
	std::cout << '\n';


	unsigned long long resitev = 0;
	int potenca = 0;
	for (auto it = vhodniPodatki.begin(); it != vhodniPodatki.end(); it++)
		if (it->first[0] == 'z')
			resitev += it->second * pow(2, potenca++);
	

	std::cout << "Izhod iz vrat je enak " << resitev << ".\n" << std::endl;



	// 2 del:

	unsigned long long vsotaX = 0;
	unsigned long long vsotaY = 0;
	int potencaX = 0;
	int potencaY = 0;
	for (auto it = vhodniPodatki.begin(); it != vhodniPodatki.end(); it++) {
		if (it->first[0] == 'x')
			vsotaX += it->second * pow(2, potencaX++);
		if (it->first[0] == 'y')
			vsotaY += it->second * pow(2, potencaY++);
	}

	std::cout << "X: " << vsotaX << "\tY: " << vsotaY << '\n';





	return 0;
}