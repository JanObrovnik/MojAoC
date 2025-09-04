/*
	Neverjetno lahko
	Neverjetno lahko
*/

#include <iostream>
#include <fstream>



std::pair<int,int> preberiPodatke(std::string pot) {

	int resitev1(0);
	int resitev2(0);


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { 0,0 };
	}

	std::pair<int, int> skrat1, skrat2;
	char c;

	while (podatki.peek() != EOF) {
		
		podatki >> skrat1.first >> c >> skrat1.second >> c >> skrat2.first >> c >> skrat2.second;

		if (skrat1.first <= skrat2.first && skrat1.second >= skrat2.second ||
			skrat2.first <= skrat1.first && skrat2.second >= skrat1.second)
			resitev1++;

		if (skrat1.first  >= skrat2.first && skrat1.first  <= skrat2.second ||
			skrat1.second >= skrat2.first && skrat1.second <= skrat2.second ||
			skrat2.first  >= skrat1.first && skrat2.first  <= skrat1.second ||
			skrat2.second >= skrat1.first && skrat2.second <= skrat1.second)
			resitev2++;
	}

	podatki.close();

	return { resitev1,resitev2 };
}


int main() {

	std::pair<int, int> resitev(preberiPodatke("Advent22/4.txt"));
	std::cout << "Stevilo vsebovanih v drugih je " << resitev.first << ".\n";
	std::cout << "Stevilo vsebovanih v drugih je " << resitev.second << ".\n";


	return 0;
}