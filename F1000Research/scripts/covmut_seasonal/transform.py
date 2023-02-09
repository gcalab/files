"""
Prepares extracted raw data into forms appropriate for analyses.
"""

import csv
from datetime import datetime
import dateutil
import math
import unicodedata

import pandas as pd
import numpy as np
import scipy.sparse

import covmut_seasonal.load as load
from covmut_seasonal.settings import (PROJECT_DIR, RAW_DIR, INTER_DIR)
import covmut_seasonal.utils as utils

def date_to_dict(date_string):
    """
        Convert date string into a dictionary.

    Parameters
    ----------
    date_string : str
        A string in the python date format `%Y-%m-%d`.

    Returns
    -------
    dict
        A dictionary with the keys "year", "month", and "day".

    Raises
    ---------
    ValueError
        Date cannot be parsed from argument string.

    """

    # print(date_string)
    # num_components = len(date_string.split('-'))
    # print(num_components)

    # if num_components == 3:
    #     dt = datetime.strptime(date_string, "%Y-%m-%d")
    # elif num_components == 2:
    #     dt = datetime.strptime(date_string, "%Y-%m")
    # else:
    #     dt = datetime(0,0,0)

    num_components = 3

    try:
        num_components = len(date_string.split('-'))
    except:
        pass

    dt = dateutil.parser.parse(date_string)

    return dict(year=dt.year, month=dt.month, day=dt.day if num_components == 3 else 1)


def format_date(date, prefix):
    """
        Convert input date into a dictionary.

    Parameters
    ----------
    date : str | dict
        A string in the python date format `%Y-%m-%d`.

    prefix : str
        A string to append to the front of the returned dict keys.

    Returns
    -------
    dict
        A dictionary with the keys "year", "month", and "day".
        These will have the provided prefix, separated by an underscore.
        Example:    "2020-02-14" as the date string and "foo" as the prefix
                    will produce the dictionary
                    `{'foo_year': 2020, 'foo_month': 2, 'foo_day': 14}`

    Raises
    ---------
    ValueError
        Date cannot be parsed from argument string.

    """

    try:
        date = date_to_dict(date)
    except TypeError as e:
        pass

    return {f'{prefix}_{k}': v for k, v in date.items()}


def get_quarter(date):
    """
        Given a year and a month, find the 1-indexed number of quarters since
        January 2020 (quarter 1).

    Parameters
    ----------
    date : dict{[year|month|day] : int} | str | datetime.datetime
        The date used to find the quarter number after January 2020.
        If a string or datetime object, this method will attempt conversion
        to a formatted dict object.

    Returns
    -------
    int
        The 1-indexed quarter from January 2020.
        Examples: June 2020 = 2, August 2021 = 7; December 2021 = 8

    Raises
    ---------
    ValueError
        Provided dictionary format is incorrect or date cannot be parsed
        from the provided string.

    """

    if hasattr(date, 'year') and hasattr(date, 'month'):
        date = dict(year=date.year, month=date.month)
    elif isinstance(date, str):
        date = date_to_dict(date)

    base_quarter = (date.get('year') - 2020) * 4
    year_quarter = math.ceil(date.get('month') / 3)

    return base_quarter + year_quarter


def get_month_idx(date):
    """
        Given a year and a month, get the number of months since January 2020,
        indexed by 1.

    Parameters
    ----------
    date : dict{[year|month|day] : int} | str | datetime.datetime
        The date used to find the quarter number after January 2020.
        If a string or datetime object, this method will attempt conversion
        to a formatted dict object.

    Returns
    -------
    int
        The 1-indexed quarter from January 2020.
        Examples: June 2020 = 6, August 2021 = 7; December 2021 = 8

    Raises
    ---------
    ValueError
        Provided dictionary format is incorrect or date cannot be parsed
        from the provided string.

    """

    if hasattr(date, 'year') and hasattr(date, 'month'):
        date = dict(year=date.year, month=date.month)
    elif isinstance(date, str):
        date = date_to_dict(date)

    base_month = (date.get('year') - 2020) * 12
    year_months = math.ceil(date.get('month'))

    return base_month + year_months


