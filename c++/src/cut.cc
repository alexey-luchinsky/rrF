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
#include <vector>

#include "cut.h"

void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}



cut::cut(std::string s, std::vector<int> d) {
    string _s = s;
    descriptor_vec = d;
    ReplaceStringInPlace(_s,">"," > ");
    ReplaceStringInPlace(_s,"<"," < ");
    ReplaceStringInPlace(_s,"="," = ");
    ReplaceStringInPlace(_s,"+-"," +- ");
    vector<string> args = split_string(_s, " ");
    if(args.size() != 3 && args.size() !=5) {
        cout<<" WRONG FORMAT of the cut " << s << "!" << endl;
        return;
    };
    var = varFactory(args[0], &descriptor_vec);
    operation = args[1];
    value = atof(args[2].c_str());
    error = 1e-5;
    if(operation == "=" && args[3]=="+-") {
        error = atof(args[4].c_str());
    }
}

cut::cut(const cut& orig) {
}

cut::~cut() {
    delete var;
}

bool cut::is_ok(RrfEvent *event) {
    float v = var->getValue(event);
    if(operation == ">") {
        return v > value;
    }
    else if(operation == "<") {
        return v < value;
    }
    else if(operation == "=") {
        return value - error < v && v < value + error;
    }
    else {
        return true;
    }
}

