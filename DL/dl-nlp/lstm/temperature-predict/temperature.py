import os
import sys
import time
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow import keras

#1.1 加载数据
temps = pd.read_csv("daily-minimum-temperatures-in-me.csv",
                    parse_dates=[0], index_col=0)  
'''                                        
print(temps.head())   
            Temperature
Date
1981-01-01         20.7
1981-01-02         17.9
1981-01-03         18.8
1981-01-04         14.6
1981-01-05         15.8     
'''           
#1.2 插入缺失日期数据
temps = temps.asfreq("1D", method="ffill") 

#1.3 制作特征和标签序列
def add_lags(series, times):
    cols = []
    column_index = []
    for time in times:
        cols.append(series.shift(-time))
        lag_fmt = "t+{time}" if time > 0 else "t{time}" if time < 0 else "t"
        column_index += [(lag_fmt.format(time=time), col_name)
                        for col_name in series.columns]
    df = pd.concat(cols, axis=1)
    df.columns = pd.MultiIndex.from_tuples(column_index)
    return df
X = add_lags(temps, times=range(-30+1,1)).iloc[30:-5] 
y = add_lags(temps, times=[5]).iloc[30:-5]
'''
print(X)
               t-29        t-28        t-27        ...         t-2         t-1           t
           Temperature Temperature Temperature     ...     Temperature Temperature Temperature
Date                                               ...
1981-01-31     17.9        18.8        14.6        ...         15.5        15.1        15.4
1981-02-01     18.8        14.6        15.8        ...         15.1        15.4        15.3
print(y)     
               t+5
           Temperature
Date
1981-01-31     16.6
1981-02-01     16.8
'''

#1.4 划分数据集
train_slice = slice(None, "1986-12-25")
valid_slice = slice("1987-01-01", "1988-12-25")
test_slice = slice("1989-01-01", None)
X_train, y_train = X.loc[train_slice], y.loc[train_slice]
X_valid, y_valid = X.loc[valid_slice], y.loc[valid_slice]
X_test, y_test = X.loc[test_slice], y.loc[test_slice]

#1.5 数据集维度转换
def multilevel_df_to_ndarray(df):
    shape = [-1] + [len(level) for level in df.columns.remove_unused_levels().levels]
    return df.values.reshape(shape)
X_train_3D = multilevel_df_to_ndarray(X_train) #(2155,30)-->(2155,30,1)
X_valid_3D = multilevel_df_to_ndarray(X_valid)
X_test_3D = multilevel_df_to_ndarray(X_test)
'''
print(X_train_3D[0])  #30x1
[[14.6]
  [11.6]
  [11.2]
  ...
  [ 9.7]
  [13.4]
  [10.5]]
'''

input_shape = X_train_3D.shape[1:] #(30,1)
model1 = keras.models.Sequential()
model1.add(keras.layers.SimpleRNN(100, return_sequences=True, input_shape=input_shape))
model1.add(keras.layers.SimpleRNN(50))
model1.add(keras.layers.Dense(1))
model1.compile(loss="mse", optimizer=keras.optimizers.SGD(lr=0.005), metrics=["mae"])
model1.summary()
history1 = model1.fit(X_train_3D, y_train, epochs=200, batch_size=200,
                      validation_data=(X_valid_3D, y_valid))
model1.evaluate(X_valid_3D, y_valid)                      