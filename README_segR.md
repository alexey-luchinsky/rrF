**segR** (from simpleEvtGenRO) library helps to simply test new EvtGen models without recompiling main EvtGen library

(c) Alexey Luchinsky

# Short Description


Often you do not want to recompile the whole EvtGen package to create and test the new model. In this case it is more convenient to keep new model's files in the separate directory (typically the one where you run simpleEvtGenRO and rrF programs), add this model in the simpleEvtGenRO program and link it with the main EvtGen library. The problem is that in this case you need to duplicate pretty long code of this generator several times. The library **segR** (abbreviation of simpleEvtGenRO) can simplify this task
