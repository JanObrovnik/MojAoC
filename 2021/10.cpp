/*
	t = 33:04 min
	precej lahko
	t = 24:26 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <algorithm>


const std::map<const char, const short> vrednostZnaka1{ {')',3},{']',57},{'}',1197},{'>',25137} };
const std::map<const char, const short> vrednostZnaka2{ {'(',1},{'[',2},{'{',3},{'<',4} };
const std::set<char> odpiralniZnaki{ '(','[','{','<' };
const std::set<char> zapiralniZnaki{ ')',']','}','>' };


bool veljavenOklepaj(const char c1, const char c2) {
	if (c1 == '(' && c2 == ')') return true;
	if (c1 == '[' && c2 == ']') return true;
	if (c1 == '{' && c2 == '}') return true;
	if (c1 == '<' && c2 == '>') return true;
	return false;
}


long long oceniMankajoceZnake(std::stack<char>& stackZnakov) {

	long long resitev = 0;

	while (!stackZnakov.empty()) {

		resitev *= 5;
		resitev += vrednostZnaka2.at(stackZnakov.top());

		stackZnakov.pop();
	}

	return resitev;
}


std::pair<long long, long long> preberiPodatke(const std::string& pot) {

	long long resitev1 = 0;
	long long resitev2 = 0;
	std::vector<long long> seznam;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	std::string vrstica;

	while (std::getline(podatki, vrstica)) {

		bool korupcija = false;

		std::stack<char> stackZnakov;

		for (const char& c : vrstica) {
			
			if (stackZnakov.empty()) {
				stackZnakov.push(c);
				continue;
			}
			
			if (odpiralniZnaki.find(c) != odpiralniZnaki.end()) {
				stackZnakov.push(c);
				continue;
			}
			if (zapiralniZnaki.find(c) != zapiralniZnaki.end()) {
				if (veljavenOklepaj(stackZnakov.top(), c)) {
					stackZnakov.pop();
					continue;
				}
				else {
					resitev1 += vrednostZnaka1.at(c);
					korupcija = true;
					break;
				}
			}

			std::cout << "Napaka\n";
		}

		if (!korupcija) {
			seznam.push_back(oceniMankajoceZnake(stackZnakov));
		}
	}

	podatki.close();

	std::sort(seznam.begin(), seznam.end());
	resitev2 = seznam[(int)floor(seznam.size()/2)];

	return { resitev1,resitev2 };
}


int main() {

	std::pair<long long, long long> resitev(preberiPodatke("2021/10.txt"));
	
	std::cout << "Vsota vseh napak je " << resitev.first << ".\n"; // 193275

	std::cout << "Vsota vseh mankajocih je " << resitev.second << ".\n"; // 2429644557


	return 0;
}
