#!/usr/bin/env python

import argparse
import os

import numpy as np
import pandas as pd
from omnetpp.scave import results as res, vectorops as ops


def get_last_value(df: pd.DataFrame) -> float:
    l = list(df.iloc[0]["vecvalue"])
    return l[len(l) - 1]


# je Auto differenz bilden, quadrieren, durchschnitt
# Autos aufaddieren
# Durchschnitt über reps bilden
# je Szenario durchschnitt bilden

parser = argparse.ArgumentParser()

parser.add_argument("--path", "-p", help="Path to result directory")
args = parser.parse_args()

resultdir = args.path
if resultdir is None:
    resultdir = "results/"

scenarios = set()
for file in os.listdir(resultdir):
    scenarios.add(file.split('_')[0])

res.set_inputs(resultdir)

filter = "name =~ repeat"
repeat = int(res.get_config_entries(filter).iloc[0]["value"])

filter = "name =~ **.scenario.caccSpacing"
headway = float(res.get_param_assignments(filter).iloc[0]["value"][0])

scenes = []
for scenario in scenarios:
    reps = []
    for repetition in range(repeat):
        file = f"{scenario}_*_*_{repetition}.*"
        filter = f"file =~ {file} AND name =~ distance AND module =~ **.node[*].appl AND NOT module =~ **.node[0].appl"
        vecs = res.get_vectors(filter)
        vecs = ops.expression(vecs.loc[0:7], f"(y - {headway}) ** 2")
        vecs = ops.mean(vecs.loc[0:7])
        vecs = ops.aggregate(vecs, "sum")
        reps.append(vecs)
    scenes.append(ops.aggregate(pd.concat(reps, ignore_index=True)))
scenes = list(map(get_last_value, scenes))
print(str(np.float128(sum(scenes))))
