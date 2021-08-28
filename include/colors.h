# pragma once
#include <math.h>

namespace colors {
    float csin(float n) {    
        float acc = 0.0001, denominator, sinx, sinval;
        n = n * (3.142 / 180.0);  //convert in radian
        float temp = n;
        sinx = n;          
        sinval = sin(n);    
        int i = 1;
        do {
            denominator = 2 * i * (2 * i + 1);
            temp = -temp * n * n / denominator;
            sinx = sinx + temp;
            i = i + 1;
        } while (acc <= fabs(sinval - sinx));

        return sinx;
    }

    float ccos(float n) {
        float acc = 0.0001, temp, denominator, cosx, cosval;
        n = n * (3.142 / 180.0); //convert in radiam
        temp = 1;
        cosx = temp;          
        cosval = cos(n);
        int i = 1;
        do {
            denominator = 2 * i * (2 * i - 1);
            temp = -temp * n * n / denominator;
            cosx = cosx + temp;
            i = i + 1;
        } while (acc <= fabs(cosval - cosx));

        return cosx;
    }
}
