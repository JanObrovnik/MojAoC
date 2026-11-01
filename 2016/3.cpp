/*
	t = 8:06 min
	zelo lahko
	t = 6:09 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


int preberiPodatke1(const std::string& pot) {

	int resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::array<int, 3> stranice = {0,0,0};

	while (podatki.peek() != EOF) {
		
		for (int i = 0; i < 3; i++)
			podatki >> stranice[i];

		auto itMax = std::max_element(stranice.begin(), stranice.end());

		int sestevek = 0;

		for (auto it = stranice.begin(); it != stranice.end(); it++)
			if (it != itMax)
				sestevek += *it;

		if (sestevek > *itMax)
			resitev++;
	}

	podatki.close();

	return resitev;
}

int preberiPodatke2(const std::string& pot) {

	int resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::array<std::array<int, 3>, 3> straniceX3 = { 0,0,0 };
	

	while (podatki.peek() != EOF) {

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				podatki >> straniceX3[j][i];

		for (const std::array<int, 3>& stranice : straniceX3) {

			auto itMax = std::max_element(stranice.begin(), stranice.end());

			int sestevek = 0;

			for (auto it = stranice.begin(); it != stranice.end(); it++)
				if (it != itMax)
					sestevek += *it;

			if (sestevek > *itMax)
				resitev++;
		}
	}

	podatki.close();

	return resitev;
}


int main() {

	int resitev1 = preberiPodatke1("2016/3.txt");
	std::cout << "Stevilo trikotnikov je " << resitev1 << ".\n";

	int resitev2 = preberiPodatke2("2016/3.txt");
	std::cout << "Stevilo trikotnikov je " << resitev2 << ".\n";


	return 0;
}
