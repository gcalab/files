#This file contains the main functions for the project: https://github.com/gcalab/files/tree/master/Methods%20in%20Molecular%20Biology/Viruses#proteome-wide-ab-initio-structural-analysis-of-viral-evolution
import os
import pandas as pd

def get_files(folder):
    files = os.listdir(folder)
    data = []

    for i in files:
        f = open(folder + i, 'r')
        temp = [x.rstrip().split() for x in f.readlines()]
        data.append(temp)
        f.close()
    
    return i, data


def get_rmsd_hdr(folder):
    rmsd_vals = []
    labels, hdr_data = get_files(folder)
    hdr_data = [x[1:-1] for x in hdr_data] #Remove the header and footnote tag in the file

    for hdr in hdr_data:
        df = pd.DataFrame(hdr)
        
        df[1] = pd.to_numeric(df[1], downcast='integer', errors='coerce') #1st index has RMSD values
        rmsd_vals.append(df[1].values)
        
    return labels, rmsd_vals


def get_iupred(folder):
    iupred, anchor = [], []
    labels, iupred_data = get_files(folder)
    iupred_data = [x[8:-1] for x in iupred_data] #Remove header and empty lists
    
    for scores in iupred_data:
        df = pd.DataFrame(scores)
        
        df[2] = pd.to_numeric(df[2], downcast='integer', errors='coerce') #2nd index has IUPred Scores
        iupred.append(df[2].values)

        df[3] = pd.to_numeric(df[3], downcast='integer', errors='coerce') #3rd index has ANCOR2 scores
        anchor.append(df[3].values)
        
    return labels, iupred, anchor

