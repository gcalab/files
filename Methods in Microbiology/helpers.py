import re
import pandas as pd
import numpy as np
import matplotlib as mpl

class RegEx():
    short_idx = re.compile(r'.*EPI_ISL_(?P<id>[0-9]{6}).*$')
    virus_code = r'(?P<reg>[A-Z|\-]+)-?(?P<num>[0-9][0-9A-Z_]+)'


def format_column_names(df):
    colnames = df.columns
    colnames = colnames.str.replace(r'[\s|-]', '_', regex=True)
    colnames = colnames.str.replace('?', '', regex=False)
    return df.rename(
        # columns={c: colnames[i].lower() for i, c in enumerate(df.columns)}
        columns=dict(zip(df.columns, colnames.str.lower()))
    )


def get_quarter(df):
    """
    Return yearly quarter in integer format using month in int format (1-12).
    """
    month = int(df.month)
    if month <= 3:
        return 1
    elif month > 3 and month <= 6:
        return 2
    elif month > 6 and month <= 9:
        return 3
    elif month > 9:
        return 4


def get_period(df, starting_year=2020):
    """
        Return year-month period as an integer.

        Starts on Jan. 1st 2020 (= 1).
    """
    year = df.year

    quarter = int(df.quarter) if 'quarter' in df else get_quarter(df)
    offset = (year - starting_year) * 4

    return quarter + offset


def get_formatted_virus_code(virus_name_series):
    """
    Convert long-form `virus name` into a shorter string
        with location identifier and zero-padded (8) alphanumeric code.

    "hCoV-19/Wuhan/Hu-1/2019" -> "Hu-00000001"
    """
    virus_code_df = virus_name_series.str.split('/', expand=True)
    virus_code_df = virus_code_df.drop(columns=[0,1,3])[2]

    regex_virus_code = RegEx.virus_code
    virus_code_df = virus_code_df.str.extractall(regex_virus_code)
    virus_code_df = virus_code_df[['reg', 'num']].droplevel(1)

    virus_code_df['num'] = virus_code_df['num'].str.pad(8, fillchar='0')
    virus_code_df = virus_code_df.apply(
        lambda x: x['reg'].strip('-') + '-' + x['num'],
        axis=1
    ).rename('virus_code')

    return virus_code_df


class RawMetadataImportConfig:
    import_args = dict(
        usecols = [
            'Virus name',
            'Type',
            'Accession ID',
            'Collection date',
            'Location',
            # 'Additional location information',
            # 'Sequence length',
            'Host',
            # 'Patient age',
            # 'Gender',
            'Clade',
            'Variant',
            'AA Substitutions',
            'Submission date',
            # 'Is reference?',
            'Is complete?',
            # 'Is high coverage?',
            'Is low coverage?',
            # 'N-Content',
            # 'GC-Content'
        ],
        dtype = {
            'Virus name': 'string',
            'Type': 'string',
            'Accession ID': 'string',
            'Location': 'string',
            # 'Additional location information': 'string',
            # 'Sequence length': 'Int32',
            'Host': 'string',
            # 'Patient age': 'string',
            # 'Gender': 'string',
            'Clade': 'string',
            'Variant': 'string',
            'AA Substitutions': 'string',
            # 'Is reference?': 'boolean',
            'Is complete?': 'boolean',
            # 'Is high coverage?': 'boolean',
            'Is low coverage?': 'boolean',
            # 'N-Content': 'float32',
            # 'GC-Content': 'float32',
        },
        na_values = {
            # 'Patient age': 'unknown'
        },
        parse_dates = [
            'Collection date',
            'Submission date'
        ]
    )

    nafills = {
        'Location': '',
        # 'Additional location information': '',
        'Host': '',
        # 'Patient age': pd.NA,
        # 'Gender': '',
        'Clade': '',
        'Variant': '',
        'AA Substitutions': '()',
        # 'Is reference?': False,
        'Is complete?': False,
        # 'Is high coverage?': False,
        'Is low coverage?': False,
        # 'N-Content': 0.0,
        # 'GC-Content': 0.0,
    }


class CleanedMetadataImportConfig:
    import_args = dict(
        usecols = [
            'virus_name',
            # 'type',
            'accession_id',
            'collection_date',
            'location',
            # 'additional_location_information',
            # 'sequence_length',
            # 'host',
            # 'patient_age',
            # 'gender',
            'clade',
            'variant',
            'aa_substitutions',
            'submission_date',
            # 'is_reference',
            # 'is_complete',
            # 'is_high_coverage',
            # 'is_low_coverage',
            # 'n_content',
            # 'gc_content'
        ],
        dtype = {
            'virus_name': 'string',
            # 'type': 'string',
            'accession_id': 'string',
            'location': 'string',
            # 'additional_location_information': 'string',
            # 'sequence_length': 'Int32',
            # 'host': 'string',
            # 'patient_age': 'string',
            # 'gender': 'string',
            'clade': 'string',
            'variant': 'string',
            'aa_substitutions': 'string',
            # 'is_reference': 'boolean',
            # 'is_complete': 'boolean',
            # 'is_high_coverage': 'boolean',
            # 'is_low_coverage': 'boolean',
            # 'n_content': 'float32',
            # 'gc_content': 'float32',
        },
        na_values = {
            'patient_age': 'unknown',
            'gender': 'unknown'
        },
        parse_dates = [
            'collection_date',
            'submission_date'
        ]
    )

    nafills = {
        'location': '',
        # 'additional_location_information': '',
        # 'host': '',
        # 'patient_age age': pd.NA,
        # 'gender': '',
        'clade': '',
        'variant': '',
        'aa_substitutions': '()',
        # 'is_reference': False,
        # 'is_complete': False,
        # 'is_high_coverage': False,
        # 'is_low_coverage': False,
        # 'n_content': 0.0,
        # 'gc_content': 0.0,
    }

class Plotting():
    """
        For matplotlib plots
    """

    @staticmethod
    def color_fader(c1, c2, mix=0):
        #fade (linear interpolate) from color c1 (at mix=0) to c2 (mix=1)
        c1=np.array(mpl.colors.to_rgb(c1))
        c2=np.array(mpl.colors.to_rgb(c2))
        c_hex = mpl.colors.to_hex((1-mix)*c1 + mix*c2)

        return c_hex

    @staticmethod
    def set_size(fig, h, cols=1, rows=1):
        """ w, h: width, height in inches """

        w = h

        l = fig.subplotpars.left
        r = fig.subplotpars.right
        t = fig.subplotpars.top
        b = fig.subplotpars.bottom

        wspace = fig.subplotpars.wspace
        hspace = fig.subplotpars.hspace

        figw = float(w * cols)/((r-l)) + ((cols-1) * wspace)
        figh = float(h * rows)/((t-b)) + ((cols-1) * hspace)

        fig.set_size_inches(figw, figh)

    @staticmethod
    def get_ax_size(fig, ax):
        bbox = ax.get_window_extent().transformed(fig.dpi_scale_trans.inverted())
        width, height = bbox.width, bbox.height
        width *= fig.dpi
        height *= fig.dpi
        return width, height