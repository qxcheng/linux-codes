#!usr/bin/python
#coding:utf-8

from ctypes import *
adder = CDLL("/home/yisheng/python/adder.so") #加载.so文件

res_int = adder.add_int(4,5)
print("Sum of 4 and 5 = " + str(res_int))

a = c_float(5.5) # 浮点数先转换
b = c_float(4.1)
add_float = adder.add_float
add_float.restype = c_float # 类型转换
print("Sum of 5.5 and 4.1 = ", str(add_float(a, b)))
