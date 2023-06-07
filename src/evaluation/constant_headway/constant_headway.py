#!/usr/bin/env python

import os
import sys

import numpy as np
import pandas as pd

sys.path.append(f'{os.environ["OMNETPP_HOME"]}/python')  # TODO: bei Installation $OMNETPP_HOME setzen (.profile)

# noinspection PyUnresolvedReferences
from omnetpp.scave import results as res, vectorops as ops


def get_last_value(df: pd.DataFrame) -> float:
    l = list(df.iloc[0]["vecvalue"])
    return l[len(l) - 1]


# je Auto differenz bilden, quadrieren, durchschnitt
# Autos aufaddieren
# Durchschnitt über reps bilden
# je Szenario durchschnitt bilden

def getConstantHeadway(resultdir: str) -> np.float128:
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
    return np.float128(sum(scenes))
