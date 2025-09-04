/*
	Zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>


inline void korak1(long long& i) {
	i ^= (i * 64);
	i %= 16777216;
}
inline void korak2(long long& i) {
	i ^= long long(i / 32);
	i %= 16777216;
}
inline void korak3(long long& i) {
	i ^= (i * 2048);
	i %= 16777216;
}

std::vector<short> psevdoNakljucno(std::list<long long>& seznam) {

	std::vector<short> resitev;
	resitev.reserve(seznam.size());

	std::for_each(seznam.begin(), seznam.end(), [&resitev](long long& i) {
		korak1(i);
		korak2(i);
		korak3(i);
		resitev.push_back(i % 10);
		});

	return resitev;
}


std::list<long long> preberiPodatke(std::string pot) {
	
	std::list<long long> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bila najdena.\n";
		return resitev;
	}

	int stevilo;
	while (podatki >> stevilo)
		resitev.push_back(stevilo);

	podatki.close();


	return resitev;
}


int main() {

	std::list<long long> seznam(preberiPodatke("Advent24-22.txt"));
	
	//for (long long& i : seznam)
	//	std::cout << i << '\n';
	//std::cout << '\n';

	
	short steviloPonovitev(2000);

	std::vector<std::vector<short>> seznamCen(seznam.size(), std::vector<short>{});
	for (std::vector<short>& vec : seznamCen) vec.reserve(steviloPonovitev);

	for (int i = 0; i < steviloPonovitev; i++) {
		std::vector<short> trenutnaCene(psevdoNakljucno(seznam));
		for (int i = 0; i < trenutnaCene.size(); i++) {

		}
	}

	long long resitev(std::accumulate(seznam.begin(), seznam.end(), long long(0)));

	//for (long long& i : seznam)
	//	std::cout << i << '\n';
	//std::cout << '\n';


	std::cout << "Vsota vseh psevdo nakljucnih stevil je " << resitev << ".\n";
	
	



	std::vector<short> test;
	//test.reserve(10);
	//std::cout << test.capacity() << '|' << test.size() << '\n';
	//
	//for (int i = 0; i < 10; i++) {
	//	test.push_back(i);
	//	//test[i] = i; ne dela
	//	std::cout << test.capacity() << '|' << test.size() << '\n';
	//}
	//
	//for(short i : test) std::cout << test[i] << '\n';
	//
	//test.reserve(20);
	//for (short i : test) std::cout << test[i] << '\n';
	//
	//std::cout << test.capacity() << '|' << test.size() << '\n';
	//
	std::cout << "\n.size()" << '|' << ".capacity()" << '\n';
	for (int i = 0; i < 500; i++) {
		std::cout << test.size() << '|' << test.capacity() << '\n';
		test.push_back(i);
	}
	//
	//for (int i = 0; i < 480; i++) {
	//	std::cout << test.capacity() << '|' << test.size() << '\n';
	//	test.erase(test.begin());
	//}
	//
	//test.resize(10);
	//std::cout << test.capacity() << '|' << test.size() << '\n';
	//
	//test.reserve(10);
	//std::cout << test.capacity() << '|' << test.size() << '\n';
	//
	//test.reserve(1776);
	//std::cout << test.capacity() << '|' << test.size() << '\n';


	return 0;
}