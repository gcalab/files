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
   ├── #contains sliced up regions of the pdb files in the pdbs/ folder using RMSD header dataafter get_slices() from main.py
```
