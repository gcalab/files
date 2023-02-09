import csv
from collections import Counter, defaultdict
from math import floor
from pathlib import Path
import re
import pickle

import numpy as np

import covmut_seasonal.load as load
from covmut_seasonal.settings import (
    RAW_DIR, INTER_DIR, PROC_DIR, METADATA_FNAME, climate_zone_names
)
import covmut_seasonal.utils as utils
import covmut_seasonal.counts as counts


def count_sequences(
    sequence_fpath,
    dst_fpath,
    force_overwrite=False
):
    """
        Get number of sequences in the specified sequence file.

        If the file exists, return the number of sequences as an integer.
        If the file does not exist or overwrite is set, count the sequences and
        save to a one-line file.

        Used as a base method for `raw_sequences` and `filtered_sequences`

        Parameters
        ----------
        sequence_fpath : str | pathlib.Path
            A file containing per-line sequence (meta)data.

        dst_fpath : str | pathlib.Path
            File to save count result.

        force_overwrite : bool
            Whether the lines in the input file should be recounted and saved
                if the output file already exists. If "falsy" and file exists,
                the cached count is returned instead.

        Returns
        -------
        int
            Number of sequences in input file.

    """
    if not isinstance(dst_fpath, Path):
        dst_fpath = Path(dst_fpath)

    count = None

    try:
        if dst_fpath.exists() and not force_overwrite:
            # get cached sequence count
            with open(dst_fpath, 'r') as fp:
                count = int(fp.readline().strip())
    except:
        print('Could not read file.')

    if not count:
        # cache sequence count
        with open(sequence_fpath, 'r') as ifp, open(dst_fpath, 'w') as ofp:
            count = str(len([_ for _ in ifp]))
            ofp.write(count + '\n')

    try:
        return int(floor(count))
    except TypeError as e:
        print('Error String:', count)
        raise(e)


def raw_sequences(
    force_overwrite=False
):
    """
        Get number of lines/sequences in the raw metadata.

        A wrapper for the general `count_sequences` function.

        Parameters
        ----------
        force_overwrite : bool
            Whether the lines in the raw metadata should be recounted and saved
                if the output file already exists. If "falsy" and file exists,
                the cached count is returned instead.

        Returns
        -------
        int
            Number of sequences in input file.
    """
    sequence_fpath = RAW_DIR / METADATA_FNAME

    dst_fpath = INTER_DIR / 'counts' / f'md_counts_raw_seqs.txt'
    if not dst_fpath.parent.exists(): dst_fpath.parent.mkdir(parents=True, exist_ok=True)

    return count_sequences(
        sequence_fpath,
        dst_fpath,
        force_overwrite=force_overwrite
    )


def filtered_sequences(
    force_overwrite=False
):
    """
        Get number of lines/sequences in the filtered, intermediate metadata.

        A wrapper for the general `count_sequences` function.

        Parameters
        ----------
        force_overwrite : bool
            Whether the lines in the interim metadata should be recounted and saved
                if the output file already exists. If "falsy" and file exists,
                the cached count is returned instead.

        Returns
        -------
        int
            Number of sequences in input file.
    """
    sequence_fpath = INTER_DIR / 'aa_subs_by_sequence.txt'

    dst_fpath = INTER_DIR / 'counts' / f'md_counts_filtered_seqs.txt'
    if not dst_fpath.parent.exists(): dst_fpath.parent.mkdir(parents=True, exist_ok=True)

    return count_sequences(
        sequence_fpath,
        dst_fpath,
        force_overwrite=force_overwrite
    )


