//
// Created by max on 26.04.24.
//

#ifndef SPEAR_COLOR_H
#define SPEAR_COLOR_H


#include <string>

class Color {

public:
    unsigned red;
    unsigned blue;
    unsigned green;

    Color(unsigned r, unsigned g, unsigned b);

    static Color interpolate(Color a, Color b, double amount);

    static unsigned long toHexVal(Color c);

    static std::string toHtmlColor(Color c);

};


#endif //SPEAR_COLOR_H
