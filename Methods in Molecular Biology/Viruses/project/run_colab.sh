#!/bin/bash

#   EXAMPLE COMMAND:
#   ./run_colab.sh "data/fasta/*" "output/"

#   Loop through directory that has your fasta files; You might need to adjust the directory 'data/fasta/*'
#   The '*' after the '/' is necessary

#   You may need to set the path to your colab fold installation whenever you want to run it in a new terminal
export PATH="/home/your-dir/your-other-dir/colabfold_batch/bin:$PATH"

for fasta in $1; do
echo "COMMAND USED: colabfold_batch --amber --random-seed 42 --templates --num-recycle 3 --use-gpu-relax $fasta $2";

#Run colab fold for each fasta file
colabfold_batch --amber --random-seed 42 --templates --num-recycle 3 --use-gpu-relax $fasta $1;

echo "OUTPUT: LocalColabFold output for $fasta file has been saved to output directory: $2";
done

echo "*~Finished Running ColabFold~*";
