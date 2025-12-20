#This file will serve as the base for the project calling important functions from other scripts
from functions import get_rmsd_hdr, get_iupred, get_regions, get_slices, get_mut_fasta, get_tm_scores
from visualize import get_tm_fig, get_rmsdhdr_fig, get_iupred_anchor_fig
import pandas as pd

def main():

    get_iupred_anchor_fig('data/processed_scores/anchor2_scores.csv', 'data/figs/')
    get_iupred_anchor_fig('data/processed_scores/iupred3_scores.csv', 'data/figs/')
    get_rmsdhdr_fig('data/processed_scores/rmsd_headers.csv', 'data/figs/')
    get_tm_fig('data/processed_scores/tm_scores.csv', 'data/figs/')
    
if __name__=="__main__":
    main()
