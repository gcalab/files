# SARS-CoV-2 Regional/Temporal Amino Acid Substitution Tracking

A pipeline for analyzing non-synonymous mutations within the SARS-CoV-2 genome
(i.e., hCov-19 or the virus causing COVID-19) relative to global location and
temporal periods since the initial emergence in late 2019.

Uses sequence metadata as submitted to the [GISAID EpiCoV(tm) project](https://gisaid.org).

Sequence acknowledgements and additional information can be found at [https://github.com/gcalab/files/tree/master/F1000Research](https://github.com/gcalab/files/tree/master/F1000Research).

## Contributors

- [Tre Tomaszewski](mailto:jtomasz2@illinois.edu)

## Setup

### Datasets

The default data directory is `./data`, local to the project.
To use a different data directory, specify the data directory in the `.env` file.

Specify the GISAID sequence metadata version as the variable `METADATA_VERSION`) in a file named `.env`.
See `.env_template` for an example.

The environment variable `METADATA_VERSION` is typically the GISAID download date,
but may be any string that matches the metadata file

The instructions assume the `metadata_tsv_{METADATA_VERSION}.tar.xz` archive from GISAID is uncompressed.
This should be placed `./{DATA_DIR}/external`.

If the archive has already been decompressed, rename the resulting `metadata.tsv` file
to `metadata_{METADATA_VERSION}.tsv` and place in the `./{DATA_DIR}/raw` directory


### Python Scripts

Create a virtualenv using your favorite python version manager.

Then,

    ```sh
        pip install -r requirements.py
    ```

within the main project directory,

    ```sh
        pip install -e .
    ```

Then make the `covmut_seasonal/main.py` executable

    ```sh
        chmod +x ./covmut_seasonal/main.py`
    ```

**NOTE**:
    Tar and LZMA (`*.xz`) must be installed on the system in order to extract the file.


## Project Structure

```
CoVMutSeasonal
├─ LICENSE                  MIT License
├─ covmut_seasonal          Source Code Directory (i.e., ./src)
│  ├─ __init__.py
│  ├─ extract.py            Extract from Source Data (`./{DATA_DIR}/external`)
│  ├─ settings.py           General Project Code Settings
│  ├─ transform.py          Dataset Transformations
│  └─ visualizations.py     Figures, Tables, etc.
│
├─ data                     Optional (see "Setup")
│  ├─ external              Source Data
│  ├─ interim               Intermediate "Cache" (e.g., Preprocessed Data)
│  ├─ processed             "Production Ready" Data for Analysis
│  └─ raw                   Raw Data "Working" Data
│
├─ notebooks                Jupyter Notebooks
├─ reports                  Analyses and Documents
│  └─ figures               Visualizations
│
├─ .env                     Environment Variables ("Dot-Env" )
├─ requirements.txt         Required Python Packages
└─ setup.py                 Python Setup file

```

Adapted from ["Data Science 'cookiecutter' Template"](https://drivendata.github.io/cookiecutter-data-science/).


## Process


### Automatic

1. Set `run_latest.sh` as an executable.

    ```sh
    chmod +x ./run_latest.sh
    ```

2. Run.

    ```sh
    ./run_latest.sh
    ```

If the metadata file is already decompressed and in `{DATA_DIR}/raw/` comment out the first line.

### Manual


Note: Refer to help documentation for an overview of optional arguments.

    ```sh
        ./covmut_seasonal/main.py --help
    ```

1. Manually download `metadata_tsv_{year}_{month}_{day}.tar.xz` from [GISAID](https://gisaid.org); store in `./{DATA_DIR}/external`

2. Extract `metadata.tsv` file to `./data/raw` directory.

    ```sh
    ./covmut_seasonal/main.py extract metadata
    ```
    This step can be skipped if the extracted file already exists as `./{DATA_DIR}/raw/metadata_{METADATA_VERSION}.tsv`

3. Transform Country Latitudes into climate zones.

    ```sh
    ./covmut_seasonal/main.py transform geoclimate
    ```

4. Transform extracted metadata into usable format.

    ```sh
    ./covmut_seasonal/main.py transform metadata
    ```

5. Get Substitution Counts

    ```sh
    ./covmut_seasonal/main.py count substitutions
    ```

6. Get Sequence Counts

    ```sh
    ./covmut_seasonal/main.py count sequences
    ```

7. Extract AA Substitutions as Sequence Features creating feature maps

    ```sh
    ./covmut_seasonal/main.py extract features
    ```

8. Count AA Substitutions per geo-temporal interval

    ```sh
    ./covmut_seasonal/main.py count geotemporal
    ```

9. Visualize geo-temporal Incidence of AA Substitutions

    ```sh
    ./covmut_seasonal/main.py visualize
    ```

Data files are written to the directory `./{DATA_DIR}/processed/{METADATA_VERSION}`.
Figures are written to the directory `./reports/figures/{METADATA_VERSION}`.


## Original Fields

    | Original Field    | Expanded Fields   | New Fieldname     | DataType    | Purpose         |
    |-------------------|-------------------|-------------------|-------------|-----------------|
    | Accession ID      |                   | accession_id      | str         | Index           |
    | Virus name        |                   | sample_name       | str         | Validation      |
    |                   | Virus Code        | sample_virus      |             | Extraneous Info |
    |                   | Location Code     | sample_location   |             | Extraneous Info |
    |                   | Sample Code       | sample_code       |             | Extraneous Info |
    |                   | Year Code         | sample_year       |             | Extraneous Info |
    | AA Substitution   |                   | aa_substitutions  | set(str, …) | Required        |
    | Collection date   |                   | collection_date   | str         | Validation      |
    |                   | Collection Period | collection_period | int         | Required        |
    |                   | Collection Year   | collection_year   | int         | Validation      |
    |                   | Collection Month  | collection_month  | int         | Validation      |
    |                   | Collection Day    | collection_day    | int         | Validation      |
    | Submission date   |                   | submission_date   | str         | Validation      |
    |                   | Submission Period | submission_period | int         | Required        |
    |                   | Submission Year   | submission_year   | int         | Validation      |
    |                   | Submission Month  | submission_month  | int         | Validation      |
    |                   | Submission Day    | submission_day    | int         | Validation      |
    | Location          |                   | location          | str         | Validation      |
    |                   | Continent         | continent         | str         | Required        |
    |                   | Country           | country           | str         | Required        |
    |                   | Region            | region            | str         | Required        |
    |                   | Sub-Region        | subregion         | str         | Required        |
    | Host              |                   | host              | str         | Filter; Removed |
    | Is complete?      |                   | is_complete       | bool        | Filter; Removed |
    | Is high coverage? |                   | is_high_coverage  | bool        | Filter; Removed |
    | Clade             |                   | clade             | str         | Extraneous Info |
    | Variant           |                   | variant           | str         | Extraneous Info |
