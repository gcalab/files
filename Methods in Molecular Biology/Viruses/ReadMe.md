# Proteome-wide *ab initio* Structural Analysis of Viral Evolution
#### This github repository serves as a supplement to Chapter 10 of the book 'Evolutionary Genomics' (Methods Molecular Biology, Vol. 2981; Springer Nature) *(Awaiting Publication)*

Software used:
1. [Local Colab Fold](https://github.com/YoshitakaMo/localcolabfold) (or latest version of [AlphaFold](https://alphafoldserver.com/welcome))
2. [UCSF Chimera](https://www.cgl.ucsf.edu/chimera/download.html)
3. [AS2TS server](http://proteinmodel.org/AS2TS/LGA/lga.html) used in CASP ([Protopedia Manual](https://proteopedia.org/wiki/index.php/Calculating_GDT_TS))
4. [USAlign](https://www.aideepmed.com/US-align/)
5. [IUPred2A](https://iupred2a.elte.hu/)
6. [Bio3D package in R](https://cran.r-project.org/web/packages/bio3d/index.html)

#### Step-by-step guide of the Methods presented in Section 3 of the chapter: 

## 3.1) AlphaFold Modeling:
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

## 3.2) Structural Validation:

## 3.3) Mutant Structure Comparison:
<ins>Using UCSF Chimera:</ins>

<img src="img/chimera1.png" alt="">
<img src="img/chimera2.png" alt="">
<img src="img/chimera3.png" alt="">
<img src="img/chimera4.png" alt="">
<img src="img/chimera5.png" alt="">
<img src="img/chimera6.png" alt="">


<ins>Using BioPDB to slice out regions along a protein:</ins>

Code snippet to slice out portions of a PDB file to allow regional analysis of structural deviation. Adapted from an [answer](https://stackoverflow.com/a/22453336) by [Juniper-](https://stackoverflow.com/users/899470/juniper) on [Stack Overflow](https://stackoverflow.com/questions).
```
import Bio.PDB as bpdb

  class ResSelect(bpdb.Select):
    def accept_residue(self, res):
    if res.id[1] >= start_res and res.id[1] <= end_res and res.parent.id == chain_id:
      return True
    else:
      return False

s = bpdb.PDBParser().get_structure('temp', og_pdb) #Retrieve target PDB file; parameters: id, file location

#Selecting residue position 4 to 62 from chain A
start_res = 4
end_res = 62
chain_id = 'A' 

io = bpdb.PDBIO() #Instantiate PDBIO object
io.set_structure(s) #Give it the PDB we want sliced
new_name = 'your desired name for the sliced region'
io.save(out + region + '/' + new_name, ResSelect()) #example output path that saves regions (e.g: region1, region2, in a directory) and uses ResSelect to slice out desired region before saving the structure
```

<ins>Using US-align to analyze regional structural deviations:</ins>

After you have your sliced regions you will need to analyse them further using [US-align](https://www.aideepmed.com/US-align/).
This can be achieved in two ways: 1) With Superposition and 2) Without Superposition.

##### With Superposition (Does not use *'-se'*)
```
USalign -dir1 path/to/pdbs/mutant.pdb path/to/pdbs/file/list.txt path/to/pdbs/reference.pdb > output_folder/results_se.txt
```
##### Without Superposition (Uses *'-se'*)
```
USalign -se -dir1 path/to/pdbs/mutant.pdb path/to/pdbs/file/list.txt path/to/pdbs/reference.pdb > output_folder/results_no_se.txt
```
[Explanation of Input Flags:](https://www.aideepmed.com/US-align/help/)
> -dir1: Use chain2 to search a list of PDB chains listed by 'chain1_list' under 'chain1_folder'. Note that the slash is necessary. e.g: USalign -dir1 chain1_folder/ chain1_list chain2

> -se: Do not perform superposition. Useful for extracting alignment from superposed structure pairs


## 3.4) Mapping Intrinsic Disorder and Binding Capability:
<ins>Normal Mode Analysis (NMA) using [Bio3D](http://thegrantlab.org/bio3d/) in [R](https://www.r-project.org/):</ins>

```
library('bio3d') #Import the Bio3D library

pdbfiles <- list.files(pattern="*.pdb", full.names=TRUE) #Retrieve list of target PDB files

#Iterate through the PDB files
for (x in pdbfiles){
  pdb <- read.pdb(x) #Read files into a PDB object
  modes <- nma(pdb) #retrieve the normal modes for the PDB file into  a variable called modes
  fluctuations <- modes$fluct #Retrieve the fluctuation values into  a variable called fluctuations

  name <- gsub("[./pdb]", "", x) #get name of pdb file (remove the '.pdb' from file name using gsub)
  n <- paste("out/",name,".csv", sep="") #concatenate PDB file name into a desired output path location and file type using paste
  write.csv(fluctuations, file = n, row.names =FALSE) #write the fluctuations into a csv file with the new name
}
```
## 3.5) Downstream Analysis:

