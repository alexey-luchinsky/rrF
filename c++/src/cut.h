/*******************************************************************************
* Copyright 2019 Alexey Luchinsky                                              *
*   email: alexey.luchinsky@gmail.com                                          *
*                                                                              *
* This program is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by         *
* the Free Software Foundation, either version 3 of the License, or            *
* (at your option) any later version.                                          *
*                                                                              *
* It is distributed in the hope that it will be useful,                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU General Public License for more details.                                 *
*                                                                              *
* You should have received a copy of the GNU General Public License            *
* along with this program.  If not, see   <https://www.gnu.org/licenses/>.     *
*******************************************************************************/

#ifndef CUT_H
#define CUT_H

#include <iostream>
#include "RrfEvent.h"
#include "RrfVar.h"

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
    cut(std::string s, std::vector<int> d);
    cut(const cut& orig);
    virtual ~cut();

    string get_var() {
        return var->to_string();
    }

    /**
     * bool is_ok()
     * checks if the current event passes the cut
     */
    bool is_ok(RrfEvent *event);
private:
    RrfVar *var;
    string operation;
    double value;
    double error;
    std::vector<int> descriptor_vec;
};

#endif /* CUT_H */

