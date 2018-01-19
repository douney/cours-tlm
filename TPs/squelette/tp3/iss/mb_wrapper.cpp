/********************************************************************
 * Copyright (C) 2009, 2012 by Verimag                              *
 * Initial author: Matthieu Moy                                     *
 ********************************************************************/

#include "ensitlm.h"
#include "mb_wrapper.h"
#include "microblaze.h"
#include <iomanip>


/* Time between two step()s */
static const sc_core::sc_time PERIOD(20, sc_core::SC_NS);

//#define DEBUG
//#define INFO

using namespace std;

MBWrapper::MBWrapper(sc_core::sc_module_name name)
    : sc_core::sc_module(name), irq("irq"),
      m_iss(0) , /* identifier, not very useful since we have only one instance */
      interrupt(false)
{
	m_iss.reset();
	m_iss.setIrq(false);
	SC_THREAD(run_iss);
	SC_METHOD(interrupt_handler);
	sensitive << irq;
        /* The method that is needed to forward the interrupts from the SystemC
         * environment to the ISS need to be declared here */
}

/* IRQ forwarding method to be defined here */

void MBWrapper::exec_data_request(enum iss_t::DataAccessType mem_type,
                                  uint32_t mem_addr, uint32_t mem_wdata) {
	uint32_t localbuf;
	tlm::tlm_response_status status;
	switch (mem_type) {
	case iss_t::READ_WORD: {
		socket.read(mem_addr, localbuf);
#ifdef DEBUG
		std::cout << hex << "read    " << setw(10) << localbuf
		          << " at address " << mem_addr << std::endl;
#endif
		m_iss.setDataResponse(0, uint32_machine_to_be(localbuf));
	} break;
	case iss_t::READ_BYTE: {
    /* On lit l'adresse du début du mot */
		uint32_t i = mem_addr % 0x4;
		socket.read(mem_addr - i, localbuf);
		localbuf = uint32_machine_to_be(localbuf);
    /* On reprend la valeur de l'octet demandé */
		localbuf = (localbuf >> (i * 8)) & 0xFF;
		m_iss.setDataResponse(0, localbuf);
	} break;
	case iss_t::WRITE_HALF:
	case iss_t::WRITE_BYTE:
	case iss_t::READ_HALF:
		// Not needed for our platform.
		std::cerr << "Operation " << mem_type << " unsupported for "
		          << std::showbase << std::hex << mem_addr << std::endl;
		abort();
	case iss_t::LINE_INVAL:
		// No cache => nothing to do.
		break;
	case iss_t::WRITE_WORD: {
		socket.write(mem_addr, uint32_be_to_machine(mem_wdata));

#ifdef DEBUG
		std::cout << hex << "wrote   " << setw(10) << mem_wdata
		          << " at address " << mem_addr << std::endl;
#endif
		m_iss.setDataResponse(0, 0);
	} break;
	case iss_t::STORE_COND:
		break;
	case iss_t::READ_LINKED:
		break;
	}
}

void MBWrapper::run_iss(void) {

	int inst_count = 0;

	while (true) {
		if (m_iss.isBusy()) {
			m_iss.nullStep();
		}
		else {
			bool ins_asked;
			uint32_t ins_addr;
			m_iss.getInstructionRequest(ins_asked, ins_addr);

			if (ins_asked) {
				uint32_t localbuf;
				socket.read(ins_addr, localbuf);
				m_iss.setInstruction(0, uint32_machine_to_be(localbuf));
			}

			bool mem_asked;
			enum iss_t::DataAccessType mem_type;
			uint32_t mem_addr;
			uint32_t mem_wdata;
			m_iss.getDataRequest(mem_asked, mem_type, mem_addr,
			                     mem_wdata);

			if (mem_asked) {
				exec_data_request(mem_type, mem_addr,
				                  mem_wdata);
			}

			m_iss.step();
            /* On compte 5 step pendant l'interruption */
            if (interrupt) {
            	inst_count++;
            	if (inst_count >= 5) {
            		inst_count = 0;
            		interrupt = false;
            		m_iss.setIrq(false);
            	}
            }
		}

		wait(PERIOD);
	}
}


void MBWrapper::interrupt_handler(void) {
	m_iss.setIrq(true);
	interrupt = true;
}
