#!/usr/bin/env python
import collections
import os
import sys
from concurrent.futures import ThreadPoolExecutor

import numpy as np
import pandas as pd

sys.path.append(f'{os.environ["OMNETPP_HOME"]}/python')

# noinspection PyUnresolvedReferences
from omnetpp.scave import results as res, vectorops as ops


def get_last_value(df: pd.DataFrame) -> np.float128:
    vec = list(df.iloc[0]["vecvalue"])
    return np.float128(vec[len(vec) - 1])


# je Auto differenz bilden, quadrieren, durchschnitt
# Autos aufaddieren
# Durchschnitt über reps bilden
# je Szenario durchschnitt bilden

def get_constant_headway(run_ids: list[str]) -> np.float128:
    run_filter = f"(run =~ {run_ids[0]}"
    for i in range(1, len(run_ids)):
        run_filter += f" OR run =~ {run_ids[0]}"
    run_filter += ")"

    runs = collections.defaultdict(set)
    for run_id in run_ids:
        runs[run_id.split('-')[0]].add(run_id)

    name_filter = f"{run_filter} AND **.scenario.caccSpacing"
    headway = float(res.get_param_assignments(name_filter).iloc[0]["value"][0])

    value = (np.float128(0), 0)
    for scenario in runs.keys():
        for repetition in runs[scenario]:
            name_filter = f"run =~ {repetition} AND distance AND module =~ **.node[*].appl AND NOT module =~ **.node[" \
                          f"0].appl"
            vecs = res.get_vectors(name_filter)
            vecs = ops.expression(vecs, f"(y - {headway}) ** 2")
            vecs = ops.mean(vecs)
            vecs = ops.aggregate(vecs, "sum")
            value = (value[0] + get_last_value(vecs), value[1] + 1)
    return value[0] / value[1]


def multithreaded(threads: int, directory: str, run_ids: list[list[str]]) -> list[np.float128]:
    res.set_inputs(directory)
    with ThreadPoolExecutor(max_workers=threads) as pool:
        results = pool.map(get_constant_headway, run_ids)
    return list(results)
