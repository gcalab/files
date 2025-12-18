library('bio3d') #Import the Bio3D library
input <- "data/pdbs"
output <- "data/bio3d/"
pdbfiles <- list.files(path=input, pattern="*.pdb", full.names=TRUE) #Retrieve list of target PDB files

#Iterate through the PDB files
for (x in pdbfiles){
  pdb <- read.pdb(x) #Read files into a PDB object
  modes <- nma(pdb) #retrieve the normal modes for the PDB file into  a variable called modes
  fluctuations <- modes$fluct #Retrieve the fluctuation values into  a variable called fluctuations
  
  name <- gsub("[./pdb]", "", x) #get name of pdb file (remove the '.pdb' from file name using gsub)
  n <- paste(output, name,".csv", sep="") #concatenate PDB file name into a desired output path location and file type using paste
  write.csv(fluctuations, file = n, row.names =FALSE) #write the fluctuations into a csv file with the new name
  traj_file <- paste(output,name,'.pdb', sep="")
  mktrj(modes, mode=7, pdb=pdb, file=traj_file) #The 7th mode is the first non trivial mode
}

