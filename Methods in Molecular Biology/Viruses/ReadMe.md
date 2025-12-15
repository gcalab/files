# Proteome-wide *ab initio* Structural Analysis of Viral Evolution
### This github repository serves as a supplement to Chapter 10 of the book 'Evolutionary Genomics' (Methods Molecular Biology, Vol. 2981; Springer Nature) *(Awaiting Publication)*

Software used:
1. [Local Colab Fold](https://github.com/YoshitakaMo/localcolabfold) (or latest version of [AlphaFold](https://alphafoldserver.com/welcome))
2. [UCSF Chimera](https://www.cgl.ucsf.edu/chimera/download.html)
3. [AS2TS server](http://proteinmodel.org/AS2TS/LGA/lga.html) used in CASP
4. [USAlign](https://www.aideepmed.com/US-align/)
5. [IUPred2A](https://iupred2a.elte.hu/)
6. [Bio3D package in R](https://cran.r-project.org/web/packages/bio3d/index.html)

#### Step-by-step guide of the Methods presented in Section 3 of the chapter: 

#### 3.1) AlphaFold Modeling:
LocalColabFold Command:
```
colabfold_batch --amber --random-seed 42 --templates --num-recycle 3 --use-gpu-relax $fasta_file output
```
[Explanation of input flags](https://github.com/YoshitakaMo/localcolabfold?tab=readme-ov-file#flags):
+ --amber: Refine structure through energy minimization.
+ --random-seed: Set random number so that others can reproduce the exact same structure when given the same parameters.
+ --templates: Use templates.
+ --num-recycle: Amount of recycles used for prediction, increasing this may improve quality but reduces speed.
+ --use-gpu-relax: Run on Nvidia GPU instead of CPU (highly recommended).

#### 3.2) Structural Validation:

#### 3.3) Mutant Structure Comparison:
Code snippet to slice out portions of a PDB file to allow regional analysis of structural deviation.
```
import Bio.PDB as bpdb

s = bpdb.PDBParser().get_structure('temp', og_pdb) #'temp' is just a temporary name, og_pdb refers to the PDB passed to this function
  class ResSelect(bpdb.Select):
    def accept_residue(self, res):
    if res.id[1] >= start_res and res.id[1] <= end_res and res.parent.id == chain_id:
      return True
    else:
      return False

s = bpdb.PDBParser().get_structure('temp', '1y26.pdb')

start_res = 4
end_res = 62
chain_id = 'A'

io = bpdb.PDBIO()
io.set_structure(s)
new_name = name + ".pdb"
io.save(out + region + '/' + new_name, ResSelect())
```
After you have your sliced regions you will need to analyse them further using [USAlign](https://www.aideepmed.com/US-align/).
This can be achieved in two ways: 1) With Superposition and 2) Without Superposition.

```
USalign -se -dir1 path/to/pdbs/mutant.pdb path/to/pdbs/file/list.txt path/to/pdbs/reference.pdb > output_folder/results_se.txt
```
```
USalign -dir1 path/to/pdbs/mutant.pdb path/to/pdbs/file/list.txt path/to/pdbs/reference.pdb > output_folder/results_no_se.txt
```
#### 3.4) Mapping Intrinsic Disorder and Binding Capability:
```
library('bio3d')
pdbfiles <- list.files(pattern="*.pdb", full.names=TRUE)
for (x in pdbfiles){
  pdb <- read.pdb(x)
  modes <- nma(pdb)
  fluctuations <- modes$fluct
  name <- gsub("[./pdb]", "", x)
  n <- paste("out/",name,".csv", sep="")
  write.csv(fluctuations, file = n, row.names =FALSE)
}
```
#### 3.5) Downstream Analysis:

