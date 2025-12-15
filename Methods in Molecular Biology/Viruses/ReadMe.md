# Proteome-wide *ab initio* Structural Analysis of Viral Evolution
#### This github repository serves as a supplement to Chapter 10 of the book 'Evolutionary Genomics' (Methods Molecular Biology, Vol. 2981; Springer Nature) *(Awaiting Publication)*

#### Software used:
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
### <ins>Using UCSF Chimera:</ins>
1. After opening your desired PDB/CIF files in the same Chimera session. Select the *'Tools'* option from the top navigation bar. <img src="img/chimera1.png" alt="Close up of upper navigation bar of the tool UCSF Chimera">
2. The *'Tools'* drop down menu will open and then select *'Structure Comparison'* -> *'Match Maker'* <img src="img/chimera2.png" alt="Navigate to 'Tools' -> 'Structure Comparison' -> 'Match Maker'">
3. A new window will open where you will choose which structure is your reference and which one will be matched to the reference. Enable *Show pairwise alignments(s)* but keep everything else at default settings. <img src="img/chimera3.png" alt="Opened view of the Match Maker window with everything set to default but enabling pairwise alignments after selecting the reference and to match structure">
4. After that you can additionally run *Match Align* also found in *'Tools'* -> *'Structure Comparison'*. This will show a Multiple Structure Alignment which is useful to visually see the structural deviations along the length of the protein sequence and can be a quick way to find out regions of interest. <img src="img/chimera4.png" alt="Tool bar naviagted to the Match Align option ('Tools' -> 'Structure Comparison' -> 'Match Align')">
5. From the Pairwise Alignment option in *Match Maker* a MultiAlignViewer window will open, where you will see a *'Headers'* option in the top navigation bar. Clicking this will reveal a drop down and you can select *'RMSD:backbone'* to only retain the RMSD values of the backbone atoms (or you can chnage this according to your needs). After this you can save the RMSD headers to a text file by clicking the *'Save...'* option from the *'Headers'* dropdown list. <img src="img/chimera5.png" alt="Opened view of the MultiAlignViewer, the navigation bar is in close view with the Headers dropdown open and the option RMSD:backbone is selected">
6. A new window will open which will prompt you to type out the name and path for the RMSD header file. <img src="img/chimera6.png" alt="Window open after selecting to save RMSD headers giving option of file name and location">

<ins>You can visualize the RMSD Headers for each of your protein structures as a heatmap:</ins> <img src="img/post_chimera1.png" alt="Heatmap depicting RMSD differences along the length of the Spike protein during the COVID-19 pandemic">


<ins>Gather information of regions cross an RMSD threshold of your choice and slice them out into smaller PDB files like this:</ins> <img src="img/post_chimera3.png" alt="Sliced out regions of the Spike protein and its variants">


<ins>These regions can then be analyzed separately using US-align:</ins> <img src="img/post_chimera2.png" alt="Spike protein with labelled regions of interest">

### <ins>Using BioPDB to slice out regions along a protein:</ins>

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

### <ins>Using US-align to analyze regional structural deviations:</ins>

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

Example of how you can visualize the results from US-align as a heatmap for both with and without superposition (for any number of proteins you wish to analyze): <img src="img/usalign.png" alt="2 heatmaps with labels a and b, with a being for no superposition and b being superimposed US-align results">

## 3.4) Mapping Intrinsic Disorder and Binding Capability:
### <ins>Using IUPred for analyzing intrinsic disorder and binding capability:</ins>

1. Navigate to the [IUPred3 website](https://iupred3.elte.hu/) <img src="img/iupred1.png" alt="IUPred3 Landing Page">

2. Paste your proteins amino acid sequence into the text box or upload the fasta file. After inputting your data. Select the paramters that best fit your needs. You will need to run this twice once for obtaining the IUPred3 scores and once for ANCHOR2 scores. To choose between long and short look at this [explanation](https://iupred3.elte.hu/help_new). This study uses hsort disorder to emphasize local/regional differences instead of the overall protein. <img src="img/iupred2.png" alt="Selecting Option for IUPred3">

3.Below is an example of how you can visualize IUPred and ANCHOR2 scores of your proteins. <img src="img/IUPred.png" alt="Example image of SARS-CoV-2 N-protein variants showing lineplots of ANCHOR2 and IUPred scores">

### <ins>Normal Mode Analysis (NMA) using [Bio3D](http://thegrantlab.org/bio3d/) in [R](https://www.r-project.org/):</ins>
First you must [Install](http://thegrantlab.org/bio3d_v2/tutorials/installing-bio3d) Bio3D.
> On all platforms (Mac, Linux, and PC) open R (version 3.1.0 or higher) and use the function install.packages() at the R command prompt to install the package from your local CRAN site:

> `install.packages("bio3d", dependencies=TRUE)`

> Alternatively, if you prefer to use RStudio, select Tools → Install Packages.., Enter bio3d in the Packages text box (and make sure Install Dependencies is selected). Then click Install.

R code below: 

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
Example of how how you can visualize the fluctuations and the first non trivial normal mode of different proteins: <img src="img/nma1.png" alt="Showing a line graph of fluctatuations of various spike proteins (variants and haplotypes) along with their first non trivial normal mode">

## 3.5) Downstream Analysis:

<ins>Examples of the kinds of analyses you can carry out:</ins>

1. https://f1000research.com/articles/12-267
2. https://www.mdpi.com/2079-7737/13/3/134
3. https://www.mdpi.com/1999-4915/16/9/1358
4. https://www.ideals.illinois.edu/items/131533
