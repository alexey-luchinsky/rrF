# *******************************************************************************
#  Copyright 2019 Alexey Luchinsky                                              *
#    email: alexey.luchinsky@gmail.com                                          *
#                                                                               *
#  This program is free software: you can redistribute it and/or modify         *
#  it under the terms of the GNU General Public License as published by         *
#  the Free Software Foundation, either version 3 of the License, or            *
#  (at your option) any later version.                                          *
#                                                                               *
#  It is distributed in the hope that it will be useful,                        *
#  but WITHOUT ANY WARRANTY; without even the implied warranty of               *
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
#  GNU General Public License for more details.                                 *
#                                                                               *
#  You should have received a copy of the GNU General Public License            *
#  along with this program.  If not, see   <https://www.gnu.org/licenses/>.     *
# ******************************************************************************/


import uproot as up
import matplotlib.pyplot as plt
import numpy as np
import awkward as ak
import re
import pandas as pd

# loading evt.pdl file for particles' names
particles_names = None
tex_names = {"B_c+":r"B_c^+", "pi+":r"\pi^+", "pi-":r"\pi^-", "pi0":r"\pi^0",
            "K+":r"K^+","K-":r"K^-",
            "J/psi":r"J/\psi",
            "tau-":r"\tau^-", "e-":"e^-", "nu_tau":r"\nu_\tau", "anti-nu_e":r"\bar{\nu}_e"}

def load_evt_pdl(file_name = "../c++/src/evt.pdl"):
    global particles_names
    with open(file_name,"r") as file:
        evt_lines = file.readlines()
    particles = [L.split()[3:5] for L in evt_lines if 'add ' in L]
    particles_names = {int(code):name for [name, code] in particles}
load_evt_pdl()    

