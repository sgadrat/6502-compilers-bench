#include <array>
#include <fstream>
#include <iostream>
#include <mos6502.h>
#include <stdexcept>
#include <string>
#include <cstring>
#include <strings.h>

// g++ -Imos6502 emulator.cpp mos6502/mos6502.cpp -o emulator

class StoppedByProgram: public std::runtime_error {
public:
	StoppedByProgram(const std::string& what_arg)
	: std::runtime_error(what_arg)
	{}
};

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
	std::memset(ram.data(), 0, ram.size());

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
				throw StoppedByProgram("stopped by program");
			}
		}
	);

	emulator.Reset();
	bool execution_error = false;
	try {
		emulator.Run(100'000'000);
	}catch (StoppedByProgram const&) {
		// Graceful stop, nothing to do
	}catch (std::runtime_error const& e) {
		std::cerr << "stopped by " << e.what() << '\n';
		execution_error = true;
	}

	// Print result
	std::cout << '{';
	std::cout << "\"cycle_count\": " << emulator.cycleCount << ',';
	std::cout << "\"ram\": [";
	for (size_t i = 0; i < ram.size(); ++i) {
		if (i != 0) {
			std::cout << ", ";
		}
		std::cout << uint16_t(ram[i]);
	}
	std::cout << "]";
	std::cout << "}\n";

	return execution_error ? 1 : 0;
}
