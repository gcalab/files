# Project Structure & Files

### Data Folder
```
/data
├──/fasta
|  └── #contains original fasta file and mutated fasta file after running get_mut_fasta() from main.py
├──/iupred
|  └── #contains results from IUPred3
├──/mutations
|  └── #contains csv files containing mutation data in 'D614G' format, line separated
├──/pdbs
|  └── #contains superimposed pdb files for target pporteins
├──/rmsd_hdr
|  └── #contains RMSD headers after running MatchMaker in Chimera
└──/sliced_pdbs
   └── #contains directory of directories with internal dirs named 0,1,... representing identified regions of structural deviation; sliced up regions of the pdb files in the pdbs/ folder using RMSD header data after running get_slices() from main.py
```
### Scripts
#### Callable functions of functions.py:

1.   get_mut_fasta(fasta_path, mut_path, out_path)
     - This will generate fasta files incorporating mutations provided in csv (look at example for formatting mutation file)
     - Needs: location to fasta file, mutation file, and a path to the output folder
2.   get_rmsd_hdr(folder)
     - This will process the RMSD headers for visualization (reads file data into array)
     - Needs: path to folder
3.   get_regions(rmsd_vals, threshold)
     - This will get regions of structural deviation using RMSD header data based on a threshold
     - Needs output from get_rmsd_hdr() and user defined threshold (reccomended value: 3.0)
4.   get_slices(regions,pdb_folder, chain, output)
     - This will slice of regions identified from get_regions() into the sliced_pdbs/ folder
     - Needs: output from get_regions(), path to slice_pdb folder, what chain you want sliced (check chain id in Chimera), path to output folder
5.   get_iupred(folder)
     - This will process the IUPred3 output files for visualization
     - Needs: path to folder with IUPred3 files
6.   get_tm_scores(tm_path)
     - This will process the output files from USalign and return a dictionary holding lists of tuples with regional TM scores like this:
     - `se_region_1_OUTPUT.txt
[('Wuhan_Alpha', 0.77419), ('Wuhan_Delta', 0.72706), ('Wuhan_H1', 0.88681), ('Wuhan_H12', 0.87898), ('Wuhan_H13', 0.79785), ('Wuhan_H14', 0.92601), ('Wuhan_H15', 0.7618), ('Wuhan_H16', 0.0562), ('Wuhan_H17', 0.85074), ('Wuhan_H19', 0.30694), ('Wuhan_H3', 0.16518), ('Wuhan_H4', 0.33909), ('Wuhan_H5', 0.33604), ('Wuhan_H7', 0.27892), ('Wuhan_H8', 0.19799), ('Wuhan_Omicron_S371F', 0.11269), ('Wuhan_Omicron_S371L', 0.10173)]`
     - Needs: path to tm score folder

> [!TIP]
> You should call functions according to your requirements in main.py and customize your workflow to where you are in the process when following the guide for [Proteome-wide ab initio Structural Analysis of Viral Evolution](https://github.com/gcalab/files/blob/master/Methods%20in%20Molecular%20Biology/Viruses/ReadMe.md)
