#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Author: qxcheng
# Datetime: 2019/12/20 11:20
# File: ga.py

"""
问题：使用遗传算法求解给定区间内函数的最大值。

函数：y = -x^2+2x+5
区间：[-32,31]

"""

import random


class Chromosome(object):
    def __init__(self, chrom=None):
        # 随机初始化一个染色体
        if chrom == None:
            self.chrom = []
            for _ in range(6):
                _num = random.randint(0, 1)
                self.chrom.append(_num)
        # 直接赋值一个染色体
        else:
            self.chrom = chrom

    def fit_score(self):
        _str = ''
        for i in self.chrom:
            _str += str(i)
        _num = int(_str, 2)
        return 5 - _num * _num + _num * 2

    def cross(self, Chrom, cross_rate):
        cross_idx = random.randint(0, 5)
        child_chrom1 = self.chrom[:cross_idx] + Chrom.chrom[cross_idx:]
        child_chrom2 = Chrom.chrom[:cross_idx] + self.chrom[cross_idx:]
        return Chromosome(child_chrom1), Chromosome(child_chrom2)

    def vary(self, vary_rate):
        _num = random.randint(1, 10)
        if _num <= 10*vary_rate:
            cross_idx = random.randint(0, 5)
            if self.chrom[cross_idx] == 0:
                self.chrom[cross_idx] = 1
            else:
                self.chrom[cross_idx] = 0


def one_iter(population, cross_rate=0.8, vary_rate=0.2):
    # 选择使用排序法      取适应度最高值
    # 交叉使用随机单点交叉 cross_rate=交叉率 (暂未开启)
    # 变异使用随机反转变异 vary_rate=变异率
    population.sort(key=lambda x:x.fit_score(), reverse=True)
    population.pop()
    population.pop()

    child_chrom1, child_chrom2 = population[0].cross(population[1], cross_rate)
    child_chrom1.vary(vary_rate)
    child_chrom2.vary(vary_rate)

    population.append(child_chrom1)
    population.append(child_chrom2)


def main():
    population = [Chromosome(), Chromosome(), Chromosome(), Chromosome()]

    for _ in range(50):
        one_iter(population)

    population.sort(key=lambda x: x.fit_score(), reverse=True)
    print(population[0].chrom, population[0].fit_score())

if __name__ == '__main__':
    main()





