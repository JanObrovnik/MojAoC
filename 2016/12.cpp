/*
	t = 1:10:6 ur
	precej lahko
	t = 3:43 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


enum class Register {
	A,
	B,
	C,
	D,
	size
};
enum class Ukaz {
	CPY,
	INC,
	DEC,
	JNZ,
	size
};

Register identificirajReg(const char& reg) {
	if (reg == 'a') return Register::A;
	if (reg == 'b') return Register::B;
	if (reg == 'c') return Register::C;
	if (reg == 'd') return Register::D;
	return Register::size;
}
Ukaz identificirajUkaz(const std::string& ukaz) {
	if (ukaz == "cpy") return Ukaz::CPY;
	if (ukaz == "inc") return Ukaz::INC;
	if (ukaz == "dec") return Ukaz::DEC;
	if (ukaz == "jnz") return Ukaz::JNZ;
	return Ukaz::size;
}


struct Vrstica {

	Ukaz ukaz;
	Register reg1, reg2;
	int val1, val2;

	Vrstica() : ukaz(Ukaz::size), reg1(Register::size), reg2(Register::size), val1(0), val2(0) {}
};


std::vector<Vrstica> preberiPodatke(const std::string& pot) {

	std::vector<Vrstica> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string str;

	while (podatki.peek() != EOF) {

		resitev.push_back(Vrstica());

		podatki >> str;
		resitev.back().ukaz = identificirajUkaz(str);

		podatki >> str;
		if (str == "a" || str == "b" || str == "c" || str == "d") {
			resitev.back().reg1 = identificirajReg(str.front());
		}
		else {
			resitev.back().val1 = std::stoi(str);
		}

		if (resitev.back().ukaz == Ukaz::INC || resitev.back().ukaz == Ukaz::DEC)
			continue;

		podatki >> str;
		if (str == "a" || str == "b" || str == "c" || str == "d") {
			resitev.back().reg2 = identificirajReg(str.front());
		}
		else {
			resitev.back().val2 = std::stoi(str);
		}
	}

	podatki.close();

	return resitev;
}


void opraviUkazCPY(const Vrstica& vrstica, std::vector<int>& seznamRegistrov, int& index) {
	if (vrstica.reg1 != Register::size)
		seznamRegistrov[static_cast<size_t>(vrstica.reg2)] = seznamRegistrov[static_cast<size_t>(vrstica.reg1)];
	else
		seznamRegistrov[static_cast<size_t>(vrstica.reg2)] = vrstica.val1;
	index++;
}
void opraviUkazINC(const Vrstica& vrstica, std::vector<int>& seznamRegistrov, int& index) {
	seznamRegistrov[static_cast<size_t>(vrstica.reg1)]++;
	index++;
}
void opraviUkazDEC(const Vrstica& vrstica, std::vector<int>& seznamRegistrov, int& index) {
	seznamRegistrov[static_cast<size_t>(vrstica.reg1)]--;
	index++;
}
void opraviUkazJNZ(const Vrstica& vrstica, std::vector<int>& seznamRegistrov, int& index) {
	if (vrstica.reg1 != Register::size && seznamRegistrov[static_cast<size_t>(vrstica.reg1)] != 0)
		index += vrstica.val2;
	else if (vrstica.reg1 == Register::size && vrstica.val1 != 0)
		index += vrstica.val2;
	else
		index++;
}
void opraviUkaz(const Vrstica& vrstica, std::vector<int>& seznamRegistrov, int& index) {
	if		(vrstica.ukaz == Ukaz::CPY) opraviUkazCPY(vrstica, seznamRegistrov, index);
	else if (vrstica.ukaz == Ukaz::INC) opraviUkazINC(vrstica, seznamRegistrov, index);
	else if (vrstica.ukaz == Ukaz::DEC) opraviUkazDEC(vrstica, seznamRegistrov, index);
	else if (vrstica.ukaz == Ukaz::JNZ) opraviUkazJNZ(vrstica, seznamRegistrov, index);
}

int izvediUkaze(const std::vector<Vrstica>& seznamVrstic, const Register& reg, const Register& ponastavljenReg = Register::size, const int& ponastavljenVal = 0) {

	std::vector<int> seznamRegistrov(static_cast<size_t>(Register::size), 0);

	if (ponastavljenReg != Register::size)
		seznamRegistrov[static_cast<size_t>(ponastavljenReg)] = ponastavljenVal;

	int index = 0;

	while (index < seznamVrstic.size())
		opraviUkaz(seznamVrstic[index], seznamRegistrov, index);

	return seznamRegistrov[static_cast<size_t>(reg)];
}


int main() {

	std::vector<Vrstica> seznamVrstic = preberiPodatke("2016/12.txt");


	int resitev1 = izvediUkaze(seznamVrstic, Register::A);
	std::cout << "Na registerju A je vrednost " << resitev1 << ".\n";

	int resitev2 = izvediUkaze(seznamVrstic, Register::A, Register::C, 1);
	std::cout << "Z ponastavitvijo je na registerju A vrednost " << resitev2 << ".\n";


	return 0;
}
