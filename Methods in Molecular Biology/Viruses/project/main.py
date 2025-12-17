#This file will serve as the base for the project calling important functions from other scripts
from functions import get_rmsd_hdr, get_iupred, get_regions, get_slices

def main():
    labels, rmsds = get_rmsd_hdr('data/rmsd_hdr/')
    labels, disorder, binding = get_iupred('data/iupred/')
    regions = get_regions(rmsds,3.0)
    slices = get_slices(regions,'data/pdbs/', 'A', 'data/sliced_pdbs/')
    

if __name__=="__main__":
    main()
