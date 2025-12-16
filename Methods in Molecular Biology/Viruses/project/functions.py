#This file contains the main functions for the project: https://github.com/gcalab/files/tree/master/Methods%20in%20Molecular%20Biology/Viruses#proteome-wide-ab-initio-structural-analysis-of-viral-evolution
import os

def get_files(folder):
    files = os.listdir()
    data = []

    for i in files:
        f = open(folder + i, 'r')
        data.append(f.read())
        f.close()
    
    return data


def get_headers(folder):
    hdr_files = get_files(folder)
    print(hdr_files[0])

folder_path = '..\data\rmsd_hdr\'
get_headers(folder_path)
