#include <array>
#include <fstream>
#include <iostream>
#include <mos6502.h>
#include <stdexcept>
#include <string>
#include <strings.h>

// g++ -Imos6502 emulator.cpp mos6502/mos6502.cpp -o emulator

static void usage(std::ostream& os) {
	os <<
		"usage: emulator <program>\n"
		"\n"
		"A simple 6502 CPU emulator capable of running compiled benchs,\n"
		"printing performance statistics.\n"
	;
}

int main(int argc, char** argv) {
	size_t const RAM_SIZE = 0x0800;
	size_t const PRG_SIZE = 0x8000;
	size_t const PRG_START = 0x8000;

	// Parse command line
	if (argc != 2 || argv[1] == std::string("-h") || argv[1] == std::string("--help")) {
		usage(argc != 2 ? std::cerr : std::cout);
		return argc != 2 ? 1 : 0;
	}
	std::string program_path(argv[1]);

	// Load progam in memory
	std::array<uint8_t, RAM_SIZE> ram;
	std::array<uint8_t, PRG_SIZE> prg;
	{
		std::ifstream program_file(program_path);
		program_file.read(reinterpret_cast<char*>(prg.data()), PRG_SIZE);
		if (program_file.fail()) {
			std::cerr << "cannot read the program from " << program_path << '\n'; // Can happen if the file is too short
		}
	}
	::bzero(ram.data(), ram.size());

	// Run the program
	mos6502 emulator;
	emulator.initBus(
		[&](uint16_t addr) {
			if (addr >= PRG_START) {
				return prg[addr - PRG_START];
			}else if (addr < RAM_SIZE) {
				return ram[addr];
			}
			throw std::runtime_error("invalid memory access");
		},
		[&](uint16_t addr, uint8_t val) {
			if (addr < RAM_SIZE) {
				ram[addr] = val;
			}else if (addr == 0x5000 && val == 0) {
				throw std::runtime_error("TODO make a type for this error (or better, cleanly quit the emulator's loop)");
			}
		}
	);

	emulator.Reset();
	try {
		emulator.Run(10'000'000);
	}catch (std::runtime_error const& e) {
		std::cout << "stopped by " << e.what() << '\n';
	}

	// Print result
	std::cout << "cycles consumed: " << emulator.cycleCount << '\n';
	std::cout << "RAM {\n\t";
	for (size_t i = 0; i < ram.size(); ++i) {
		if (i % 0x10 == 0 && i != 0) {
			printf("\n\t");
		}
		::printf("%02x ", ram[i]);
	}
	std::cout << "\n}\n";

	return 0;
}
