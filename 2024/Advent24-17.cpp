/*
	Res preprosto, edin nadlezno je oblika rezultata, mors zapisat z ','

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

#define combo 1
#define literal 0

#define adv 0
#define bxl 1
#define bst 2
#define jnz 3
#define bxc 4
#define out 5
#define bdv 6
#define cdv 7


struct Register {
	long long A = 0;
	long long B = 0;
	long long C = 0;
};

std::ostream& operator<<(std::ostream& os, Register& re) {
	return os << "A: " << re.A << ", B: " << re.B << ", C: " << re.C;
}


inline long long dobiOperand(short stevilo, bool comboLiteral, Register& spisek) {
	if (comboLiteral == combo) {
		if (stevilo >= 0 && stevilo <= 3) return stevilo;
		if (stevilo == 4) return spisek.A;
		if (stevilo == 5) return spisek.B;
		if (stevilo == 6) return spisek.C;
		if (stevilo == 7) { std::cout << "Napaka - neveljavna vrednost.\n"; return -1; }
	}
	if (comboLiteral == literal) {
		return stevilo;
	}
}

bool pozeniProgram(std::vector<short>& program, Register& spisek, long long ponastavljenA = -1) {

	if (ponastavljenA != -1)
		spisek.A = ponastavljenA;


	std::vector<short> primerjava;

	std::vector<short>::iterator it = program.begin();

	while (it != program.end()) {
		//std::cout << spisek << '\n';
		if (*it == adv) { // 0
			long long stevec = spisek.A;
			long long imenovalec = pow(2, dobiOperand(*(it + 1), combo, spisek));
			spisek.A = floor(stevec / imenovalec);
			it++;
			it++;
		}
		else if (*it == bxl) { // 1
			long long lhs = spisek.B;
			long long rhs = dobiOperand(*(it + 1), literal, spisek);
			spisek.B = lhs ^ rhs;
			it++;
			it++;
		}
		else if (*it == bst) { // 2
			long long lhs = dobiOperand(*(it + 1), combo, spisek);
			long long rhs = 8;
			spisek.B = lhs % rhs;
			it++;
			it++;
		}
		else if (*it == jnz) { // 3
			if (spisek.A == 0) {
				it++;
				it++;
				continue;
			}
			it = program.begin() + dobiOperand(*(it + 1), literal, spisek);
		}
		else if (*it == bxc) { // 4
			long long lhs = spisek.B;
			long long rhs = spisek.C;
			spisek.B = lhs ^ rhs;
			it++;
			it++;
		}
		else if (*it == out) { // 5
			long long lhs = dobiOperand(*(it + 1), combo, spisek);
			long long rhs = 8;
			std::cout << (lhs % rhs) << ',';
			primerjava.push_back(lhs % rhs);
			it++;
			it++;
		}
		else if (*it == bdv) { // 6
			long long stevec = spisek.A;
			long long imenovalec = pow(2, dobiOperand(*(it + 1), combo, spisek));
			spisek.B = floor(stevec / imenovalec);
			it++;
			it++;
		}
		else if (*it == cdv) { // 7
			long long stevec = spisek.A;
			long long imenovalec = pow(2, dobiOperand(*(it + 1), combo, spisek));
			spisek.C = floor(stevec / imenovalec);
			it++;
			it++;
		}
		else {
			std::cout << "Napaka - neveljavna vrednost vnosa.\n";
			it++;
			it++;
		}

		if (primerjava == program) {
			std::cout << "\nNaredili\n";
			return true;
		}
	}

	return false;
}


void preberiPodatke(std::string pot, Register& spisek, std::vector<short>& program) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return;
	}

	std::string vrstica;

	while (std::getline(podatki, vrstica)) {

		if (vrstica.empty()) break;

		std::stringstream ss(vrstica);
		char reg;

		ss.ignore(8);
		ss >> reg;
		ss.ignore(1);
		
		if		(reg == 'A') ss >> spisek.A;
		else if (reg == 'B') ss >> spisek.B;
		else if (reg == 'C') ss >> spisek.C;
	}

	podatki.ignore(8);
	while (podatki.peek() != EOF) {
		short i;

		podatki >> i;
		program.push_back(i);

		podatki.ignore(1);
	}

	podatki.close();
}



int main() {

	Register spisek;
	std::vector<short> program;

	preberiPodatke("Advent24-17.txt", spisek, program);

	std::cout << spisek << '\n';
	for (short i : program) std::cout << i << ',';
	std::cout << '\n' << '\n';


	pozeniProgram(program, spisek);

	std::cout << '\n' << '\n' << spisek << '\n';
	for (short i : program) std::cout << i << ',';
	std::cout << '\n' << '\n';
	
	//for (long long i = 216122748300000; i < 216191498300000; i += 25000000) {
	//for (long long i = 216139928300000; i < 216148528300000; i += 10000000)
	
	for (long long i = 216139928300000; i < 216148628300000; i+= 10000000) {
		
		if (i % 10000000 == 0) {
			std::cout << "dela..." << 100 * double(i - 216139928300000) / (216148528300000 - 216139928300000) << " %\n";
		}

		if (pozeniProgram(program, spisek, i)) {
			std::cout << ' ' << i << '\n';
			break;
		}
		std::cout << ' ' << i << '\n';
	}

	//for (long long i = 200000000000004; i < 200000000008000; i += 1) {
	//	std::cout;	   
	//	pozeniProgram(program, spisek, i);
	//	std::cout << ' ' << i << '\n';
	//}

	/*
		/8   | 12-4
		/512 | 640-128
		/134.217.728 | 2688-2432-1601-960-640-448
	*/

	//for (long long i = 262100*8; i < 262200*8; i += 1) {
	//	std::cout;	   
	//	pozeniProgram(program, spisek, i);
	//	std::cout << ' ' << i << ' ' << std::bitset<32>(i) << '\n';
	//}

	//long long min = pow(8, 3) + 1;
	//long long max = pow(8, 4);
	//
	//for (long long i = min; i < max; i += 1) {
	//	std::cout;
	//	pozeniProgram(program, spisek, i);
	//	std::cout << ' ' << i << ' ' << std::bitset<32>(i) << '\n';
	//}

	long long min = pow(8, program.size() - 1) + 1;
	long long max = pow(8, program.size());

	std::cout << max - min;

	//for (long long i = min; i < max; i += 1) {
	//	std::cout;
	//	pozeniProgram(program, spisek, i);
	//	std::cout << ' ' << i << ' ' << std::bitset<32>(i) << '\n';
	//}


	std::cout << '\n' << '\n' << spisek << '\n';
	for (short i : program) std::cout << i << ',';
	std::cout << '\n';


	return 0;
}


