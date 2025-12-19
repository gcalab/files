#This file will serve as the base for the project calling important functions from other scripts
from functions import get_rmsd_hdr, get_iupred, get_regions, get_slices, get_mut_fasta, get_tm_scores
import pandas as pd

def main():
    '''
    get_mut_fasta('data/fasta/Spike_Wuhan.fasta','data/mutations/','data/fasta/')
    labels, rmsds = get_rmsd_hdr('data/rmsd_hdr/')
    labels, disorder, binding = get_iupred('data/iupred/')
    regions = get_regions(rmsds,3.0)
    slices = get_slices(regions,'data/pdbs/', 'A', 'data/sliced_pdbs/')
    get_tm_scores('data/us_align/test/')
    labels, rmsds = get_rmsd_hdr('data/rmsd_hdr/','data/processed_scores/')
    regions = get_regions(rmsds,3.0,'data/processed_scores/')
    labels, disorder, binding = get_iupred('data/iupred/', 'data/processed_scores/')
    '''
    grps = get_tm_scores('data/us_align/test/', 'data/processed_scores/')
    
    
if __name__=="__main__":
    main()
