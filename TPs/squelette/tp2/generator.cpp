
#include "generator.h"
#include "const.h"
#include "LCDC_registermap.h"

using namespace std;
using namespace sc_core;

Generator::Generator(sc_module_name name)
	: sc_module(name) 
	, irq_received(false)
	, decalage(0)
{
	SC_THREAD(thread);
	SC_THREAD(animation_thread);
	// Quand la valeur de irq change, irq_handler est appelé
	SC_METHOD(irq_handler);
	sensitive << irq;
}

void Generator::thread(void) {
	ensitlm::addr_t mem_addr = MEMORY_LCD_BASE;
	ensitlm::addr_t rom_addr = ROM_BASE;
	ensitlm::data_t pix4b, pix8b;

	this->initiator.write(LCDC_ADDR_REG, MEMORY_LCD_BASE);
	this->initiator.write(LCDC_START_REG, 1);

	while(1) {

		rom_addr = ROM_BASE;
		// calcul du début de l'image par rapport au décalage
		mem_addr = MEMORY_LCD_BASE + decalage * WIDTH;

		// Je remet le registre des interruptions à 0 comme indiqué dans la doc
		this->initiator.write(LCDC_INT_REG, 0);

		for (int i = 0 ; i < IMG_SIZE_32 / 2 ; i++) {
			this->initiator.read(rom_addr, pix4b);

			pix8b = 0;
			pix8b = pix8b | ( pix4b & 0xF0000000);
			pix8b = pix8b | ((pix4b & 0x0F000000) >> 4);
			pix8b = pix8b | ((pix4b & 0x00F00000) >> 8);
			pix8b = pix8b | ((pix4b & 0x000F0000) >> 12);
			this->initiator.write(mem_addr, pix8b);

			pix8b = 0;
			pix8b = pix8b | ((pix4b & 0x0000F000) << 16);
			pix8b = pix8b | ((pix4b & 0x00000F00) << 12);
			pix8b = pix8b | ((pix4b & 0x000000F0) << 8);
			pix8b = pix8b | ((pix4b & 0x0000000F) << 4);
			this->initiator.write(mem_addr + 4, pix8b);

			// Recalcul de l'adresse
			mem_addr = mem_addr + 8;
			// Si on arrive à la fin de l'écran
			if (mem_addr >= MEMORY_LCD_SIZE + MEMORY_LCD_BASE)
				mem_addr = MEMORY_LCD_BASE;
			rom_addr += 4;
		}

		while (!this->irq_received) {
			wait(1, SC_MS);
		}

		this->irq_received = false;
		cout << "IRQ received" << endl;
	}
}

void Generator::animation_thread(void) {
	// Permet de gérer la vitesse de l'animation sans géner les FPS
	while(1) {
		wait(100, SC_MS);
		this->decalage = (this->decalage + 1) % HEIGHT;
	}
}

void Generator::irq_handler(void) {
	this->irq_received = true;
}
