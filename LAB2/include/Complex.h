//
// Created by Илья Атмажитов on 22.11.2024.
//

#ifndef LABORATORY2_COMPLEX_H
#define LABORATORY2_COMPLEX_H

#include <iostream>

class Complex {
public:
    double re;
    double im;

    Complex(double re = 0, double im = 0): re(re), im(im) {}

    bool operator>(Complex & other) {
        if (re != other.re) {
            return re > other.re;
        }
        if (im != other.im) {
            return im > other.im;
        }
    }

    bool operator<(Complex & other) {
        if (re != other.re) {
            return re < other.re;
        }
        if (im != other.im) {
            return im < other.im;
        }
    }
};

#endif //LABORATORY2_COMPLEX_H
