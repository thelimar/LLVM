#include <iostream>

extern "C" {

    void traceMyInstr(char *instr_name) { std::cout << instr_name << ","; }

} // extern "C"