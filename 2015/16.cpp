/*
	t = 54:07 min
	lahko
	t = 9:14 min
	zelo lahko (malo sprememb od dela 1)
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>


struct Sue {
	int id;
	int children, cats, samoyeds, pomeranians, akitas, vizslas, goldfish, trees, cars, perfumes;

	Sue() : id(-1), children(-1), cats(-1), samoyeds(-1), pomeranians(-1), akitas(-1), vizslas(-1), goldfish(-1), trees(-1), cars(-1), perfumes(-1) {}
};

std::ostream& operator<<(std::ostream& os, const Sue& su) {
	return os << su.id
		<< ": children " << su.children
		<< ", cats " << su.cats
		<< ", samoyeds " << su.samoyeds
		<< ", pomeranians " << su.pomeranians
		<< ", akitas " << su.akitas
		<< ", vizslas " << su.vizslas
		<< ", goldfish " << su.goldfish
		<< ", trees " << su.trees
		<< ", cars " << su.cars
		<< ", perfumes " << su.perfumes;
}


std::array<Sue, 500>* preberiPodatke(const std::string& pot) {

	std::array<Sue, 500>* resitev = new std::array<Sue, 500>;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki.peek() != EOF) {

		std::getline(podatki, vrstica);
		std::stringstream ss(vrstica);

		std::string naziv;
		int id, kolicina;

		ss >> naziv;
		ss >> id;
		ss.ignore(1);

		resitev->at(id - 1).id = id;

		while (ss.peek() != EOF) {

			ss >> naziv;
			naziv.pop_back();
			ss >> kolicina;
			ss.ignore(1);

			if (naziv == "children") resitev->at(id - 1).children = kolicina;
			else if (naziv == "cats") resitev->at(id - 1).cats = kolicina;
			else if (naziv == "samoyeds") resitev->at(id - 1).samoyeds = kolicina;
			else if (naziv == "pomeranians") resitev->at(id - 1).pomeranians = kolicina;
			else if (naziv == "akitas") resitev->at(id - 1).akitas = kolicina;
			else if (naziv == "vizslas") resitev->at(id - 1).vizslas = kolicina;
			else if (naziv == "goldfish") resitev->at(id - 1).goldfish = kolicina;
			else if (naziv == "trees") resitev->at(id - 1).trees = kolicina;
			else if (naziv == "cars") resitev->at(id - 1).cars = kolicina;
			else if (naziv == "perfumes") resitev->at(id - 1).perfumes = kolicina;
		}
	}

	podatki.close();

	return resitev;
}


inline bool primerjajSueSue1(const Sue& sue, Sue& iskanaSue) {
	
	return (sue.children == -1 || (sue.children == iskanaSue.children)) &&
		(sue.cats == -1 || (sue.cats == iskanaSue.cats)) &&
		(sue.samoyeds == -1 || (sue.samoyeds == iskanaSue.samoyeds)) &&
		(sue.pomeranians == -1 || (sue.pomeranians == iskanaSue.pomeranians)) &&
		(sue.akitas == -1 || (sue.akitas == iskanaSue.akitas)) &&
		(sue.vizslas == -1 || (sue.vizslas == iskanaSue.vizslas)) &&
		(sue.goldfish == -1 || (sue.goldfish == iskanaSue.goldfish)) &&
		(sue.trees == -1 || (sue.trees == iskanaSue.trees)) &&
		(sue.cars == -1 || (sue.cars == iskanaSue.cars)) &&
		(sue.perfumes == -1 || (sue.perfumes == iskanaSue.perfumes));
}
inline bool primerjajSueSue2(const Sue& sue, Sue& iskanaSue) {

	return (sue.children == -1 || (sue.children == iskanaSue.children)) &&
		(sue.cats == -1 || (sue.cats > iskanaSue.cats)) &&
		(sue.samoyeds == -1 || (sue.samoyeds == iskanaSue.samoyeds)) &&
		(sue.pomeranians == -1 || (sue.pomeranians < iskanaSue.pomeranians)) &&
		(sue.akitas == -1 || (sue.akitas == iskanaSue.akitas)) &&
		(sue.vizslas == -1 || (sue.vizslas == iskanaSue.vizslas)) &&
		(sue.goldfish == -1 || (sue.goldfish < iskanaSue.goldfish)) &&
		(sue.trees == -1 || (sue.trees > iskanaSue.trees)) &&
		(sue.cars == -1 || (sue.cars == iskanaSue.cars)) &&
		(sue.perfumes == -1 || (sue.perfumes == iskanaSue.perfumes));
}

void najdiSueID(std::array<Sue, 500>* seznamSue, Sue& iskanaSue, int del = 1) {

	//std::cout << iskanaSue << '\n' << '\n';
	//for (const Sue& sue : *seznamSue) std::cout << sue << '\n';

	for (const Sue& sue : *seznamSue) {
		if (del == 1) {
			if (primerjajSueSue1(sue, iskanaSue))
				iskanaSue.id = sue.id;
		}
		else if (del == 2) {
			if (primerjajSueSue2(sue, iskanaSue))
				iskanaSue.id = sue.id;
		}
	}
}


int main() {

	std::array<Sue, 500>* seznamSue = preberiPodatke("2015/16.txt");
	
	Sue iskanaSue;
	iskanaSue.children = 3;
	iskanaSue.cats = 7;
	iskanaSue.samoyeds = 2;
	iskanaSue.pomeranians = 3;
	iskanaSue.akitas = 0;
	iskanaSue.vizslas = 0;
	iskanaSue.goldfish = 5;
	iskanaSue.trees = 3;
	iskanaSue.cars = 2;
	iskanaSue.perfumes = 1;


	najdiSueID(seznamSue, iskanaSue, 1);
	std::cout << "Iskana Sue je Sue " << iskanaSue.id << ".\n";

	najdiSueID(seznamSue, iskanaSue, 2);
	std::cout << "Iskana Sue je Sue " << iskanaSue.id << ".\n";


	delete(seznamSue);
	
	return 0;
}