def unique_substitutions(
    src_fpath=INTER_DIR / 'aa_subs_by_sequence.txt',
    dst_fpath=INTER_DIR / 'counts' / 'unique_substitutions.csv'
):
    """
        Build list of existing substitutions, name subfields, and
        number of occurences, saved to a CSV file.

        Names are broken each into subfields
            - "gene",
            - "reference amino acid",
            - "gene location",
            - "substituted amino acid"

        This will only write an existing substitution once, removing duplicates
        by coercion of the input into a `set` datatype.

        The strings in the set must be formatted as:

            ```
            {gene}_{ref_amino_acid}{gene_location}{sub_amino_acid}
            ```

        the regular expression (in python "flavor") for extraction is:

            ```
            r'(?P<gene>[A-Z][A-Za-z]*[0-9]{,2}[ab]?)_(?P<ref>.+?)(?P<loc>[0-9]+?)(?P<sub>[A-Za-z].*)'
            ```

        Parameters
        ----------
        src_fpath:

        dst_fpath : str | pathlib.Path
            Filepath for writing unique AA substitutions, name fields,
            and the number of sequences with the AA substitution.

    """

    aa_subs = Counter()
    total_count_sequences = None

    if not Path(src_fpath).exists():
        err_msg = (
            'Source AA Substitution file could not be found.\n'
            '       Be sure to run `[PROG] transform metadata` before this command.'
        )
        raise FileNotFoundError(err_msg)

    try:
        total_count_sequences = load.count_filtered_sequences()
    except Exception as e:
        pass

    with open(src_fpath, 'r') as fp:
        for line in utils.tqdm(fp, total=total_count_sequences, desc='Reading from Source:'):
            row = line.split(',')
            # remove accession id with
            row = row[1:]

            for aa_sub in row:
                aa_sub = aa_sub.strip()
                aa_subs[aa_sub] += 1


    aa_sub_regex = re.compile(r'(?P<gene>[A-Z][A-Za-z]*[0-9]{,2}[ab]?)_(?P<ref>.+?)(?P<loc>[0-9]+?)(?P<sub>[A-Za-z].*)')

    fieldnames = ['aa_sub', 'gene', 'ref', 'loc', 'sub', 'num_seqs']

    # create directory if it does not exist
    if not dst_fpath.parent.exists(): dst_fpath.parent.mkdir(parents=True, exist_ok=True)

    with open(dst_fpath, 'w') as sn_fp:
        writer = csv.DictWriter(sn_fp, fieldnames=fieldnames, extrasaction='ignore')
        writer.writeheader()

        rows = []

        for aa_sub, instance_count in utils.tqdm(aa_subs.items(), desc='Writing to Destination:'):
            aa_sub = aa_sub.strip()
            if (match := aa_sub_regex.match(aa_sub)) is not None:
                row = dict(
                    aa_sub=aa_sub,
                    **match.groupdict(),
                    num_seqs=instance_count
                )
                rows.append(row)

        # sort descending
        rows = sorted(rows, key=lambda r: r.get('num_seqs'), reverse=True)

        writer.writerows(rows)

    with open(INTER_DIR / 'counts' / f'md_counts_{dst_fpath.stem}.txt', 'w') as fp:
        fp.write(str(len(rows)))

def substitutions_per_sequence():
    """
        Get number of AA substitutions per sequence.
    """

    with open(INTER_DIR / 'substitutions_per_sequence.csv', 'w') as dst_fp:

        dst_fp.write(f'accession_id,num_subs\n')

        for accession_id, aa_subs in load.aa_subs_by_sequence():
            num_subs = len(aa_subs)
            dst_fp.write(f'{accession_id},{num_subs}\n')


