#!/bin/python3

import pandas as pd
import sys,os
import plotly.graph_objects as go
from glob import glob
from datetime import datetime,timedelta

"""
Example of usage :
bench.py 0 /abs/path/to/dir/cipher   merge all csv in cipher in a unique file 
becnh.py 1 /abs/path/to/dir/         plot a bar chart with cpu and mem usage
becnh.py 2 /abs/path/to/file         plot bar chart for battery

TODO handle params
"""


colors = ['rgb(220, 90, 90)','rgb(130, 120, 90)','rgb(31, 127, 44)','rgb(31, 119, 180)', 'rgb(255, 127, 14)', 'rgb(44, 160, 44)', 'rgb(214, 39, 40)', 'rgb(148, 103, 189)', 'rgb(140, 86, 75)', 'rgb(227, 119, 194)', 'rgb(127, 127, 127)', 'rgb(188, 189, 34)', 'rgb(23, 190, 207)']

def merge(dir_path):
    list_of_csv = [csv for csv in glob('{}/*.csv'.format(dir_path))]
    for csv in list_of_csv:
        timer = csv.split(".")[0].split("/")[-1].split("-")[1:]
        file = pd.read_csv(csv,sep=",")
        d = datetime.strptime(file['Time'][0],"%H:%M:%S")
        max_time = d + timedelta(minutes=int(timer[1]),seconds=int(timer[2]))
        bad_rows=[idx for idx,time in enumerate(file['Time']) if not d <= datetime.strptime(time,"%H:%M:%S") <= max_time]
        file.drop(file.index[bad_rows],inplace=True)
        file.to_csv(csv,index=False)

    pd_obj = pd.concat([pd.read_csv(file,sep=",") for file in list_of_csv])

    merged_csv = "{}/result.csv".format(dir_path)
    pd_obj.to_csv(merged_csv,index=False)

def plot(dir_path):
    list_result =  ["{}/{}".format(dir_path,name) for name in os.listdir(dir_path) if os.path.isdir("{}/{}".format(dir_path,name)) and os.path.isfile("{}/{}/result.csv".format(dir_path,name)) and os.access("{}/{}/result.csv".format(dir_path,name), os.R_OK)]
    names = []
    cpus_avg = []
    mems_avg = []
    for result in list_result:
        df = pd.read_csv("{}/result.csv".format(result),sep=",",header=0,encoding='utf-8')
        names.append(result.split('/')[-1].capitalize())
        cpus_avg.append(round(df['%CPU'].mean(),2))
        mems_avg.append(round(df['RSS'].mean()/1000,2))

    fig_cpu = go.Figure(data=[
	go.Bar(name='%CPU usage',x=names,y=cpus_avg, marker_color=colors,text=cpus_avg, textposition='outside')
	])

    fig_cpu.update_layout(
            barmode='group',
            title="CPU usage",
            xaxis_title="algorithms",
            yaxis_title="Percentage",
           font=dict(
               family="Courier New, monospace",
               size=18,
               color="#7f7f7f"
    ))
    fig_cpu.show()

    fig_mem = go.Figure(data=[
        go.Bar(name='Mem usage(MB)',x=names,y=mems_avg, marker_color=colors,text=mems_avg, textposition='outside')
        ])

    fig_mem.update_layout(
            barmode='group', 
            title="Mem usage",
            xaxis_title="algorithms", 
            yaxis_title="MB", 
           font=dict(
               family="Courier New, monospace",
               size=18,
               color="#7f7f7f"
    ))
    fig_mem.show()
    
    if not os.path.exists("{}/result".format(dir_path)):
        os.mkdir("{}/result".format(dir_path))

    fig_cpu.write_image("{}/result/cpu.png".format(dir_path),width=1400,height=1000)
    fig_mem.write_image("{}/result/mem.png".format(dir_path),width=1400,height=1000)

def battery_plot(file_path):
    df = pd.read_csv(file_path,sep=",",header=0,encoding='utf-8')
    bats_avg = [df[name].mean() for name in list(df.columns)]

    fig_bat = go.Figure(data=[
        go.Bar(name='Accumulated Consumption ',x=list(df.columns),y=bats_avg, marker_color=colors, text=bats_avg,textposition='outside')
        ])
    fig_bat.update_layout(
            barmode='group',
            title="Accumulated Consumption",
            xaxis_title="algorithms",
            yaxis_title="mAh",
           font=dict(
               family="Courier New, monospace",
               size=18,
               color="#7f7f7f"
    ))

    fig_bat.show()

    if not os.path.exists("{}/result".format(file_path.rsplit("/",1)[0])):
        os.mkdir("{}/result".format(file_path.rsplit("/",1)[0]))

    fig_bat.write_image("{}/result/bat.png".format(file_path.rsplit("/",1)[0]))


if __name__ == "__main__":
    if sys.argv[1] == '0':
        print("Merge files")
        merge(sys.argv[2])
    elif sys.argv[1] == '1':
        print("CPU and MEM plot")
        plot(sys.argv[2])
    elif sys.argv[1] == '2':
        print("Battery plot")
        battery_plot(sys.argv[2])
