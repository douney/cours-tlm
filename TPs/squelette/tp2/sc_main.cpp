
#include "bus.h"
#include "generator.h"
#include "ensitlm.h"
#include "memory.h"
#include "LCDC.h"
#include "const.h"
#include "ROM.h"


int sc_main(int argc, char **argv) {

	Generator generator("Generator1");
	Memory memory("Memory1", MEMORY_SIZE);
	Bus bus("Bus");
	LCDC lcd("LCD", sc_core::sc_time(1.0 / 25.0, sc_core::SC_SEC));
	ROM rom("Rom");
	sc_core::sc_signal <bool, sc_core::SC_MANY_WRITERS> irq_signal("IRQ");

	bus.map(memory.target, MEMORY_BASE, MEMORY_SIZE);
	bus.map(lcd.target_socket, LCD_REG_BASE, LCD_REG_SIZE);
	bus.map(rom.socket, ROM_BASE, ROM_SIZE);

	bus.initiator.bind(lcd.target_socket);
	bus.initiator.bind(rom.socket);
	bus.initiator.bind(memory.target);

	generator.initiator.bind(bus.target);
	lcd.initiator_socket.bind(bus.target);

	generator.irq(irq_signal);
	lcd.display_int(irq_signal);
		
	sc_core::sc_start();
	return 0;
}