def sequences_per_geotemporal_bin(geographic_resolution=3, temporal_resolution=1, climate_zone=0):
    """
        Aggregate sequences in a geographical region during a period of time
        given provided resolutions.

        Parameters
        ----------
        geographic_resolution : int{0, 1, 2, 3}
            Resolution of the geographic bin in which sequences are assigned.
                0 = 'continent',
                1 = 'country',
                2 = 'region',
                3 = 'zone'
            Defaults to 3 [Zone]

        temporal_resolution : int{0, 1, 2}
            Resolution of the temporal bin in which sequences are assigned.
                0 = 'Year',
                1 = 'Quarter',
                2 = 'Month'
            Defaults to 1 [Quarter]

        climate_zone : int{0, 1, 2} | str
            The climate zoning type.
                0 = 'simple': Uses simplified latitude demarcations +/-(30, 60),
                1 = 'real' : Uses standard latitude demarcations' +/-(23.27, 66.33),
                2 = 'subtropic' : Adds subtropics +/-(23.27, 35) to 'real'
            Defaults to 0 [simple]

    """

    period_translations = ['collection_year', 'collection_quarter', 'collection_month_idx']

    geo_res_index = int(geographic_resolution)
    temp_res_index = int(temporal_resolution)

    geographic_resolution = utils.convert_geographic_resolution_to_string(geographic_resolution)
    temporal_resolution = utils.convert_temporal_resolution_to_string(temporal_resolution)
    climate_zone = utils.convert_climate_zone_type_to_string(climate_zone)
    period_to_column_translation = period_translations[temp_res_index]


    def adjust_geographic_resolution(row):
        continent = row.get('continent', '').lower()
        country =  row.get('country', '').lower()
        region = row.get('region', '').lower()

        if geographic_resolution == 'continent':
            return [continent]
        elif geographic_resolution == 'country':
            return [continent, country]
        elif geographic_resolution == 'region':
            return [continent, country, region]



    dst_fstem = f'seq_count_by_{temporal_resolution}_{geographic_resolution}'

    if geographic_resolution == 'zone':
        dst_fstem += f'_{climate_zone}'

    dst_dirpath= INTER_DIR / 'counts'
    if not dst_dirpath.exists(): dst_dirpath.mkdir(parents=True, exist_ok=True)

    with (
        open(INTER_DIR / 'seq_metadata.csv', 'r') as region_fp,
        open(dst_dirpath / f'{dst_fstem}.csv', 'w') as counts_fp,
    ):
        rp_fieldnames = [f.strip() for f in next(region_fp).strip().split(',')]
        rp_reader = csv.DictReader(region_fp, fieldnames=rp_fieldnames)

        # Build Fieldnames
        counts_fieldnames = []

        if temporal_resolution:
            counts_fieldnames.append(temporal_resolution)

        if geographic_resolution == 'zone':
            geo_fieldnames = [f'zone_{climate_zone}']
        else:
            geo_fieldnames = ['continent', 'country', 'region'][:geo_res_index + 1]

        counts_fieldnames += geo_fieldnames

        counts_fieldnames.append('num_seqs')

        counter = Counter()
        output = dict()

        for row in utils.tqdm(rp_reader, total=counts.filtered_sequences(), desc="Reading"):

            new_row = {k: row[k] for k in geo_fieldnames}

            if geographic_resolution != 'zone':
                key = adjust_geographic_resolution(new_row)
                key = '_'.join(key)
            else:
                key = f'{climate_zone}_{row.get(f"zone_{climate_zone}", "")}'

            if temporal_resolution:
                row_period = row.get(period_to_column_translation)
                new_row = {temporal_resolution: row_period, **new_row}
                key += f'_{row_period:s}'

            counter[key] += 1
            if key not in output:
                output[key] = new_row

        # Writer
        count_writer = csv.DictWriter(counts_fp, fieldnames=counts_fieldnames)
        count_writer.writeheader()

        for k, v in utils.tqdm(counter.items(), desc="Writing"):
            row = dict(output[k], num_seqs=v)
            count_writer.writerow(row)


