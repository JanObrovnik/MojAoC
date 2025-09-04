/*
	srednja tezavnost

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <thread>
#include <chrono>


std::string zapisiTrinarno(unsigned long long i) {

	std::string str = "";

	if (i > 2) {
		str += zapisiTrinarno(floor(i / 3));
	}

	str += std::to_string(i % 3);

	return str;
}


unsigned long long zapisiTrinarnoInt(unsigned long long i) {
	return std::stoull(zapisiTrinarno(i));
}

unsigned long long sestavi(unsigned long long num1, int num2) {
	return std::stoull(std::to_string(num1) + std::to_string(num2));
}


class Enacba {
	unsigned long long resitev;
	std::vector<int> stevila;

	
	bool mozno1Del() {

		int steviloOperatorjev = stevila.size() - 1;
		int steviloRazlicnihIzracunov = pow(2, steviloOperatorjev);

		for (int i = 0; i < steviloRazlicnihIzracunov; i++) {

			unsigned long long vmesnaVrednost = stevila[0];
			for (int j = 0; j < steviloOperatorjev; j++)
				vmesnaVrednost = ((i & (1 << j)) == (1 << j)) ?
					vmesnaVrednost * stevila[j + 1] : vmesnaVrednost + stevila[j + 1];

			if (vmesnaVrednost == resitev) return true;
		}

		return false;
	}

	bool mozno2Del() {

		int steviloOperatorjev = stevila.size() - 1;
		unsigned long long steviloRazlicnihIzracunov = pow(3, steviloOperatorjev);

		for (unsigned long long i = 0; i < steviloRazlicnihIzracunov; i++) {

			std::string trinarnoStevilo = zapisiTrinarno(i);
			while (trinarnoStevilo.size() < steviloOperatorjev)
				trinarnoStevilo.insert(trinarnoStevilo.begin(), '0');

			unsigned long long vmesnaVrednost = stevila[0];
			for (unsigned long long j = 0; j < steviloOperatorjev; j++) {				
				if (trinarnoStevilo[j] == '0') vmesnaVrednost *= stevila[j + 1];
				else if (trinarnoStevilo[j] == '1') vmesnaVrednost += stevila[j + 1];
				else vmesnaVrednost = sestavi(vmesnaVrednost, stevila[j + 1]);
			}


			if (vmesnaVrednost == resitev) return true;
		}


		return false;
	}

public:
	Enacba() : resitev(0), stevila({}) {}
	Enacba(unsigned long long resitev, std::vector<int> stevila) : resitev(resitev), stevila(stevila) {}


	unsigned long long ovrednoti(bool drugiDel = false) {
		if (!drugiDel) return mozno1Del() * resitev;
		else return mozno2Del() * resitev;
	}


	void izpis() {
		std::cout << resitev << ':';
		for (int i : stevila)
			std::cout << ' ' << i;
		std::cout << '\n';
	}
};


static std::list<Enacba> preberiPodatke(std::string pot) {

	std::list<Enacba> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std:: cout << "datoteka ni najdena.\n";
		return resitev;
	}

	std::string vrstica;
	while (std::getline(podatki, vrstica)) {

		std::stringstream ss(vrstica);

		unsigned long long vrednost;
		ss >> vrednost;
		ss.ignore(1);

		std::vector<int> stevila;
		int i;
		while (ss >> i)
			stevila.push_back(i);

		resitev.push_back(Enacba(vrednost, stevila));
	}

	podatki.close();


	return resitev;
}


unsigned long long resitev2 = 0;

void resi1(std::list<Enacba> seznam) {
	auto it = seznam.begin();
	std::advance(it, seznam.size() / 3);
	std::for_each(seznam.begin(), it, [&](Enacba& en) {resitev2 += en.ovrednoti(true); });
}

void resi2(std::list<Enacba> seznam) {

	auto it1 = seznam.begin();
	std::advance(it1, seznam.size() / 3);
	auto it2 = seznam.begin();
	std::advance(it2, seznam.size() / 3);
	std::advance(it2, seznam.size() / 3);
	std::for_each(it1, it2, [&](Enacba& en) {resitev2 += en.ovrednoti(true); });
}

void resi3(std::list<Enacba> seznam) {

	auto it = seznam.begin();
	std::advance(it, seznam.size() / 3);
	std::advance(it, seznam.size() / 3);
	std::for_each(it, seznam.end(), [&](Enacba& en) {resitev2 += en.ovrednoti(true); });
}


int main() {

	std::list<Enacba> seznam(preberiPodatke("Advent24-7.txt"));
	//std::for_each(seznam.begin(), seznam.end(), [](Enacba& en) {en.izpis(); });


	unsigned long long resitev1 = 0;
	std::chrono::high_resolution_clock::time_point cas11 (std::chrono::high_resolution_clock::now());
	std::for_each(seznam.begin(), seznam.end(), [&](Enacba& en) {resitev1 += en.ovrednoti(); });
	std::chrono::high_resolution_clock::time_point cas12 (std::chrono::high_resolution_clock::now());
	std::chrono::duration<double> cas1(std::chrono::duration_cast<std::chrono::duration<double>>(cas12 - cas11));

	std::cout << "Vsota vseh pravilnih vrednosti je " << resitev1 << ". Cas = " << cas1.count()*1000 << " ms.\n";


	std::chrono::high_resolution_clock::time_point cas31(std::chrono::high_resolution_clock::now());
	std::thread worker1(resi1, seznam);
	std::thread worker2(resi2, seznam);
	std::thread worker3(resi3, seznam);
	worker1.join();
	worker2.join();
	worker3.join();
	std::chrono::high_resolution_clock::time_point cas32(std::chrono::high_resolution_clock::now());
	std::chrono::duration<double> cas3(std::chrono::duration_cast<std::chrono::duration<double>>(cas32 - cas31));

	std::cout << "Vsota vseh pravilnih vrednosti je " << ::resitev2 << ". Cas = " << cas3.count() * 1000 << " ms.\n";;


	unsigned long long resitev2 = 0;
	std::chrono::high_resolution_clock::time_point cas21(std::chrono::high_resolution_clock::now());
	std::for_each(seznam.begin(), seznam.end(), [&](Enacba& en) {resitev2 += en.ovrednoti(true); });
	std::chrono::high_resolution_clock::time_point cas22(std::chrono::high_resolution_clock::now());
	std::chrono::duration<double> cas2(std::chrono::duration_cast<std::chrono::duration<double>>(cas22 - cas21));
	
	std::cout << "Vsota vseh pravilnih vrednosti je " << resitev2 << ". Cas = " << cas2.count() * 1000 << " ms.\n";


	std::cout << "Naredilo.\n";
	for (;;) {}


	return 0;
}