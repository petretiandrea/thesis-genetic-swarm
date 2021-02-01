import math
import numpy as np
from typing import *
from numba import njit

def probability_from_frequency(events) -> List[float]:
    symbols = dict.fromkeys(events)
    prob = [np.double(events.count(symbol)) / len(events) for symbol in symbols]
    return prob


def joint_probability_from_frequency(events1, events2) -> List[float]:
    symbols_x = dict.fromkeys(events1)
    symbols_y = dict.fromkeys(events2)

    events_xy = [f"{xi}{yi}" for xi, yi in zip(symbols_x, symbols_y)]
    symbols_xy = dict.fromkeys(events_xy)

    return [float(events_xy.count(c)) / len(events_xy) for c in symbols_xy]


def shannon(events) -> float:
    prob = probability_from_frequency(events)
    entropy = -sum([p * math.log(p) / math.log(2.0) for p in prob])
    return entropy


# MI[X,Y] = H[X] + H[Y] - H[X,Y]
def mutual_info(events_x, events_y) -> float:
    entropy_x = shannon(events_x)
    entropy_y = shannon(events_y)

    prob_xy = joint_probability_from_frequency(events_x, events_y)
    entropy_xy = -sum([p * math.log(p) / math.log(2.0) for p in prob_xy])

    return entropy_x + entropy_y - entropy_xy


def tuple_to_string(values) -> str: return ''.join([str(v) for v in values])


def sensor_tuples_to_string(sensor_tuples) -> List[str]:
    return [tuple_to_string(v) for v in sensor_tuples]


def cartesian_product(x, y):
    cross_product = np.transpose([np.tile(x, len(y)), np.repeat(y, len(x))])
    return cross_product
