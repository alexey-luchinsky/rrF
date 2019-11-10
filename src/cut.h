/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cut.h
 * Author: luchinsky
 *
 * Created on November 10, 2019, 7:25 AM
 */

#ifndef CUT_H
#define CUT_H

#include <iostream>
using namespace std;

float calc_var(std::string var);
std::vector<std::string> split_string(std::string str, std::string sep);

/**
 * cut
 * 
 * A class implementing cuts on the kinematic variable
 */
class cut {
public:
    cut(std::string s);
    cut(const cut& orig);
    virtual ~cut();
    
    /**
     * bool is_ok()
     * checks if the current event passes the cut
     */
    bool is_ok();
private:
    std::string var;
    string operation;
    double value;
};

#endif /* CUT_H */

