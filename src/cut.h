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
#include "RrfEvent.h"

using namespace std;


std::vector<std::string> split_string(std::string str, std::string sep);
void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);
float calc_var(RrfEvent *event, string var);

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

    string get_var() {
        return var;
    }

    /**
     * bool is_ok()
     * checks if the current event passes the cut
     */
    bool is_ok(RrfEvent *event);
private:
    std::string var;
    string operation;
    double value;
    double error;
};

#endif /* CUT_H */

