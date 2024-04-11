# -*- coding:utf-8 -*-
# @FileName : best_para.py
# @Time : 2024/3/22 19:23
# @Author : fiv

import os
import glob
from log_util import get_log
from multiprocessing import Pool

log, file_path = get_log("best_para")


def get_params():
    with open('./train.log', 'r') as f:
        lines = f.readlines()
        l = len(lines)
        result = []
        for i in range(l):
            if "Average score:" in lines[i]:
                avg_score = float(lines[i].split()[-1])
                params = lines[i - 3].split("params:")[-1].strip()
                params = ",".join(params.split())
                # if avg_score >= 240000:
                result.append((avg_score, params))
    result = sorted(result, key=lambda x: x[0], reverse=True)
    return result


# def train(path, params):
#     judge = "./PreliminaryJudge"
#     exe = "./main"
#     cmd = f'{judge} -m {path} "{exe} {params}" -l NONE'
#     result = os.popen(cmd).read()
#     output = dict(eval(result))
#     if output['status'] == 'Successful':
#         score = output['score']
#     else:
#         score = -1
#     return score
#
#
# def get_best_params():
#     params = get_params()
#     paths = sorted(glob.glob("./maps/*.txt"))
#     result = []
#     for param in params:
#         with Pool() as p:
#             score_list = p.starmap(train, [(path, param) for path in paths])
#         score_sum = sum(score_list)
#         avg_score = score_sum / len(score_list)
#         log.info(f"Params: {param}")
#         log.info(f"Score list: {score_list}")
#         log.info(f"Total score: {score_sum}")
#         log.info(f"Average score: {avg_score}")
#         result.append((avg_score, param))
#     result.sort(key=lambda x: x[0], reverse=True)
#     for i in result:
#         log.info(f"Average score: {i[0]}, Params: {i[1]}")


if __name__ == "__main__":
    params = get_params()
    for i in params:
        log.info(i)