def format_bool(field_data):
    """
        Convert input string into a boolean (True|False).

        Parameters
        ----------
        field_data : str | int | None
            A string identifiable as a boolean object. `None` is `False`
            Input arguments "True", "true", "t", 1, and "1" are `True`.
            All other strings, objects, or exceptions are `False`.

        Returns
        -------
        bool
            The boolean representation of the input.

    """

    try:
        if field_data == 1:
            return True
    except TypeError as e:
        pass

    try:
        lower_data = field_data.lower()
    except AttributeError as e:
        return False

    return any([s == lower_data for s in ['true', 't', '1']])


def normalize_characters(field_data):
    """
        Removes accent characters, converting string to nearest Anglicized format.

        Parameters
        ----------
        field_data : str
            A string

        Returns
        -------
        str
            A string with accent characters removed.

    """
    return (unicodedata.normalize('NFKD', field_data)
                .encode('ascii', 'ignore') # to ascii
                .decode('utf8', 'ignore') # back to utf8
            )


def reconcile_country_names(field_data):
    """
        Convert country names which do not match with geographic data
        to other form. Return the original if no change is found.

        Parameters
        ----------
        field_data : str
            A string representing a country

        Returns
        -------
        str
            A reconciled region name.

    """
    region_renames = {
        'USA': 'United States',
        'The Bahamas': 'Bahamas',
        'Democratic Republic of the Congo': 'Congo [DRC]',
        'Republic of the Congo': 'Congo [Republic]',
        'Myanmar': 'Myanmar [Burma]',
        'Palestine': 'Palestinian Territories',
        'North Macedonia': 'Macedonia [FYROM]',
        'Saint Martin': 'Sint Maarten'
    }

    return region_renames.get(field_data, field_data)


def calculate_climate_zone(latitude, method='simple'):
    """
        Returns the global latitudinal climate zone.

        Takes the range [-2,-1,0,1,2]:
            * -2 and 2 are ant/arctic
            * -1 and 1 are temperate
            * 0 is tropical

        The simplified 30/60 deg. version was used in the paper
        "The seasonal behaviour of COVID-19 and its galectin-like culprit of the viral spike"

        Parameters
        ----------
        latitude : float
            The latitude for determining the zone

        method : str ('real', 'subtropic', 'simple') | None
            Demarcation method for splitting latitude into climate zones.
            If 'simple' or `None`, use the +/- 30/60 degree demarcation.
            If 'real', use 23.27/66.33 degree demarcations.
            If 'subtropic' or None, use the +/- 23.27/35/66.33 degree demarcation.
            Defaults to 'simple'.


        Returns
        -------
        float
            The Climate Zone.
            If `method` is 'real' or 'simple', integers will be returned:
                * 0 is Equatorial Tropics,
                * +/-1 are Temperate,
                * +/- 2 are Antarctic/Arctic.
            If `method` is 'subtropic', floats will be used. In addition:
                * +/- 0.5 is Subtropical

    """
    zone = 0 # Tropics

    if not method:
        method = 'simple'
    else:
        method = method.lower()

    # Actual Arctic/Antarctic circles begin at 66.33 and -66.33
    max_temperate_zone = 60 if method == 'simple' else 66.33
    min_temperate_zone = 30 if method == 'simple' else 23.27
    subtropic_zone = 35

    if latitude > max_temperate_zone:
        # Arctic
        zone = 2
    elif latitude >= min_temperate_zone and latitude <= max_temperate_zone:
        # Temperate (North of Tropic of Cancer)
        zone = 1
    elif latitude <= -min_temperate_zone and latitude >= -max_temperate_zone:
        # Temperate (North of Tropic of Capricorn)
        zone = -1
    elif latitude < -max_temperate_zone:
        # Antarctic
        zone = -2

    if method == 'subtropic' and abs(zone) == 1 and abs(latitude) < subtropic_zone:
        # zone = 35 * zone #  zone will either be 1 or -1
        zone = 0.5 if zone > 0 else -0.5

    return zone


