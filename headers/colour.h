//
// Created by will on 16/01/25.
//

#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"
#include "interval.h"


using colour = vec3;

inline double linear_to_gamma(double linear_comp) {
    if (linear_comp > 0) return std::sqrt(linear_comp);
    return 0;
}


inline void write_colour(std::ostream &out, const colour &pixel_colour) {
    double r = pixel_colour.x();
    double g = pixel_colour.y();
    double b = pixel_colour.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#include "rt_weekend.h"

#endif //COLOUR_H
