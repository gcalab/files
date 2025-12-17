# Project Structure & Files

### Data Folder
```
/data
├──/fasta
|  ├── #contains original fasta file and mutated fasta file after running get_mut_fasta() from main.py
├──/iupred
|  ├── #contains results from IUPred3
├──/mutations
|  ├── #contains csv files containing mutation data in 'D614G' format, line separated
├──/pdbs
|  ├── #contains superimposed pdb files for target pporteins
├──/rmsd_hdr
|  ├── #contains RMSD headers after running MatchMaker in Chimera
├──/sliced_pdbs
   ├── #contains directory of directories with internal dirs named 0,1,... representing identified regions of structural deviation; sliced up regions of the pdb files in the pdbs/ folder using RMSD header data after running get_slices() from main.py
```
### Scripts
#### Callable functions of main.py:

1.   get_mut_fasta(fasta_path, mut_path, out_path)
     - This will generate fasta files incorporating mutations provided in csv (look at example for formatting mutation file)
     - Needs: location to fasta file, mutation file, and a path to the output folder
3.   get_rmsd_hdr(folder)
     - This will process the RMSD headers for visualization (reads file data into array)
     - Needs: path to folder
5.   get_regions(rmsd_vals, threshold)
     - This will get regions of structural deviation using RMSD header data based on a threshold
     - Needs output from get_rmsd_hdr() and user defined threshold (reccomended value: 3.0)
7.   get_slices(regions,pdb_folder, chain, output)
8.   get_iupred(folder)
