# -*- coding:utf-8 -*-
# @FileName : tmp.py
# @Time : 2024/3/15 9:14
# @Author : fiv

from multiprocessing import Pool


def fn(num):
    return num * 2


with Pool(4) as p:
    print(p.map(fn, [1, 2, 3, 4, 5]))
