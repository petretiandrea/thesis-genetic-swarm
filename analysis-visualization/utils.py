import math
import numpy as np
from typing import *
import pandas as pd
import os

# result_task1_nodes50_k3_bias0.21_prox0_p80_g100_pM0.03_pC0_el5_trials3_21-39_19-01.csv
def info_from_filename(filename: str, prefix='') -> Dict[str, Any]:
    basename = os.path.basename(filename)
    pieces = basename.replace(prefix, '').split('_')
    return {
        'nodes': pieces[1][5:],
        'k': pieces[2][1:],
        'bias': pieces[3][4:],
        'proximity': pieces[4][4:],
        'population': pieces[5][1:],
        'generations': pieces[6][1:],
        'mutation': pieces[7][2:],
        'crossover': pieces[8][2:],
        'elitism': pieces[9][2:],
        'trials': pieces[10][6:]
    }


def legend_from_filename(filename: str, prefix='') -> str:
    info = info_from_filename(filename, prefix)
    return "Prox: %s\nGen: %s\nBias: %s\nMut: %s\nCross: %s\nPop: %s" % (
        info['proximity'], info['generations'], info['bias'], info['mutation'], info['crossover'], info['population'])


def avg_fitness_from_file(filename: str, csv_header):
    data = pd.read_csv(filename, ";", skiprows=1, index_col=False, names=csv_header)
    return avg_fitness_from_dataframe(data)


def avg_fitness_from_dataframe(data: pd.DataFrame):
    data = data.drop(['genome'], axis=1)
    return data.groupby(['generation']).mean()


def avg_fitness_from_files(*filenames, csv_header):
    means = []
    for filename in filenames:
        data = pd.read_csv(filename, ";", skiprows=1, index_col=False, names=csv_header)
        data.drop(["genome", "score"], axis=1, inplace=True)
        means.append(data.groupby(['generation']).mean())
    return means