class MetadataFields:
    """
        Convert metadata field strings into desired formats.
    """

    alt_fieldnames = {
        'Accession ID': 'accession_id',
        'Virus name': 'virus_name',
        'AA Substitutions': 'aa_substitutions',
        'Collection date': 'collection_date',
        'Submission date': 'submission_date',
        'Location': 'location',
        'Host': 'host',
        'Is complete?': 'is_complete',
        'Is high coverage?': 'is_high_coverage',
        'Clade': 'clade',
        'Variant': 'variant'
    }

    @staticmethod
    def aa_substitutions(field_data):
        """
            Convert input data into a set of string elements.

        Parameters
        ----------
        field_data : str
            A string in the format of a list of strings separated by commas,
            enclosed in parentheses, i.e., `(` and `)`.

        Returns
        -------
        set of strings
            A immutable set of substitutions as strings.

        """

        return set(field_data[1:-1].split(','))

    @staticmethod
    def collection_date(field_data):
        """
            Convert input data into an expanded date dictionary.

        Parameters
        ----------
        field_data : str
            A string in the python date format `%Y-%m-%d`.

        Returns
        -------
        dict
            A dictionary with the keys "collection_year", "collection_month",
            "collection_day", "collection_quarter", "collection_month_idx"
             as well as the original field.

        """

        date_dict = date_to_dict(field_data)

        return {
            'collection_quarter': get_quarter(date_dict),
            'collection_month_idx': get_month_idx(date_dict),
            'collection_date': field_data,
            **format_date(date_dict, 'collection')
        }

    @staticmethod
    def submission_date(field_data):
        """
            Convert input data into an expanded date dictionary.

        Parameters
        ----------
        field_data : str
            A string in the python date format `%Y-%m-%d`.

        Returns
        -------
        dict
            A dictionary with the keys "submission_year", "submission_month",
            "submission_day", "submission_quarter", "submission_month_idx"
            as well as the original field.

        """

        date_dict = date_to_dict(field_data)

        return {
            'submission_quarter': get_quarter(date_dict),
            'submission_month_idx': get_month_idx(date_dict),
            'submission_date': field_data,
            **format_date(date_dict, 'submission')
        }

    @staticmethod
    def location(field_data):
        """
            Convert an input string into location fields of descending granularity.

        Parameters
        ----------
        field_data : str
            A string with 3 subfields, separated by a '/'.
            Example: "{Continent} / {Country} / {Region}"

        Returns
        -------
        dict of str
            A dictionary object containing the keys "continent", "country", "region".

        """

        subfields = [s.strip() for s in field_data.split('/', 3)]
        subfields = dict(zip(["continent", "country", "region", "subregion"], subfields))
        subfields['country'] = reconcile_country_names(subfields['country'])
        return dict(location=field_data, **subfields)

    @staticmethod
    def host(field_data):
        """
            Convert input string into either "human", "not human", or "unknown".

            Parameters
            ----------
            field_data : str
                A string identifying the affected host.

            Returns
            -------
            str
                A string: either "human", "not human", or "unknown"
        """

        try:
            host = field_data.lower()
        except Exception as e:
            # this cannot be 'human' if it is not a string.
            return 'unknown'

        if host == 'human':
            return 'human'
        else:
            return 'not human'

    @staticmethod
    def is_complete(field_data):
        """
            Convert input string into a boolean (True|False).

            A convenience wrapper for the `_format_bool` method.

            Parameters
            ----------
            field_data : str | None
                A string identifiable as a boolean object. `None` is `False`
                Input arguments "True", "true", "t", 1, and "1" are `True`.
                All other strings, objects, or exceptions are `False`.

            Returns
            -------
            bool
                The boolean representation of the input.

        """

        return format_bool(field_data)

    @staticmethod
    def is_high_coverage(field_data):
        """
            Convert input string into a boolean (True|False).

            A convenience wrapper for the `_format_bool` method.

            Parameters
            ----------
            field_data : str | None
                A string identifiable as a boolean object. `None` is `False`
                Input arguments "True", "true", "t", 1, and "1" are `True`.
                All other strings, objects, or exceptions are `False`.

            Returns
            -------
            bool
                The boolean representation of the input.

        """

        return format_bool(field_data)