def substitutions_per_geotemporal_bin(geographic_resolution=3, temporal_resolution=1, climate_zone=0):
    """
        Aggregate specific substitutions in a geographical region
        during a period of time given by provided resolutions.

        Parameters
        ----------
        geographic_resolution : int{0, 1, 2, 3} | str
            Resolution of the geographic bin in which sequences are assigned.
                0 = 'Continent',
                1 = 'Country',
                2 = 'Region',
                3 = 'Zone'
            Defaults to 3 [Zone]

        temporal_resolution : int{0, 1, 2} | str
            Resolution of the temporal bin in which sequences are assigned.
                0 = 'Year',
                1 = 'Quarter',
                2 = 'Month'
            Defaults to 1 [Quarter]

        climate_zone : int{0, 1, 2} | str
            The climate zoning type.
                0 = 'simple': Uses simplified latitude demarcations +/-(30, 60),
                1 = 'real' : Uses standard latitude demarcations' +/-(23.27, 66.33),
                2 = 'subtropic' : Adds subtropics +/-(23.27, 35) to 'real'
            Defaults to 0 [simple]
    """

    seq_totals = load.sequences_per_geotemporal_bin(geographic_resolution, temporal_resolution, climate_zone)

    total_num_seqs = seq_totals.num_seqs.sum()

    geographic_resolution_str = utils.convert_geographic_resolution_to_string(geographic_resolution)
    temporal_resolution_str = utils.convert_temporal_resolution_to_string(temporal_resolution)
    climate_zone_str = f'zone_{utils.convert_climate_zone_type_to_string(climate_zone)}'

    dst_fname = geographic_resolution_str if geographic_resolution_str != 'zone' else climate_zone_str

    temporal_column_mapping = {
        'year': 'collection_year',
        'quarter': 'collection_quarter',
        'month': 'collection_month_idx',
    }

    temporal_column = temporal_column_mapping[temporal_resolution_str.lower()]

    # seqsubs_by_temporal_period = load.sequence_substitutions_by_temporal_period(temporal_period_index, temporal_resolution)

    aa_subs_list_str = ''
    with open(INTER_DIR / 'seqsubs' / temporal_resolution_str / 'columns.txt') as fp:
        aa_subs_list_str = next(fp)

    accession_ids_by_period = []
    row_indices_by_period_zone = []
    with open(INTER_DIR / 'seqsubs' / temporal_resolution_str / 'indices.txt') as fp:
        for line in fp:
            accession_ids_by_period.append({v: i for i, v in enumerate(utils.split_line(line))})
            row_indices_by_period_zone.append(defaultdict(list))

    with open(INTER_DIR / 'seq_metadata.csv', 'r') as seq_fp:
        reader = csv.DictReader(seq_fp)

        for row in utils.tqdm(reader, total=total_num_seqs):
            accession_id = row.get('accession_id')
            period_idx = int(row.get(temporal_column))
            climate_zone = row.get(climate_zone_str)

            period_row_index = accession_ids_by_period[period_idx][accession_id]

            del accession_ids_by_period[period_idx][accession_id]

            row_indices_by_period_zone[period_idx][climate_zone].append(period_row_index)


    with open(INTER_DIR / 'seqsubs' / temporal_resolution_str / f'indices_by_{dst_fname}.pkl', 'wb') as fp:
        pickle.dump(row_indices_by_period_zone, fp)

    total_iters = len(row_indices_by_period_zone) * (7 if 'subtropic' in climate_zone_str else 5)

    with (
        open(PROC_DIR / f'{temporal_resolution_str}_{climate_zone_str}_counts.csv', 'w') as counts_fp,
        open(PROC_DIR / f'{temporal_resolution_str}_{climate_zone_str}_props.csv', 'w') as props_fp
    ):
        headers = ','.join([climate_zone_str, temporal_resolution_str, aa_subs_list_str])

        counts_fp.write(headers)
        props_fp.write(headers)

        pbar = utils.tqdm(enumerate(row_indices_by_period_zone), total=total_iters)
        for temporal_period, period_index_dict in pbar:

            data = load.sequence_substitutions_by_temporal_period(temporal_period, temporal_resolution_str)

            for zone, indices in period_index_dict.items():
                counts_fp.write(str(zone) + ',')
                props_fp.write(str(zone) + ',')

                counts_fp.write(str(temporal_period) + ',')
                props_fp.write(str(temporal_period) + ',')

                arr = data.get('table')[indices].sum(axis=0).flatten()[0]

                total_num_seqs = seq_totals[
                    (seq_totals[temporal_resolution_str] == temporal_period) &
                    (seq_totals[climate_zone_str] == float(zone))
                ].num_seqs.to_numpy()

                arr = arr.astype(int)
                counts_fp.write(','.join([f'{c:d}' for c in arr.tolist()[0]]) + '\n')

                props = (arr/total_num_seqs)
                props_fp.write(','.join([f'{c:0.4f}' for c in props.tolist()[0]]) + '\n')
