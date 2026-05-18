#ifndef FUNC_H
#define FUNC_H

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>



inline double GetExactValue(double x, int k){
    switch(k){
        case 0:
            return 1;
        case 1:
            return x;
        case 2:
            return pow(x,2);
        case 3:
            return pow(x,3);
        case 4:
            return pow(x,4);
        case 5:
            return exp(x);
        case 6:
            return (1.0/(25*pow(x,2)+1));
        default:
            throw std::invalid_argument("Error: Wrong formula number");
    }

}
void printVector(const std::vector<double>& vec, int m);
void initialize(std::vector<double>&x, std::vector<double>&f, int n, int k, double a, double b);
#endif
