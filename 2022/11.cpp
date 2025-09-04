/*
	Nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


enum class Operator {
	SESTEVANJE,
	MNOZENJE,
	POTENCA
};

class Opica {
private:
	int ID;
	std::vector<int> seznamElementov;
	std::pair<Operator, int> operacija;
	int test, trueOpica, falseOpica;
	int steviloInspekcij;


public:
	Opica(int ID, std::vector<int> seznamElementov, std::pair<Operator, int> operacija, int test, int trueOpica, int falseOpica) :
		ID(ID), seznamElementov(seznamElementov), operacija(operacija), test(test), trueOpica(trueOpica), falseOpica(falseOpica),
		steviloInspekcij(0) {}


	std::pair<int,int> vrziElementOpici(int delitelj = 3) {
		steviloInspekcij++;
		int element(seznamElementov.front());
		seznamElementov.erase(seznamElementov.begin());
		if		(operacija.first == Operator::SESTEVANJE) element += operacija.second;
		else if (operacija.first == Operator::MNOZENJE)   element *= operacija.second;
		else if (operacija.first == Operator::POTENCA)    element = pow(element, operacija.second);
		element /= delitelj;
		int drugID((element % test == 0) ? trueOpica : falseOpica);
		return { drugID,element };
	}


	int steviloElementov() const {
		return seznamElementov.size();
	}

	void dodajElement(const int& element) {
		seznamElementov.push_back(element);
	}

	int vrniSteviloInspekcij() const {
		return steviloInspekcij;
	}


	friend std::ostream& operator<<(std::ostream& os, const Opica& op);
};

std::ostream& operator<<(std::ostream& os, const Opica& op) {
	os << "Opica " << op.ID << ": ";
	for (const int& element : op.seznamElementov) {
		os << element;
		if (element != op.seznamElementov.back()) os << ", ";
	}
	return os;
}


std::vector<Opica> preberiPodatke(std::string pot) {

	std::vector<Opica> resitev{};


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	while (podatki.peek() != EOF) {

		std::string str;
		int i;
		char c;

		std::vector<int> seznam;
		int ID;

		podatki >> str >> ID;
		//std::cout << ID << '\n'; ////////////////
		std::getline(podatki, str);
		std::getline(podatki, str);
		std::stringstream ss(str);
		
		//std::cout << str << '\n'; ////////////////


		ss >> str >> str;
		while (ss.peek() != EOF) {
			ss >> i;
			//std::cout << i << '\n'; ////////////////

			seznam.push_back(i);
			ss >> c;
		}

		podatki >> str >> str >> str >> str;
		podatki >> c >> str;
		//std::cout << c << ' '; ////////////////
		//std::cout << str << '\n'; ////////////////


		std::pair<Operator, int> par;
		if (str == "old") {
			if		(c == '+') par = { Operator::MNOZENJE, 2 };
			else if (c == '*') par = { Operator::POTENCA,  2 };
		}
		else {
			if		(c == '+') par = { Operator::SESTEVANJE, std::stoi(str) };
			else if (c == '*') par = { Operator::MNOZENJE,   std::stoi(str) };
		}

		int test, trueOpica, falseOpica;
		podatki >> str >> str >> str >> test;
		podatki >> str >> str >> str >> str >> str >> trueOpica;
		podatki >> str >> str >> str >> str >> str >> falseOpica;
		//std::cout << test << ' ' << trueOpica << ' ' << falseOpica << '\n'; ////////////////


		resitev.push_back(Opica(ID, seznam, par, test, trueOpica, falseOpica));
	}

	podatki.close();


	return resitev;
}


void simuliraj(std::vector<Opica>& seznamOpic, int krat, int delitelj = 3) {

	for (int i = 0; i < krat; i++)
		for (Opica& opica : seznamOpic)
			while (opica.steviloElementov() > 0) {
				std::pair<int, int> par(opica.vrziElementOpici(delitelj));
				seznamOpic[par.first].dodajElement(par.second);
			}
}

int monkeyBusiness(std::vector<Opica> seznamOpic, int krat, int delitelj = 3) {

	simuliraj(seznamOpic, krat, delitelj);

	std::pair<int, int> max{ 0,0 };

	for (const Opica& opica : seznamOpic) {
		std::cout << opica << "  |  "; //////////////////////
		std::cout << opica.vrniSteviloInspekcij() << '\n'; //////////////////////

		int st(opica.vrniSteviloInspekcij());
		if (max.first > max.second) {
			if (st > max.second)
				max.second = st;
		}
		else if (st > max.first)
			max.first = st;
	}
	std::cout << '\n'; //////////////////////

	return max.first * max.second;
}


int main() {

	std::vector<Opica> seznamOpic(preberiPodatke("Advent22/11.txt"));
	int resitev1(monkeyBusiness(seznamOpic, 20));
	std::cout << "\"monkey business\" je " << resitev1 << ".\n\n";
	//for (const Opica& opica : seznamOpic)
	//	std::cout << opica << '\n';
	//std::cout << std::endl;


	int test(monkeyBusiness(seznamOpic, 20, 1));
	std::cout << "\"monkey business\" je " << test << ".\n\n";

	//int resitev2(monkeyBusiness(seznamOpic, 1000, 1));
	//std::cout << "\"monkey business\" je " << resitev2 << ".\n";


	return 0;
}
