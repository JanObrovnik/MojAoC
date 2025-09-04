#include <iostream>
#include <fstream>
#include <sstream>
//#include <string>
#include <vector>
#include <algorithm>



//struct Seznam {
//	int notranjost;
//	std::vector<int> vrednosti;
//	std::vector<Seznam> seznam;
//
//	Seznam(int notranjost) : notranjost(notranjost), vrednosti(), seznam() {}
//};


void razberiSeznam(std::string argument) {

	std::vector<std::string::iterator> seznamVejic;

	for ()
}


bool pravilnoZaporedje(std::string& leviArgument, std::string& desniArgument) {




	//std::find_if(leviArgument.begin(), leviArgument.end(), [](const char& c) {return c >= '0' && c <= '9'; });

	int notranjost(0);

	for (const char& c : leviArgument) {

		if (c == '[') {
			notranjost++;
			continue;
		}
		if (c == ']') {
			notranjost--;
			continue;
		}
		if (c == '[') {
			notranjost++;
			continue;
		}
	}
}


int preberiPodatke(const std::string& pot) {

	int resitev(0);


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string leviArgument, desniArgument;
	int index = 1;

	while (podatki.peek() != EOF) {

		podatki >> leviArgument >> desniArgument;
		podatki.ignore(1);

		///////////////////// tu funkcija()
		resitev += index * 
		index++;
	}

	podatki.close();



}
