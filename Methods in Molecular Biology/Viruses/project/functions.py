#This file contains the main functions for the project: https://github.com/gcalab/files/tree/master/Methods%20in%20Molecular%20Biology/Viruses#proteome-wide-ab-initio-structural-analysis-of-viral-evolution
import os
import pandas as pd
import Bio.PDB as bpdb
import itertools
import re

#Function to read individual file
def read_file(file_path):
    f = open(file_path, 'r')
    temp = [x.rstrip().split() for x in f.readlines()]
    f.close()

    return temp

#General function to retrieve data from files into a string object
def get_files(folder):
    files = os.listdir(folder)
    data = []

    for i in files:
        temp = read_file(folder + i)
        data.append(temp)
    
    return files, data

#Function to get positions from mutation string: D614G
def get_pos(subs):
    for i in range(len(subs)):
        subs[i] = int(''.join(re.findall(r'\d', subs[i]))) #extract digits only from string text
    return subs

#Function to add mutations to original fasta sequence
def mutate(seq,subs):
    og_subs = subs.copy()
    positions = get_pos(subs)
    sorted_subs = [val for _, val in sorted(zip(positions, og_subs))]
    sorted_pos = sorted(positions)

    pos_adjuster = -1 #Accounting for deletions or insertions the mutation position needs to be adjusted; we start at -1 due to how python counts arrays (start at 0)
    
    for i in range(len(sorted_pos)):
        if '-' in sorted_subs[i]:
            if seq[sorted_pos[i] + pos_adjuster] == sorted_subs[i][0]:
                seq.pop(sorted_pos[i] + pos_adjuster)
                pos_adjuster -= 1
            else:
                print('Original Residue Mismatch AA DEL', sorted_subs[i][0], ' -X- ', seq[sorted_pos[i]])

        elif '+' in sorted_subs[i]:
            seq.insert(sorted_pos[i] + pos_adjuster, ''.join(char for char in sorted_subs[i] if char.isalpha())) #Add only alphabets back into char array [A,B,C,D] -> +2EPE [A,EPE,BCD]
            pos_adjuster += 1

        else:
            if seq[sorted_pos[i] + pos_adjuster] == sorted_subs[i][0]:
                seq[sorted_pos[i] + pos_adjuster] = sorted_subs[i][-1]
            else:
                print('Original Residue Mismatch AA SUB Position: ', str(sorted_pos[i]), ', ',  sorted_subs[i][0], ' -X- ', seq[sorted_pos[i]])

    return ''.join(seq)

#Function to mutate original FASTA file and save new FASTA sequences
def get_mut_fasta(fasta_path, mut_path, out_path):
    #Convert fasta string to char array for easy mutating
    og_fasta = list(read_file(fasta_path)[0][0]) #Common structure list of lists as used by most other functions so access fasta string through [0][0]
    labels, mutations = get_files(mut_path) #[label array, [array of mutation arrays]] = [['label1','label2','label3'],[[list1 of mutations],[list2 of mutations],[list3 of mutations]]]
    new_fasta = {} #dict to store new mutated sequences labelled with their original mutant file name

    for i in range(len(labels)):
        mutated_fasta = og_fasta.copy()
        mutations[i] = list(set([x[0] for x in mutations[i]])) #Rectify formating for easy mutation and removing duplicates using set then converting back to list
        new_fasta[labels[i]] = mutate(mutated_fasta, mutations[i])

    return new_fasta

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

