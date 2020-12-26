import uproot as up
import matplotlib.pyplot as plt
import numpy as np
import awkward as ak
import re
import pandas as pd
from uproot3_methods.classes.TLorentzVector import TLorentzVector
from tqdm.notebook import tqdm
import pandas as pd
import plotly.express as ps
import plotly.graph_objects as go

# loading evt.pdl file for particles' names
particles_names = None
tex_names = {"B_c+":r"B_c^+", "pi+":r"\pi^+", "pi-":r"\pi^-", "pi0":r"\pi^0",
            "K+":r"K^+","K-":r"K^-",
            "J/psi":r"J/\psi"}
def load_evt_pdl(file_name = "../src/evt.pdl"):
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
        df = pd.DataFrame(index = ak.to_pandas(ntp["pz"].array()).index, columns=["px", "py", "pz", "E", "Id"])
        df["px"] = ak.to_pandas(ntp["px"].array())
        df["py"] = ak.to_pandas(ntp["py"].array())
        df["pz"] = ak.to_pandas(ntp["pz"].array())
        df["E"] = ak.to_pandas(ntp["E"].array())
        df["Id"] = ak.to_pandas(ntp["Id"].array())
        df = df.reset_index()
        self._df_pivoted = pd.pivot_table(df, index = "entry", columns="subentry", values = ["px","py", "pz","E", "Id"])
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
                 'pt': lambda t, x, y, z: np.sqrt(x**2+y**2)};
            if name in mom_functions_dict.keys():
                func =mom_functions_dict[name]
                df_res = self.get_momDF_from_partList(part_list)
                df_res = df_res[ self._filter]
                return func(df_res["E"], df_res["px"], df_res["py"], df_res["pz"])
            elif name == "id":
                df_res = self._df_pivoted["Id", abs(part_list[0])]
                df_res = df_res[ self._filter]
                return df_res
            elif name == "name":
                df_res = self._df_pivoted["Id",abs(part_list[0])]
                df_res = df_res.fillna("0").astype(int)
                df_res = df_res.apply(lambda i: particles_names.get(i,"???"))
                df_res = df_res[ self._filter]
                return df_res
            elif name == "tex":
                df_res = self._df_pivoted["Id",abs(part_list[0])]
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
