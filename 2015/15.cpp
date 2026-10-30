/*
	t = 1:03:30 ur
	nekoliko lahko
	t = 4:00 min
	neverjetno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct Sestavina {

	std::string ime;
	int capacity, durability, flavor, texture, calories;

	Sestavina() : ime(""), capacity(0), durability(0), flavor(0), texture(0), calories(0) {}

	unsigned long long vrednostSestavin() {
		if (capacity < 0 || durability < 0 || flavor < 0 || texture < 0)
			return 0;
		unsigned long long resitev = 1;
		resitev *= capacity;
		resitev *= durability;
		resitev *= flavor;
		resitev *= texture;
		return resitev;
	}
};

Sestavina& operator+=(Sestavina& se1, const Sestavina& se2) {
	se1.capacity	+= se2.capacity;
	se1.durability	+= se2.durability;
	se1.flavor		+= se2.flavor;
	se1.texture		+= se2.texture;
	se1.calories	+= se2.calories;
	return se1;
}
Sestavina operator*(Sestavina se, const int& i) {
	se.capacity		*= i;
	se.durability	*= i;
	se.flavor		*= i;
	se.texture		*= i;
	se.calories		*= i;
	return se;
}

inline std::istream& operator>>(std::istream& is, Sestavina& se) {
	is >> se.ime;
	se.ime.pop_back();
	is.ignore(10);
	is >> se.capacity;
	is.ignore(13);
	is >> se.durability;
	is.ignore(9);
	is >> se.flavor;
	is.ignore(10);
	is >> se.texture;
	is.ignore(11);
	is >> se.calories;
	return is;
}
inline std::ostream& operator<<(std::ostream& os, Sestavina& se) {
	return os << se.ime
		<< ": capacity " << se.capacity 
		<< ", durability " << se.durability 
		<< ", flavor " << se.flavor 
		<< ", texture " << se.texture 
		<< ", calories " << se.calories;
}


std::vector<Sestavina> preberiPodatke(const std::string& pot) {

	std::vector<Sestavina> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	while (podatki.peek() != EOF) {
		resitev.push_back(Sestavina());
		podatki >> resitev.back();
	}

	podatki.close();

	return resitev;
}


std::pair<unsigned long long, unsigned long long> preveriVse(const std::vector<Sestavina>& seznamSestavin) {

	unsigned long long resitev1 = 0;
	unsigned long long resitev2 = 0;

	for (int i0 = 0; i0 <= 100; i0++)
		for (int i1 = 0; i1 <= 100 - i0; i1++)
			for (int i2 = 0; i2 <= 100 - i0 - i1; i2++) {
				int i3 = 100 - i0 - i1 - i2;

				Sestavina skupno;

				skupno += seznamSestavin[0] * i0;
				skupno += seznamSestavin[1] * i1;
				skupno += seznamSestavin[2] * i2;
				skupno += seznamSestavin[3] * i3;

				const unsigned long long skupnaVrednost = skupno.vrednostSestavin();

				if (skupnaVrednost > resitev1)
					resitev1 = skupnaVrednost;

				if (skupno.calories == 500 && skupnaVrednost > resitev2)
					resitev2 = skupnaVrednost;
			}

	return { resitev1,resitev2 };
}


int main() {

	std::vector<Sestavina> seznamSestavin = preberiPodatke("2015/15.txt");


	std::pair<unsigned long long, unsigned long long> resitev = preveriVse(seznamSestavin);

	std::cout << "skupna vrednost sestavin je " << resitev.first << ".\n";
	std::cout << "skupna vrednost sestavin z 500 kcal je " << resitev.second << ".\n";


	return 0;
}
