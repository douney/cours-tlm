
#include "generator.h"

using namespace std;
using namespace sc_core;

Generator::Generator(sc_module_name name) : sc_module(name) {
	SC_THREAD(thread);
}

void Generator::thread(void) {


}