class rrFpy_pandas:
    def __init__(self, file_name=None):
        if type(file_name) == str:
            self.load_ROOT(file_name)
        else:
            self.df_pivoted = pd.DataFrame()
            self._filter = pd.Series(dtype = bool)
            
            
    def load_ROOT(self, file_name):
        """
        Loads the ROOT file created by simpleEvtGenRO programs and fills all fields
        """
        file = up.open(file_name)
        ntp = file["ntp"]
        df = pd.DataFrame(index = ak.to_pandas(ntp["pz"].array()).index, columns=["px", "py", "pz", "E", \
                                                                                  "Id", "nDau", "DF", "DL"])
        df["px"] = ak.to_pandas(ntp["px"].array())
        df["py"] = ak.to_pandas(ntp["py"].array())
        df["pz"] = ak.to_pandas(ntp["pz"].array())
        df["E"] = ak.to_pandas(ntp["E"].array())
        df["id"] = ak.to_pandas(ntp["Id"].array())
        df["nDau"] = ak.to_pandas(ntp["nDau"].array())
        df["DF"] = ak.to_pandas(ntp["DF"].array())
        df["DL"] = ak.to_pandas(ntp["DL"].array())
        df = df.reset_index()
        self._df_pivoted = pd.pivot_table(df, index = "entry", columns="subentry", values = ["px","py", "pz","E", \
                                                                                             "id", "nDau", "DF", "DL"])
        self._df_pivoted["nTrk"] = ak.to_pandas(ntp["nTrk"].array())
        self._filter = pd.Series(index=self._df_pivoted.index, data=True)
        file.close()
        
    def sign(self, x):
        return -1 if x<0 else 1
    
    def parse_var(self, var):
        """
        parse_var(var): parses variables into name and list of particles
        """
        [name, numbers] = var.split("_")
        part_list = [int(n) for n in re.sub('([0-9])',r'\1 ',numbers.replace('m','-')).split(' ')[:-1]]
        return name, part_list
    
    def get_momDF_from_partList(self, part_list):
        """
        Constructs new dataFrame with 'px', 'py', 'pz', 'E' fields polulated with components of 
        total momentum that corresponds to part_list
        """
        df_res = pd.DataFrame(index=self._df_pivoted.index, columns = ["px","py","pz","E", "out"]).fillna(0)
        for i in part_list:
            abs_i = abs(i)
            for v in ["px","py","pz","E"]:
                df_res[v] += self.sign(i)*self._df_pivoted[v,abs_i]
        return df_res
        
    def __getitem__(self, key):
        """
        [var] operator: constructs the table of the variable for given dataset (taking cuts into account)
        """
        if type(key) == str:   # extract one var as a Series
            if key == "ntr":
                df_res = self._df_pivoted["nTrk"]
                df_res = df_res[ self._filter]
                return df_res
            elif key == "reac":
                df_res = self[["tex_%d" % i for i in range(self["ntr"].max())]]
                df_res = df_res.apply(lambda r: 
                                      ("$"+r[0]+r"\to "+" ".join(r[1:])+"$").replace("???","")
                                      , axis=1)
                df_res = df_res[ self._filter]
                return df_res
            name, part_list = self.parse_var(key)
            mom_functions_dict =  {'m2': lambda t, x, y, z: t**2-x**2-y**2-z**2,
                'm': lambda t, x, y, z: np.sqrt(t**2-x**2-y**2-z**2),
                'pt2': lambda t, x, y, z: x**2+y**2,
                 'pt': lambda t, x, y, z: np.sqrt(x**2+y**2),
                  'E': lambda t, x, y, z: t,
                  'px': lambda t, x, y, z: x,
                  'py': lambda t, x, y, z: y,
                  'pz': lambda t, x, y, z: z};
            if name in mom_functions_dict.keys():
                func =mom_functions_dict[name]
                df_res = self.get_momDF_from_partList(part_list)
                df_res = df_res[ self._filter]
                return func(df_res["E"], df_res["px"], df_res["py"], df_res["pz"])
            elif name in ["id", "nDau", "DF", "DL"]:
                df_res = self._df_pivoted[name, abs(part_list[0])]
                df_res = df_res[ self._filter]
                return df_res
            elif name == "name":
                df_res = self._df_pivoted["id",abs(part_list[0])]
                df_res = df_res.fillna("0").astype(int)
                df_res = df_res.apply(lambda i: particles_names.get(i,"???"))
                df_res = df_res[ self._filter]
                return df_res
            elif name == "tex":
                df_res = self._df_pivoted["id",abs(part_list[0])]
                df_res = df_res.fillna("0").astype(int)
                df_res = df_res.apply(lambda i: particles_names.get(i,"???"))
                df_res = df_res.apply(lambda n: tex_names.get(n,n))
                df_res = df_res[ self._filter]
                return df_res
        elif type(key) == list:  # exctract many vars and DataFrame
            df_all =  pd.concat([self[k] for k in key], axis=1)
            df_all.columns = key
            return df_all
    
    def reset_cuts(self):
        """
        Resets all filters set by previous cuts
        """
        self._filter = pd.Series(index=self._df_pivoted.index, data=True)

    def cut(self, condition, in_place = False):
        """
        cut(condition): imposes a cut to the given dataset. Can be restored with reset_cuts
        """
        new_filter = self._filter.copy()
        if type(condition) == str:
            condition = re.sub('([<>=])',r' \1 ', condition)
            var, operation, lhs = [s for s in condition.split(' ') if len(s)>0]
            if(operation == '>'):
                lhs = float(lhs)
                new_filter = self._filter & (self[var] > lhs) 
            elif operation == '<':
                lhs = float(lhs)
                new_filter = self._filter & (self[var] < lhs) 
            elif operation == '=':
                if '+-' in lhs:
                    lhs, error = [float(n) for n in lhs.split('+-')]
                else:
                    lhs = float(lhs)
                    error = 1e-3
                new_filter = self._filter & \
                    (self[var] > lhs - error) & (self[var] < lhs + error)
            result = rrFpy_pandas(None)
            result._df_pivoted = self._df_pivoted
            result._filter = new_filter.copy()
        elif type(condition) == list:
            result = rrFpy_pandas(None)
            result._df_pivoted = self._df_pivoted
            result._filter = self._filter.copy()
            for c in condition:
                result = result.cut(c)
        return result

    def filter(self, new_filter):
        result = rrFpy_pandas(None)
        result._df_pivoted = self._df_pivoted
        result._filter = new_filter.copy()
        return result
    
    def size(self):
        return np.count_nonzero(self._filter)
    
    def remove_decayed(self):
        # what particles are final (did not decay, DF<0)?
        df = self._df_pivoted
        df = df[ self._filter]
        final_index = df["DF"][ df["DF"]<0].dropna(axis=1).columns
        final_index = final_index.insert(0,0)
        # extracting only these columns
        cols = [(v,i) for v in ["px", "py", "pz", "E", "id","DF", "DL"] for i in final_index]
        final_df = df[cols]
        # renaming the indeces (final momenta should be indexed 0,1,2,3,...)
        idx = final_df.columns
        lev0, lev1 = [i[0] for i in idx], [i[1] for i in idx]
        dict_lev1 = {L:i for i, L in enumerate( np.unique(lev1))}
        lev1 = [dict_lev1[L] for L in lev1]
        new_idx = pd.MultiIndex.from_arrays([lev0, lev1])
        # creating the new dataset with the renamed indexes
        root_final = rrFpy_pandas()
        root_final._df_pivoted = final_df.set_axis(new_idx, axis = 1, inplace=False).copy()
        root_final.reset_cuts()
        root_final._df_pivoted["nTrk"] = root_final._df_pivoted["id"].shape[1]
        root_final._df_pivoted["DF", 0] = 1
        root_final._df_pivoted["DL", 0] = len(final_index)-1
        return root_final
    
    def PDT(self, iEv=0, i=0, offset=0, separator_char=' ', print_number=True, decay_char = "*"):
        record =self._df_pivoted.iloc[iEv][["id","DF","DL"]]
        self.PDT_(record, iEv=iEv, i=i, offset=offset, separator_char=separator_char, 
            print_number=print_number, decay_char=decay_char)

    def PDT_(self, record, iEv=0, i=0, offset=0, separator_char=' ', print_number=True, decay_char = "*"):
        pdgID = int(record["id", i])
        name = particles_names.get(pdgID, pdgID)
        df = int(record["DF", i])
        dl = int(record["DL", i])
        print(i if print_number else "", "".join([separator_char]*offset), name, decay_char if df >=0 else "")
        if df >=0:
            for d in range(df, dl+1):
                self.PDT_(record, iEv, d, offset+3, print_number=print_number, 
                    separator_char=separator_char, decay_char=decay_char)
