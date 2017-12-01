#include "ensitlm.h"
#include "generator.h"

// #define DEBUG

// Constructor
Generator::Generator(sc_core::sc_module_name name, unsigned int size)
    : sc_module(name), m_size(size) {
	storage = new ensitlm::data_t[size / sizeof(ensitlm::data_t)];
}

// Destructor
Generator::~Generator() {
	delete[] storage;
}


