# Supplimentary Notebooks for "The emergence of SARS-CoV-2 variants of concern in Australia by haplotype coalescence reveals a continental link to COVID-19 seasonality"

## Setup and Use

Below generally assumes knowledge of [Python (3.5+)](https://python.org) and [Jupyter](https://jupyter.org).\
Knowledge of [Pandas](https://pandas.pydata.org) and [MatPlotLib](https://matplotlib.org) will help in understanding the scripts, but is not necessary.

0. Download or clone this folder to a desired directory.
    - Add a `data` directory if it does not exist.
1. Acquire relevant metadata files from [GISAID](https://gisaid.org).
    - a. Login / Create an Account
    - b. Navigate to the "EpiCoV(tm)" page.
    - c. Click "Downloads" under the header.
    - d. Search for "Download packages"
    - e. Download "metadata" (`metadata.tsv.xz`) to the `data` directory created in Step 0.
2. Navigate to the directory using the cli.
3. Optional: Create a python environment(e.g., virtualenv, pyenv, conda, ...)
4. Use "pip" to install required packages: `pip install -r requirements.txt`
5. Launch Jupyter from the directory
6. For Data: Run all of `00_ExtractTransform.ipynb
7. For Visualizations: Complete Step 6, then 'Run All' of `01_Visualizations.ipynb`
