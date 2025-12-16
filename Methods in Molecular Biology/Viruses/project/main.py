#This file will serve as the base for the project calling important functions from other scripts
from functions import get_rmsd_hdr, get_iupred

def main():
    labels, rmsds = get_rmsd_hdr('data/rmsd_hdr/')
    labels, disorder, binding = get_iupred('data/iupred/')
    print(rmsds)
    print(disorder)
    print(binding)

if __name__=="__main__":
    main()