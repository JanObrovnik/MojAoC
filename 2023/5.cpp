/*
	Srednje tezavno
	Nekoliko tezka
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>


class Map {
private:
	std::vector<unsigned int> destination;
	std::vector<unsigned int> source;
	std::vector<unsigned int> lenght;

	std::vector<std::pair<unsigned int, unsigned int>> range;
	std::vector<unsigned int> difference;

public:

	void addTransformation(const unsigned int& destination, const unsigned int& source, const unsigned int& lenght) {
		this->destination.push_back(destination);
		this->source.push_back(source);
		this->lenght.push_back(lenght);
	}

	unsigned int transform(const unsigned int& input) const {
		unsigned int output = input;

		for (int i = 0; i < destination.size(); i++) {
			const unsigned int&des = destination[i];
			const unsigned int&sou = source[i];
			const unsigned int&len = lenght[i];

			if (input >= sou && input < sou + len) output += des - sou;
		}

		return output;
	}

	void writeRange() {

		range.clear();
		difference.clear();

		for (int i = 0; i < destination.size(); i++) {
			const unsigned int& des = destination[i];
			const unsigned int& sou = source[i];
			const unsigned int& len = lenght[i];

			range.push_back({ sou, sou + len - 1 });
			difference.push_back(des - sou);
		}
	}

	std::list<std::pair<unsigned int, unsigned int>> transformList(const std::pair<unsigned int, unsigned int>& input) const {

		std::list<std::pair<unsigned int, unsigned int>> output;
		std::list<std::pair<unsigned int, unsigned int>> untransformedOutput;

		//std::cout << "dela-1: " << input.first << '|' << input.second << ':' << destination.front() << "\n";

		for (int i = 0; i < range.size(); i++) {
			const std::pair<unsigned int, unsigned int>& pair = range[i];
			const unsigned int& diff = difference[i];

			if (pair.first > input.second)
				continue;
			if (pair.second < input.first)
				continue;

			if (pair.first < input.first && pair.second > input.second)
				return{ { input.first + diff,input.second + diff } };
			//std::cout << "dela0\n";

			if (pair.first < input.first) {
				output.push_back({ input.first + diff,pair.second + diff });
				untransformedOutput.push_back({ input.first,pair.second });
				continue;
			}
			if (pair.second > input.second) {
				output.push_back({ pair.first + diff,input.second + diff });
				untransformedOutput.push_back({ pair.first,input.second });
				continue;
			}

			output.push_back({ pair.first + diff,pair.second + diff });
			untransformedOutput.push_back(pair);
		}

		if (output.empty()) //////////////////
			return { input };

		untransformedOutput.sort();

		//std::cout << "dela1 " << untransformedOutput.size() << "\n";

		auto it = untransformedOutput.begin();
		while (true) {
			std::pair<unsigned int, unsigned int> pair;
			pair.first = it++->second;
			if (it == untransformedOutput.end())
				break;
			pair.second = it->first;
			if (pair.first != pair.second)
				output.push_back(pair);
		}
		//std::cout << "dela2\n";

		if (input.first != untransformedOutput.begin()->first)
			output.push_back({ input.first,untransformedOutput.begin()->first });
		//std::cout << "dela3\n";


		if (untransformedOutput.rbegin()->second != input.second)
			output.push_back({ untransformedOutput.rbegin()->second,input.second });
		//std::cout << "dela4\n";

		output.sort();


		return output;
	}
};

unsigned int multiTransform(unsigned int input, std::list<Map>& orderOfOperations) {

	unsigned int output = input;

	std::for_each(orderOfOperations.begin(), orderOfOperations.end(), [&output](Map& map) {
		output = map.transform(output);
		});

	return output;
}

unsigned int multiRangeTransform(std::pair<unsigned int, unsigned int> input, const std::list<Map>& orderOfOperations) {
	

	std::list<std::pair<unsigned int, unsigned int>> list = { input };

	std::for_each(orderOfOperations.begin(), orderOfOperations.end(), [&list](const Map& map) {
		
		std::list<std::pair<unsigned int, unsigned int>> middleList{};
		//std::cout << "del0 " << list.size() << "\n";

		std::for_each(list.begin(), list.end(), [&](const std::pair<unsigned int, unsigned int>& pair) {
			//std::cout << "del1 " << pair.first << '|' << pair.second << "\n";
			std::list<std::pair<unsigned int, unsigned int>> innerList = map.transformList(pair);
			//std::cout << "del2 " << innerList.size() << "\n";

			std::for_each(innerList.begin(), innerList.end(), [&](std::pair<unsigned int, unsigned int> innerPair) {
			
				middleList.push_back(innerPair);
				});
			});
		
		list = middleList;
		});

	//std::cout << list.size() << '\n';
	return std::min_element(list.begin(), list.end())->first;
}


int main() {

	std::list<unsigned int> seedList;
	std::list<std::pair<unsigned int, unsigned int>> seedRangeList;
	std::list<Map> orderOfOperations;


	std::fstream podatki;
	podatki.open("Advent23/5.txt", std::ios::in);


	if (podatki.is_open()) {

		std::string str;
		unsigned int i;
		char ch;

		std::getline(podatki, str);
		std::stringstream ss(str);

		ss >> str;
		while (ss >> i) {
			seedList.push_back(i);
			seedRangeList.push_back({ i,0 });
			ss >> i;
			seedList.push_back(i);
			seedRangeList.back().second = seedRangeList.back().first + i - 1;
		}

		while (podatki.peek() != EOF) {

			std::getline(podatki, str);


			if (str.empty()) continue;

			if (str[0] < '0' || str[0] > '9') {
				orderOfOperations.push_back(Map());
				continue;
			}

			std::stringstream ss(str);
			unsigned int des, sou, len;

			ss >> des >> sou >> len;

			orderOfOperations.back().addTransformation(des, sou, len);
		}
		for (Map& map : orderOfOperations)
			map.writeRange();

		podatki.close();
	}
	else std::cout << "Datoteka ni najdena.";


	std::list<unsigned int> destinationList;

	std::for_each(seedList.begin(), seedList.end(), [&destinationList, &orderOfOperations](unsigned int i) {
		destinationList.push_back(multiTransform(i, orderOfOperations));
		});

	unsigned int result = *std::min_element(destinationList.begin(), destinationList.end());


	std::cout << "\nnajmanjsa vrednost je " << result << ".\n";
	std::cout << std::endl;


	
	
	std::list<unsigned int> destinationRangeList;

	std::for_each(seedRangeList.begin(), seedRangeList.end(), [&](std::pair<unsigned int, unsigned int> pair) {
		destinationRangeList.push_back(multiRangeTransform(pair, orderOfOperations));
		});

	unsigned int result2 = *std::min_element(destinationRangeList.begin(), destinationRangeList.end());

	std::cout << "\nnajmanjsa vrednost rejndza je " << result2 << ".\n";
	std::cout << std::endl;






	return 0;
}
