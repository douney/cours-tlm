
#include "generator.h"
#include "const.h"
#include "LCDC_registermap.h"

using namespace std;
using namespace sc_core;

Generator::Generator(sc_module_name name) : sc_module(name) {
	SC_THREAD(thread);
}

void Generator::thread(void) {

	this->initiator.write(LCDC_ADDR_REG, MEMORY_LCD_BASE);
	this->initiator.write(LCDC_START_REG, 1);

	ensitlm::addr_t mem_addr = MEMORY_LCD_BASE;
	ensitlm::addr_t rom_addr = ROM_BASE;
	ensitlm::data_t pix4b, pix8b;

	for (int i = 0 ; i < IMG_SIZE_32 ; i++) {
		this->initiator.read(rom_addr, pix4b);
		

		
		this->initiator.write(addr, pix8b);
		addr += 4;
		rom_addr += 4;
	}

}