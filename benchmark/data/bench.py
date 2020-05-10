import pandas as pd
import sys,os
import plotly.graph_objects as go
from glob import glob
import numpy as np
import plotly.express as px

def merge(dir_path):
    list_of_csv = [csv for csv in glob('{}/*.csv'.format(dir_path))]
    pd_obj = pd.concat([pd.read_csv(file,sep=",") for file in list_of_csv])

    merged_csv = "{}/result.csv".format(dir_path)
    pd_obj.to_csv(merged_csv,index=False)

def plot(dir_path):
    list_result =  [os.path.abspath(name) for name in os.listdir(dir_path) if os.path.isdir(name) and os.path.isfile("{}/result.csv".format(name)) and os.access("{}/result.csv".format(name), os.R_OK)]
    colors = ['rgb(220, 90, 90)','rgb(130, 120, 90)','rgb(31, 127, 44)','rgb(31, 119, 180)', 'rgb(255, 127, 14)', 'rgb(44, 160, 44)', 'rgb(214, 39, 40)', 'rgb(148, 103, 189)', 'rgb(140, 86, 75)', 'rgb(227, 119, 194)', 'rgb(127, 127, 127)', 'rgb(188, 189, 34)', 'rgb(23, 190, 207)']
    
    names = []
    cpus_avg = []
    mems_avg = []
    for result in list_result:
        df = pd.read_csv("{}/result.csv".format(result),sep=",",header=0,encoding='utf-8')
        names.append(result.split('/')[-1])
        cpus_avg.append(df['%CPU'].mean())
        mems_avg.append(df['RSS'].mean()/1000)
    #avg = df['%CPU'].mean()
    fig_cpu = go.Figure(data=[
	go.Bar(name='%CPU usage',x=names,y=cpus_avg, marker_color=colors)
	])
    fig_cpu.update_layout(barmode='group')
    fig_cpu.show()

    fig_mem = go.Figure(data=[
        go.Bar(name='Mem usage(MB)',x=names,y=mems_avg, marker_color=colors)
        ])
    fig_mem.update_layout(barmode='group')
    fig_mem.show()


if __name__ == "__main__":
    if sys.argv[1] == '0':
        print("Merge files")
        merge(sys.argv[2])
    elif sys.argv[1] == '1':
        print("Plot graph")
        plot(sys.argv[2])