##
# Data Row Transformations
##

def format_row(data):
    """
        Converts an input dictionary with appropriate fields into the desired form.

        Parameters
        ----------
        data : dict of str
            Original dictionary as exported from source file

        Returns
        -------
        dict
            Dictionary in format suitable for analysis.
    """
    _data = {}

    for k, v in data.items():
        new_key = MetadataFields.alt_fieldnames.get(k)
        new_val = None

        # skip fields without alt-names
        if not new_key:
            continue

        # transforms field, if a transformation exists.
        # Otherwise, the field remains unchanged.
        if new_key in MetadataFields.__dict__:
            new_val = MetadataFields.__dict__.get(new_key).__func__(v)
        else:
            new_val = v

        if isinstance(new_val, dict):
            _data.update(new_val)
        else:
            _data[new_key] = new_val

    return _data


def interim_comparator(x):
    """
        Checks if a dictionary-like object fulfills conditionals.

        Parameters
        ----------
        x : dict | dict-like object
            An object with the keys 'host', 'is_complete', and 'is_high_coverage'.

        Returns
        -------
        bool
            Result qualification of the comparison.
    """
    return x.get('host') == 'human' and (x.get('is_complete') or x.get('is_high_coverage'))



def format_and_filter_rows(data, comparator=lambda x: True):
    """
        A generator converting an iterator or generator returning dictionaries
        to pre-defined field formatting, keeping only rows which pass desired
        inclusion/exclusion tests.

        Parameters
        ----------
        data : iterator or generator of dicts
            Rows in source data

        comparator : function or None
            Filter for formatted rows. Should return `True` for inclusion
            of the row and `False` for exclusion of the row.
            Default: Inclusive identity (always passes)

        Yields
        ------
        dict
            Formatted dictionary passing comparator test.


    """
    try:
        while (_data := next(data)) is not None:
            try:
                new_row = format_row(_data)
                if comparator != None and comparator(new_row):
                    yield new_row
            except ValueError as e:
                print(e)
                pass
    except FileNotFoundError as e:
        raise(e)
    except Exception as e:
        print(e)
        raise(e)


##
# Write Data
##

def country_geo_climate(
    src_fpath=RAW_DIR / 'country_coords.csv',
    dst_fpath=INTER_DIR / 'country_geo_climate.csv'
):
    """
        Normalize names in country coordintes file and include climate zone info.
        Write results to `dst_fpath` in CSV format.

        Parameters
        ----------
        src_fpath : str | pathlib.Path
            Filepath to unformatted country coordinate data.

        dst_fpath : str | pathlib.Path
            Filepath to save formatted country coordinate data with zones.
    """

    dst_fieldnames = ['country', 'latitude', 'longitude', 'zone_simple', 'zone_real', 'zone_subtropic']

    with (
        open(src_fpath, 'r') as src_fp,
        open(dst_fpath, 'w') as dst_fp
    ):
        src_fieldnames = utils.split_line(next(src_fp))
        src_reader = csv.DictReader(src_fp, fieldnames=src_fieldnames)

        dst_writer = csv.DictWriter(dst_fp, fieldnames=dst_fieldnames)
        dst_writer.writeheader()

        country_names = []

        for row in src_reader:
            try:
                country_name = normalize_characters(row['name'])

                # Country Names should be unique
                if country_name in country_names:
                    continue

                country_names.append(country_name)

                latitude = float(row['latitude'])
                longitude = float(row['longitude'])

                new_row = {
                    # Normalize Country names (remove accents)
                    'country': country_name,
                    'latitude': latitude,
                    'longitude': longitude,
                    'zone_simple': calculate_climate_zone(latitude, 'simple'),
                    'zone_real': calculate_climate_zone(latitude, 'real'),
                    'zone_subtropic': calculate_climate_zone(latitude, 'subtropic')
                }
            except Exception as e:
                # print(e)
                # print(row)
                pass

            dst_writer.writerow(new_row)

