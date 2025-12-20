#This file has code for visualizing many of the results from the functions.py file
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from functions import simple_read
from pathlib import Path

#Function to format output of TM score output file as it has a different structure due to region and variant data combined
def get_tm_fig(file_path, fig_path, dpi=None):
    default_dpi = 90
    if dpi:
        default_dpi = dpi

    df = pd.read_csv(file_path)
    df.set_index('label', inplace=True)

    sns.heatmap(df, annot=True)
    plt.savefig(fig_path + Path(file_path).stem + '.png', bbox_inches='tight', dpi=default_dpi)
    return 

#All other output fies have the same format so one function can be used R:RMSD, I:IUPred, A:ANCHOR2
def get_rmsdhdr_fig(file_path, fig_path, dpi=None):
    default_dpi = 90
    if dpi:
        default_dpi = dpi
    df = pd.read_csv(file_path, header=None)
    df.set_index(0, inplace=True)

    df[(df >= 5.0)] = 5
    df[(df >= 3.0) & (df < 5.0)] = 3
    df[(df >= 1.0) & (df < 3.0)] = 1
    df[(df < 1.0)] = 0

    fig, ax = plt.subplots()
    im = ax.imshow(df, aspect='auto')

    # Show all ticks and label them with the respective list entries
    labels = df.index.values.tolist()
    ax.set_yticks(range(len(labels)), labels=labels)
    fig.tight_layout()
    fig.colorbar(im)
    plt.savefig(fig_path + Path(file_path).stem + '.png', bbox_inches='tight', dpi=default_dpi)
    return

def get_iupred_anchor_fig(file_path, fig_path, dpi=None):
    default_dpi = 90
    if dpi:
        default_dpi = dpi

    data = [x.rstrip().split(',') for x in simple_read(file_path)] #split lines from file by separator ','
    labels = [x[0] for x in data] # the first column has file id
    data = [x[1:] for x in data] #  the data follows

    length = len(max(data, key=len)) #the rows are unequal due to insertions and deletions so we get lengh of longest row for padding others
    #Creating custom heatmap using stacked bargraphs and coloring individual bars
    y_i = [1] * length #padded x length and y height set to 1
    x_i = [i for i in range(1, length + 1)]
    bottoms = np.zeros(len(x_i))

    for y in data:
        barlist = plt.bar(x_i, y_i, width=1, bottom=bottoms ,color='black')
        for i in range(len(y)):
            barlist[i].set_alpha(round(float(y[i]), 2))
        bottoms += y_i #needed for stacking

    plt.yticks(ticks=[i+0.5 for i in range(len(labels))], labels=labels, fontsize=16)
    x_ticks = [i for i in range(0,length,50)]
    plt.xticks(ticks=x_ticks, fontsize=12, rotation=90)

    plt.xlabel("Residue Position", fontsize=14)
    plt.ylabel("Variants", fontsize=14)
    plt.title(file_path, fontsize=16)
    plt.savefig(fig_path + Path(file_path).stem + '.png', bbox_inches='tight', dpi=default_dpi)
    
    return