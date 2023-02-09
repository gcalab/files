"""
Functions for decompressing and converting source data into immutable subsets.
"""

from pathlib import Path

import tarfile

import covmut_seasonal.counts as counts
import covmut_seasonal.load as load
from covmut_seasonal.transform import sparsify
from covmut_seasonal.settings import (
    EXTERNAL_DIR, RAW_DIR, INTER_DIR,
    METADATA_TAR_FNAME, METADATA_FNAME,
    temporal_resolutions
)
import covmut_seasonal.utils as utils

def untar(
    archive_fpath=EXTERNAL_DIR / METADATA_TAR_FNAME,
    dst_dirpath=RAW_DIR,
    force_overwrite=False
):
    """
        Decompress tar archive to the parent directory of the archive.

        Parameters
        ----------
        archive_fpath : str | pathlib.Path
            Filepath of `tar.xz` archive.
            Defaults to the archive in the `external` data directory matching
                the GISAID version specified in the `.env` file and `settings.py`.
        dst_path : str | pathlib.Path | None
            Destination directory for decompressed file. If `None`, directory is
            set to the parent directory of the archive.
            Defaults to `raw` data directory.

        Raises
        ------
        FileNotFoundError
            Archive file provided does not exist.
        NotADirectoryError
            Destination directory does not exist or does not point to a directory.
        KeyError
            File `metadata.tsv` does not exist in tar archive.
    """

    try:
        if not isinstance(archive_fpath, Path):
            archive_fpath = Path(archive_fpath)
        if not archive_fpath.exists():
            raise FileNotFoundError()
    except (TypeError, FileNotFoundError):
        raise FileNotFoundError(f'File {archive_fpath} not found.')

    try:
        if not isinstance(dst_dirpath, Path):
            dst_dirpath = Path(dst_dirpath)
    except TypeError:
        # not a filetype
        dst_dirpath = archive_fpath.parents[0]

    # Throw error if file already exists and force overwrite is not specified
    if not force_overwrite and (dst_dirpath / METADATA_FNAME).exists():
        raise FileExistsError(
            f'File `{METADATA_FNAME}` already exists in {dst_dirpath}.'
        )

    # Throw error if destination is not a known directory
    if not (dst_dirpath.exists() and dst_dirpath.is_dir()):
        raise(NotADirectoryError(
            f'Destination `{dst_dirpath}` is not a directory.'
        ))

    # Attempt to extract metadata file.
    with tarfile.open(archive_fpath, mode='r:xz') as tar_fp:
        try:
            info = tar_fp.getmember('metadata.tsv')
        except KeyError:
            raise KeyError(
                f'File `metadata.tsv` does not exist in tar archive `{archive_fpath}`.'
            )

        tar_fp.extractall(dst_dirpath, members=[info])

    Path(dst_dirpath / 'metadata.tsv').rename(dst_dirpath / METADATA_FNAME)


def aa_substitution_feature_map(temporal_resolution=1, top_n_subs=None):
    """
        Create a feature map using aa_substitutions as features for each sequence.

        Parameters
        ----------
        temporal_resolution : int{0, 1, 2} | str ['year', 'quarter', 'month']
            Resolution of the temporal bin in which sequences are assigned
                0 = 'Year',
                1 = 'Quarter',
                2 = 'Month'
            Defaults to 1 [Quarter]

        top_n_subs : int | None
            The number of AA Substitutions to include, starting from
            the substitution with the highest incidence across collective
            sequences (likely "Spike D614G").

            If `None`, all substutions with incidence rates one standard deviation
            over the mean rate are used.

    """

    temporal_resolution_str = utils.convert_temporal_resolution_to_string(temporal_resolution)

    # Get relevant subs
    import pandas as pd
    subs_df = pd.read_csv(INTER_DIR / 'counts' / 'unique_substitutions.csv', index_col='aa_sub')

    if not top_n_subs:
        condition = subs_df.num_seqs >= subs_df.num_seqs.std() + subs_df.num_seqs.mean()
        subs_of_interest = subs_df[condition]
    else:
        subs_of_interest = subs_df.num_seqs.nlargest(top_n_subs)

    subs_of_interest = subs_of_interest.index.to_list()

    columns = {
        'year': 'collection_year',
        'quarter': 'collection_quarter',
        'month': 'collection_month_idx',
    }

    # Sequence metadata to filter with
    seq_df = pd.read_csv(
        INTER_DIR / 'seq_metadata.csv',
        index_col='accession_id',
        usecols=[
            'accession_id',
            columns.get(temporal_resolution_str)
        ]
    ).rename(columns={
        columns.get(temporal_resolution_str): temporal_resolution_str
    })

    dst_dirpath = INTER_DIR / 'seqsubs' / temporal_resolution_str
    # create directory
    if not dst_dirpath.exists(): dst_dirpath.mkdir(exist_ok=True, parents=True)

    # File for each temporal period
    files = {
        i: open(dst_dirpath / f'{i:02d}.csv', 'w')
        for i in range(seq_df[temporal_resolution_str].nunique())
    }

    fieldnames = ['accession_id', *subs_of_interest]

    for fp in files.values():
        fp.write(','.join(fieldnames) + '\n')

    try:
        pbar = utils.tqdm(load.aa_subs_by_sequence(), total=counts.filtered_sequences())
        pbar.set_description(f'Saving {temporal_resolution_str}ly sequences')
        for (accession_id, aa_subs) in pbar:
            temporal_period = seq_df.loc[accession_id, temporal_resolution_str]
            feature_row = ['1' if aa_sub in aa_subs else '' for aa_sub in subs_of_interest]
            files.get(temporal_period).write(','.join([accession_id, *feature_row]) + '\n')

    finally:
        for fp in files.values():
            fp.close()

    # create
    sparsify(temporal_resolution)
