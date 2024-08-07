//
// Created by maximiliank on 04.05.24.
//

#ifndef SPEAR_DEMANGLER_H
#define SPEAR_DEMANGLER_H

#include <string>
#include <llvm/Demangle/Demangle.h>


class DeMangler {
public:
    static std::string demangle(std::string mangledName);
};


#endif //SPEAR_DEMANGLER_H
