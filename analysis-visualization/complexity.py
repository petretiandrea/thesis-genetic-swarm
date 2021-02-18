import math
import numpy as np
from typing import *


def probability_from_frequency(events) -> Dict[str, float]:
    symbols = dict.fromkeys(events)
    prob = { symbol : (np.double(events.count(symbol)) / len(events)) for symbol in symbols }
    return prob


def joint_probability_from_frequency(events1, events2) -> Dict[str, float]:
    events_xy = [f"{xi}{yi}" for xi, yi in zip(events1, events2)]
    symbols_xy = dict.fromkeys(events_xy)

    return {c : (float(events_xy.count(c)) / len(events_xy)) for c in symbols_xy }


def shannon(events) -> float:
    prob = probability_from_frequency(events)
    entropy = -sum([p * math.log2(p) for (k, p) in prob.items()])
    return entropy


# MI[X,Y] = H[X] + H[Y] - H[X,Y]
def mutual_info(events_x, events_y) -> float:
    entropy_x = shannon(events_x)
    entropy_y = shannon(events_y)

    prob_xy = joint_probability_from_frequency(events_x, events_y)
    entropy_xy = -sum([p * math.log2(p) for (k, p) in prob_xy.items()])

    return entropy_x + entropy_y - entropy_xy


def tuple_to_string(values) -> str: return ''.join([str(v) for v in values])


def sensor_tuples_to_string(sensor_tuples) -> List[str]:
    return [tuple_to_string(v) for v in sensor_tuples]


def cartesian_product(x, y):
    cross_product = np.transpose([np.tile(x, len(y)), np.repeat(y, len(x))])
    return cross_product


from itertools import combinations
if __name__ == "__main__":
    import pandas as pd
    import ast

    df = pd.read_csv("./collected-data/complexity/task1/temp/task1_bis_worst_0.csv", ";", index_col=False)
    trial_x = df[df["trial"] == 0]
    trial_x.reset_index(drop=True, inplace=True)

    pairs = list(combinations(np.arange(0, len(trial_x["bot_id"]), 1), 2))

    mi = []
    for (xi, yi) in pairs:
        x = ast.literal_eval(trial_x.loc[xi]["motor_value"])
        y = ast.literal_eval(trial_x.loc[yi]["motor_value"])
        mi.append(mutual_info(x, y))

    # def cosine_dist(df, trial) -> float:
    #     from itertools import combinations
    #     from scipy.spatial import distance
    #     trial_x = df[df["trial"] == trial]
    #     trial_x.reset_index(drop=True, inplace=True)
    #     pairs = list(combinations(np.arange(0, len(trial_x["bot_id"]), 1), 2))
    #
    #     cosines = []
    #     for (xi, yi) in pairs:
    #         x = ast.literal_eval(trial_x.loc[xi]["motor_value"])
    #         y = ast.literal_eval(trial_x.loc[yi]["motor_value"])
    #         cosines.append(distance.cosine(np.array(x), np.array(y)))
    #
    #     return np.mean(cosines)
    #
    # print(cosine_dist(trial_x, 0))