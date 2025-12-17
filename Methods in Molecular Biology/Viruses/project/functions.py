#This file contains the main functions for the project: https://github.com/gcalab/files/tree/master/Methods%20in%20Molecular%20Biology/Viruses#proteome-wide-ab-initio-structural-analysis-of-viral-evolution
import os
import pandas as pd
import Bio.PDB as bpdb
import itertools

#General function to retrieve data from files into a string object
def get_files(folder):
    files = os.listdir(folder)
    data = []

    for i in files:
        f = open(folder + i, 'r')
        temp = [x.rstrip().split() for x in f.readlines()]
        data.append(temp)
        f.close()
    
    return i, data

#Function to process the RMSD headers from the Chimera MultiAlignViewer
def get_rmsd_hdr(folder):
    rmsd_vals = []
    labels, hdr_data = get_files(folder)
    hdr_data = [x[1:-1] for x in hdr_data] #Remove the header and footnote tag in the file

    for hdr in hdr_data:
        df = pd.DataFrame(hdr)
        
        df[1] = pd.to_numeric(df[1], downcast='integer', errors='coerce') #1st index has RMSD values
        rmsd_vals.append(df[1].values)
        
    return labels, rmsd_vals

#Helper function for get_regions(rmsd_vals, threshold)
def ranges(i):
    for a, b in itertools.groupby(enumerate(i), lambda pair: pair[1] - pair[0]):
        b = list(b)
        yield b[0][1], b[-1][1]

#Function to assess regions of structural deviation from a reference structure using a RMSD threshold (usually 3Angstrom)
def get_regions(rmsd_vals, threshold):
    regions = [] #TO DO: Remove regions smaller than 4 or expand based on RMSD and account for indels
    for vals in rmsd_vals:
        for i in range(len(vals)):
            if vals[i] >= threshold:
                regions.append(i)
    
    regions = list(ranges(sorted(list(set(regions)))))
    #You may need to assess the regions visually some may be uninformative or may need to be expanded as US-align needs a minimum of 4 amino acids
    return regions

#Function to slice out regions depending on regions identified by function: get_regions(rmsd_vals, threshold)
def slice_pdb(og_pdb,start_pos,end_pos,chain,region, out_path):
    #currently dummy code, will generalize soon
    s = bpdb.PDBParser().get_structure('temp', og_pdb)
    start_res = int(start_pos)
    end_res = int(end_pos)
    chain_id = chain

    class ResSelect(bpdb.Select):
        def accept_residue(self, res):
            if res.id[1] >= start_res and res.id[1] <= end_res and res.parent.id == chain_id:
                return True
            else:
                return False

    io = bpdb.PDBIO()
    io.set_structure(s)
    new_name = og_pdb.split('/')[-1].split('.')[0] + ".pdb"
    io.save(out_path + region + '/' + new_name, ResSelect())

#Wrapper function for slice_pdb(og_pdb,start_pos,end_pos,chain,region)
def get_slices(regions,pdb_folder, chain, output):
    pdb_files = os.listdir(pdb_folder)
    
    for i in range(len(regions)):
        try:
            os.mkdir(output + str(i) + '/')
        except OSError as e:
            print(e, ': Dir exists, continuing process')
            continue
        
        for pdb in pdb_files:
            slice_pdb(pdb_folder + pdb, regions[i][0], regions[i][1], chain, str(i), output)

    
    return

#Function to process the output generated from the ANCOR2 selection output on IUPred3
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

