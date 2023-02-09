import csv

import scipy.sparse

import covmut_seasonal.counts as counts
from covmut_seasonal.settings import (
    PROJECT_DIR, RAW_DIR, INTER_DIR, METADATA_FNAME
)
import covmut_seasonal.utils as utils

def sequence_metadata(metadata_fpath=RAW_DIR / METADATA_FNAME, nrows=None):
    """
        Return generator yielding dictionaries of sequence metadata as.

        Parameters
        ----------
        filepath : str | pathlib.Path
            The path of the metadata file.
            Defaults: metadata filename in `raw` data directory.

        nrows : int | None
            Number of rows to return, starting from first data row (header excluded).
            If `None`, return __all__ rows. Default: `None`

        Yield
        -----
        dict
            Each dictionary returned is a row in the metadata defining a sequence.
            The keys in each dictionary identify the header's fieldname.

    """

    delimiter = '\t'

    with open(metadata_fpath, 'r') as fp:
        # First row is header
        fieldnames = next(fp).strip().split(delimiter)

        csv_reader = csv.DictReader(fp, fieldnames=fieldnames, delimiter=delimiter)

        for i, row in enumerate(csv_reader):
            # quit iterator after nrows
            if nrows != None and i >= (nrows):
                break

            yield row

    return None


def aa_subs_by_sequence():
    """
        Return AA Substitutions for each sequence as a generator

        Yields
        ------
        tuple of (str, list of str)
            A tuple containing the sequence `accession_id` and a list of
            substitutions found in the sequence.

    """
    with open(INTER_DIR / 'aa_subs_by_sequence.txt', 'r') as fp:
        for line in fp:
            line = line.strip().split(',')
            yield (line[0], line[1:])

    return


def country_geoclimates():
    """
        Retrieve per-country latitude and climate zones.

        Returns
        -------
        dict of dict | dict
            Latitude and climate zone data keyed by country.
            Takes the form:
                {
                    country:str : {
                        latitude: float,
                        longitude: float,
                        zone_simple: int,
                        zone_real: int,
                        zone_subtropic: float
                    }, ...
                }
    """

    with open(INTER_DIR / 'country_geo_climate.csv', 'r') as fp:
        reader = csv.DictReader(fp)

        return {
            row.get('country'): {k: v for k, v in row.items() if k != 'country'}
            for row in reader
        }


def get_indices_by_temporal_period(temporal_period_index, temporal_resolution=2):
    """
        Retrieve a list of Sequence Accession ID for a given temporal period and resolution

        Parameters
        ----------
        temporal_period_index : int
            The period index to load. Must be within available range,
            dependent on the temporal resolution

        temporal_resolution : int{0, 1, 2} | str ['year', 'quarter', 'month']
            Resolution of the temporal bin in which sequences are assigned
                0 = 'Year',
                1 = 'Quarter',
                2 = 'Month'
            Defaults to 1 [Quarter]

        Returns
        -------
        list
            A list of sequence Accession IDs for the given temporal period
            and temporal resolution.
    """

    temporal_resolution = utils.convert_temporal_resolution_to_string(temporal_resolution)

    index_fname = INTER_DIR / 'seqsubs' / temporal_resolution / f'indices.txt'

    if not index_fname.exists():
        raise FileNotFoundError((
            f'Index file of {temporal_resolution}ly Sequence Accession IDs was '
            f'not found at expected location: `{index_fname.relative_to(PROJECT_DIR)}`'
        ))

    with open(index_fname, 'r') as fp:
        curr_period_index = 0

        for line in fp:
            if curr_period_index == temporal_period_index:
                return utils.split_line(line)
            curr_period_index += 1

    raise ValueError((
        f'Index for temporal period #{temporal_period_index} at '
        f'temporal resolution {temporal_resolution} was not found.'
    ))


def get_columns_by_temporal_resolution(temporal_resolution=2):
    """
        Retrieve a list of AA Substitutions for a given temporal period

        Parameters
        ----------
        temporal_resolution : int{0, 1, 2} | str ['year', 'quarter', 'month']
            Resolution of the temporal bin in which sequences are assigned
                0 = 'Year',
                1 = 'Quarter',
                2 = 'Month'
            Defaults to 1 [Quarter]

        Returns
        -------
        list
            A list of AA Substitutions for sequences at the provided temporal resolution.
    """

    temporal_resolution = utils.convert_temporal_resolution_to_string(temporal_resolution)

    column_fname = INTER_DIR / 'seqsubs' / temporal_resolution / f'columns.txt'

    if not column_fname.exists():
        raise FileNotFoundError((
            f'Column file for {temporal_resolution}ly AA Substitutions was '
            f'not found at expected location: `{column_fname.relative_to(PROJECT_DIR)}`'
        ))

    with open(column_fname, 'r') as fp:
        return utils.split_line(next(fp))


def sequence_substitutions_by_temporal_period(temporal_period_index, temporal_resolution=2):
    """
        Retrieve a dictionary of sequence indexed substitutions as sparse array.
        Each substitution column is marked with a 1 or 0, or exists / does not exist.

        Parameters
        ----------
        temporal_period_index : int
            The period index to load. Must be within available range,
            dependent on the temporal resolution

        temporal_resolution : int{0, 1, 2} | str ['year', 'quarter', 'month']
            Resolution of the temporal bin in which sequences are assigned
                0 = 'Year',
                1 = 'Quarter',
                2 = 'Month'
            Defaults to 1 [Quarter]

        Returns
        -------
        dict[accession_ids: list[str], aa_substitutions: list[str], table: scipy.sparse.csr_array)
            A dictionary containing a sparse array, accession ID index and
            AA Substitution column labels
    """

    temporal_resolution = utils.convert_temporal_resolution_to_string(temporal_resolution)

    csr_fname = INTER_DIR / 'seqsubs' / temporal_resolution / f'{temporal_period_index:02d}.npz'

    if not csr_fname.exists():
        raise FileNotFoundError((
            f'Sequence substitutions for period #{temporal_period_index} '
            f'at temporal resolution {temporal_resolution} were not found at '
            f'expected location: `{csr_fname.relative_to(PROJECT_DIR)}`'
        ))

    return {
        'accession_ids': get_indices_by_temporal_period(temporal_period_index, temporal_resolution),
        'aa_substitutions': get_columns_by_temporal_resolution(temporal_resolution),
        'table': scipy.sparse.load_npz(csr_fname)
    }


def sequences_per_geotemporal_bin(geographic_resolution=3, temporal_resolution=1, climate_zone=0):
    """
        Get number of sequences in a geographical region during a period of time
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

    geographic_resolution = utils.convert_geographic_resolution_to_string(geographic_resolution)
    temporal_resolution = utils.convert_temporal_resolution_to_string(temporal_resolution)
    climate_zone = utils.convert_climate_zone_type_to_string(climate_zone)

    fstem = f'seq_count_by_{temporal_resolution}_{geographic_resolution}'

    if geographic_resolution == 'zone':
        fstem += f'_{climate_zone}'

    import pandas

    return pandas.read_csv(INTER_DIR / 'counts' / f'{fstem}.csv')
