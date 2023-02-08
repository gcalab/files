File Descriptions and Metadata
==============================

## `unique_substitutions.csv`

Total Number of Sequences Used: 11,923,363

| Column Name | Description                                                                                           | Datatype |
|-------------|:------------------------------------------------------------------------------------------------------|----------|
| aa_sub      | Full Substitution Name in raw format as provided by GISAID                                            | string   |
| gene        | Gene Name                                                                                             | string   |
| ref         | Protein in Reference Sequence [GISAID Accession ID: EPI_ISL_402124; hCoV-19/Wuhan/WIV04/2019 (WIV04)] | string   |
| loc         | Protein location on reference sequence                                                                | int      |
| sub         | Protein substitution in compared sequence                                                             | string   |
| seq_count   | Count of sequences with substituted protein                                                           | int      |
| seq_frac    | Fraction of sequences with substituted protein relative total sequences (11,923,363)                  | float    |

Substitution Nomenclature based on that suggested within:

        Dunnen, J. T. den, & Antonarakis, S. E. (2000). Mutation nomenclature extensions and suggestions to describe complex mutations: A discussion. Human Mutation, 15(1), 7–12. https://doi.org/10.1002/(SICI)1098-1004(200001)15:1<7::AID-HUMU4>3.0.CO;2-N


## `gisaid_accession_ids.txt.xz`

Compressed text file of GISAID accession ids, each separated by a line break.


## `gisaid_supplemental_table_epi_set_230208ex.pdf`

Sequence acknowledgement file containing links to GISAID's sequence acknowledgement browser, as recommended by GISAID.