def raw_to_interim(
    raw_metadata,
    seq_metadata_fpath=INTER_DIR / 'seq_metadata.csv',
    seq_aa_sub_fpath=INTER_DIR / 'aa_subs_by_sequence.txt',
    force_overwrite=False
):
    f"""
        Formats fields for each row in the raw metadata file and saves
        result region-period and substitution data to respective files,
        indexed by the `accession_id`.

        Parameters
        ----------
        raw_metadata : iterable of dict | generator of dict
            A iterable object containing dictionaries, each representing
                a row in the "raw" metadata file.

        seq_aa_sub_fpath : str | pathlib.Path
            Filepath to save per-line sequence-AA substitutions mapping
            as a ragged (non-uniform), comma-separated row.

        seq_aa_sub_fpath : str | pathlib.Path
            Filepath to save per-line sequence-AA substitutions mapping
            as a ragged (non-uniform), comma-separated row.

        force_overwrite : bool
            If "truthy", overwrite output files `{INTER_DIR / 'seq_metadata.csv'}`
            and/or `{INTER_DIR / 'aa_subs_by_sequence.txt'}` if they exist.
            If "falsy", an error will be raised if the files already exist.

        Raises
        ------
        FileExistsError
            The files `{INTER_DIR / 'seq_metadata.csv'}` and/or
            `{INTER_DIR / 'aa_subs_by_sequence.txt'}` already exists and
            `force_overwrite` is "falsy"

    """

    fieldnames = [
        "accession_id",
        "collection_quarter",
        "collection_month_idx",
        "collection_year",
        "collection_month",
        "collection_day",
        "submission_quarter",
        "submission_month_idx",
        "submission_year",
        "submission_month",
        "submission_day",
        "continent",
        "country",
        "region",
        "zone_simple",
        "zone_real",
        "zone_subtropic"
    ]

    # returns a generator
    rows = format_and_filter_rows(raw_metadata, interim_comparator)
    country_geoclimates = load.country_geoclimates()

    if not force_overwrite:
        error_tmpl = lambda s: f'Cannot write data. Files {str(s)} already exist'
        err_msg = None
        if seq_metadata_fpath.exists() and seq_aa_sub_fpath.exists():
            err_msg = error_tmpl(f'`.{seq_metadata_fpath.relative_to(PROJECT_DIR)}` and `.{seq_aa_sub_fpath.relative_to(PROJECT_DIR)}`') + '.'
        elif seq_metadata_fpath.exists() and not seq_aa_sub_fpath.exists() :
            err_msg = error_tmpl(f'`.{seq_metadata_fpath.relative_to(PROJECT_DIR)}`') + 's.'
        elif seq_aa_sub_fpath.exists():
            err_msg = error_tmpl(f'`.{seq_aa_sub_fpath.relative_to(PROJECT_DIR)}`') + 's.'

        if err_msg and (input(utils.overwrite_warning_msg) != 'y'):
            raise FileExistsError(err_msg)

    with (
        open(seq_metadata_fpath, 'w') as rp_fp,
        open(seq_aa_sub_fpath, 'w') as sub_fp
    ):
        # skip keys not in `fieldnames` (such as `aa_substitutions`)
        writer = csv.DictWriter(rp_fp, fieldnames=fieldnames, extrasaction='ignore')
        writer.writeheader()

        for row in utils.tqdm(rows, total=9957705):
            try:
                geoclimes = country_geoclimates.get(row.get('country'))
            except AttributeError:
                continue

            # invalid country
            if not geoclimes:
                continue

            try:
                new_row = {
                    **row,
                    'zone_simple': geoclimes.get('zone_simple'),
                    'zone_real': geoclimes.get('zone_real'),
                    'zone_subtropic': geoclimes.get('zone_subtropic'),
                }
            except AttributeError as e:
                pass


            writer.writerow(new_row)

            aa_subs = new_row.get('aa_substitutions')

            # write in "ragged" row format (not a true CSV)
            reformat_subs = ','.join(list(aa_subs))
            sub_fp.write(f'{new_row.get("accession_id")},{reformat_subs}\n')


def sparsify(temporal_resolution=1):
    """
        Convert CSV files of the specified temporal resolution to
        sparse arrays along with line-matched indices
        for sequence identification.

        temporal_resolution : int{0, 1, 2} | str
            Resolution of the temporal bin in which sequences are assigned.
                0 = 'Year',
                1 = 'Quarter',
                2 = 'Month'
            Defaults to 1 [Quarter]
    """

    temporal_resolution_str = utils.convert_temporal_resolution_to_string(temporal_resolution)

    parent_path = (INTER_DIR / 'seqsubs' / temporal_resolution_str)

    num_cols = 0 # NOTE: we could just read 'md_counts_unique_substitutions'

        ## Clear files so we can append
    if (parent_path / 'columns.txt').exists():
        (parent_path / 'columns.txt').unlink()

    if (parent_path / 'indices.txt').exists():
        print('clearing')
        (parent_path / 'indices.txt').unlink()

    seqsub_files = sorted(parent_path.glob('*.csv'))
    for filepath in seqsub_files:

        # temporal_resolution_str = filepath.parent
        period_idx = filepath.stem

        sparse_arr = None

        # Write Indices and Columns
        with filepath.open('r') as fp:

            # one write per period
            # if not column_files.get(temporal_resolution):
            header = next(fp)

            if not (parent_path / 'columns.txt').exists():
                num_cols = len(header.split(',')) - 1 # ignore index header
                with open(parent_path / 'columns.txt', 'w') as col_fp:
                    # first line (minus first entry) are the subs
                    # This already ends in a newline
                    col_fp.write(header.split(',', 1)[-1])

            # https://stackoverflow.com/a/1939102
            sparse_arr = scipy.sparse.csr_array((0, num_cols), dtype=float)

            # get number of rows and cols for sparse array
            num_rows = sum(1 for _ in fp)
            # return to first row and exclude header
            fp.seek(0)
            next(fp)

            with utils.tqdm(fp, total=num_rows) as pbar_b:
                # if not index_files.get(temporal_resolution):
                row_count = 0
                with open(parent_path / 'indices.txt', 'a') as idx_fp:
                    first_seq_name, count_data = next(fp).strip().split(',', 1)
                    idx_fp.write(first_seq_name)

                    sparse_rows = [[float(i or 0) for i in count_data.split(',')]]
                    row_count += 1

                    for row in pbar_b:
                        seq_name, count_data = row.strip().split(',', 1)
                        # comma prior to name avoids trailing comma
                        idx_fp.write(f',{seq_name}')

                        sparse_rows.append([float(i or 0) for i in count_data.split(',')])

                        if row_count % 10000 == 0:
                            sparse_rows = scipy.sparse.csr_array(sparse_rows)
                            sparse_arr = scipy.sparse.vstack([sparse_arr, sparse_rows])
                            nbytes = float(sparse_arr.data.nbytes + sparse_arr.indptr.nbytes + sparse_arr.indices.nbytes)
                            pbar_b.set_description(f'{filepath.stem} @ {row_count} size (mb): {(nbytes/1024**2):.04f}')

                            sparse_rows = []

                        row_count += 1

                    # capture last bunch, if the len was less than 10000
                    sparse_rows = scipy.sparse.csr_array(sparse_rows)
                    sparse_arr = scipy.sparse.vstack([sparse_arr, sparse_rows])

                    idx_fp.write(f'\n')

                nbytes = sparse_arr.data.nbytes + sparse_arr.indptr.nbytes + sparse_arr.indices.nbytes
                pbar_b.set_description(f'{filepath.stem} final size (mb): {(nbytes / (1024**2)):.04f}')
                pbar_b.update(0)
                pbar_b.refresh(True)

            # Save sparse array as compressed numpy file
            if sparse_arr is not None:
                sparse_fpath = filepath.parents[0] / f'{period_idx:s}.npz'
                scipy.sparse.save_npz(sparse_fpath, sparse_arr)