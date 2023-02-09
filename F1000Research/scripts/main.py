#!/usr/bin/env python3

import argparse
import traceback
import pathlib

import covmut_seasonal.counts as count_analytics
import covmut_seasonal.load as load
import covmut_seasonal.extract as extract
import covmut_seasonal.transform as transform
import covmut_seasonal.utils as utils
import covmut_seasonal.visualizations as visualizations

CWD = pathlib.Path.cwd()

parser = argparse.ArgumentParser(
    prog='covmut',
    description=(
        "Conduct analysis of regional-seasonal "
        "amino acid mutations in the SARS-CoV-2 genome."
    )
)

subparsers = parser.add_subparsers(dest='subparser')

"""
Extract
"""

extract_parser = subparsers.add_parser(
    'extract',
    help='Extraction functionality.'
)
extract_subparser = extract_parser.add_subparsers(dest="extract_subparser")

#
# Raw
#

extract_raw_subparser = extract_subparser.add_parser(
    'metadata',
    help="Extract metadata from compressed archive."
)

extract_raw_subparser.add_argument(
    '-f', '--force',
    action='store_true',
    help=("If set, overwrite the existing file in destination. "
            "Otherwise, an error will be thrown if the file exists."),
    dest="force"
)

##
# Sequence / AA Sub Feature map
##

extract_features_subparser = extract_subparser.add_parser(
    'features',
    help="Create sequence / AA Substitution feature map in binary 'hot' format."
)

extract_features_subparser.add_argument(
    '-t',
    type=int,
    choices=[0,1,2],
    default=1,
    help=(
        "Temporal resolution: 0 = 'year', 1 = 'quarter', 2 = 'month'. "
        "(Default: %(default)s [quarter])"
    ),
    dest="temporal"
)


"""
Transform
"""

transformation_parser = subparsers.add_parser('transform', help='Transformation functionalities.')
transformation_subparser = transformation_parser.add_subparsers(dest='trans_subparser')

#
# Country Latitudes and Climate Zones
#
geoclimate_transform_subparser = transformation_subparser.add_parser(
    'geoclimate', help="Get climate zone of latitudes."
)

#
# Raw-to-Interim
#
raw_to_interim_transform_subparser = transformation_subparser.add_parser(
    'metadata', help="Format raw metadata into intermediate datasets."
)

raw_to_interim_transform_subparser.add_argument(
    '-f', '--force',
    action='store_true',
    help=("If set, overwrite the existing file in destination without asking. "
            "Otherwise, a prompt will be shown if the file exists."),
    dest="force"
)


"""
Counts
"""

count_parser = subparsers.add_parser('count', help='Data Instance Counts.')
count_subparser = count_parser.add_subparsers(dest='count_subparser')

#
# AA Substitutions
#

count_substitutions_subparser = count_subparser.add_parser(
    'substitutions', help="Extract substitution names and counts."
)

#
# Geographic Counts
#

count_sequences_subparser = count_subparser.add_parser(
    'sequences', help="Count sequences binned by geographic and temporal periods with different granularities/resolutions."
)

count_sequences_subparser.add_argument(
    '-g',
    type=int,
    choices=[0,1,2,3],
    default=3,
    help="Geographic resolution: 0 = 'continent', 1 = 'country', 2 = 'region', 3 = 'zone'. (Default: %(default)s [zone])",
    dest="geographic"
)

count_sequences_subparser.add_argument(
    '-t',
    type=int,
    choices=[0,1,2],
    default=1,
    help=(
        "Temporal resolution: 0 = 'year', 1 = 'quarter', 2 = 'month'. "
        "(Default: %(default)s [quarter])"
    ),
    dest="temporal"
)

count_sequences_subparser.add_argument(
    '-z',
    type=int,
    choices=[0,1,2],
    default=0,
    help=(
        "Climate Zone type: 0 = 'simple', 1 = 'real', 2 = 'subtropic'. "
        "This argument is ignored if geographic resolution (`-g`) is not '3' [zone]. "
        "(Default: %(default)s [simple])"
    ),
    dest="zone"
)

#
# Substitution Count by Geography and Time
#

count_geotemporal_subparser = count_subparser.add_parser(
    'geotemporal', help="Count substitutions binned by geographic and temporal periods with different granularities/resolutions."
)

count_geotemporal_subparser.add_argument(
    '-g',
    type=int,
    choices=[0,1,2,3],
    default=3,
    help="Geographic resolution: 0 = 'continent', 1 = 'country', 2 = 'region', 3 = 'zone'. (Default: %(default)s [zone])",
    dest="geographic"
)

count_geotemporal_subparser.add_argument(
    '-t',
    type=int,
    choices=[0,1,2],
    default=1,
    help=(
        "Temporal resolution: 0 = 'year', 1 = 'quarter', 2 = 'month'. "
        "(Default: %(default)s [quarter])"
    ),
    dest="temporal"
)

count_geotemporal_subparser.add_argument(
    '-z',
    type=int,
    choices=[0,1,2],
    default=0,
    help=(
        "Climate Zone type: 0 = 'simple', 1 = 'real', 2 = 'subtropic'. "
        "This argument is ignored if geographic resolution (`-g`) is not '3' [zone]. "
        "(Default: %(default)s [simple])"
    ),
    dest="zone"
)

"""
Visualization
"""
visualization_parser = subparsers.add_parser('visualize', help='Visualization functionalities.')


visualization_parser.add_argument(
    '-t',
    type=int,
    choices=[0,1,2],
    default=1,
    help=(
        "Temporal resolution: 0 = 'year', 1 = 'quarter', 2 = 'month'. "
        "(Default: %(default)s [quarter])"
    ),
    dest="temporal"
)

visualization_parser.add_argument(
    '-z',
    type=int,
    choices=[0,1,2],
    default=0,
    help=(
        "Climate Zone type: 0 = 'simple', 1 = 'real', 2 = 'subtropic'. "
        "(Default: %(default)s [simple])"
    ),
    dest="zone"
)



"""
Command Interpreters
"""

def _extract_cli(_args):
    if _args.extract_subparser == 'metadata':
        extract.untar(force_overwrite=_args.force)
    elif _args.extract_subparser == 'features':
        extract.aa_substitution_feature_map(temporal_resolution=_args.temporal)

def _transform_cli(_args):
    if _args.trans_subparser == 'geoclimate':
        transform.country_geo_climate()
    elif _args.trans_subparser == 'metadata':
        md_generator = load.sequence_metadata()
        transform.raw_to_interim(md_generator, force_overwrite=_args.force)

def _counts_cli(_args):
    if _args.count_subparser == 'substitutions':
        count_analytics.unique_substitutions()
    elif _args.count_subparser == 'sequences':
        count_analytics.sequences_per_geotemporal_bin(
            geographic_resolution=_args.geographic,
            temporal_resolution=_args.temporal,
            climate_zone=_args.zone
        )
    elif _args.count_subparser == 'geotemporal':
        count_analytics.substitutions_per_geotemporal_bin(
            geographic_resolution=_args.geographic,
            temporal_resolution=_args.temporal,
            climate_zone=_args.zone
        )

def _visualization_cli(_args):
    visualizations.plot_per_substitution(_args.temporal, _args.zone)

if __name__ == "__main__":

    args = parser.parse_args()

    try:
        if args.subparser == 'extract':
            _extract_cli(args)
        elif args.subparser == 'transform':
            _transform_cli(args)
        elif args.subparser == 'count':
            _counts_cli(args)
        elif args.subparser == 'visualize':
            _visualization_cli(args)
        else:
            parser.print_help()
    except Exception as e:
        traceback.print_exc()
        raise SystemExit(utils.TermColors.color_fg('red', f'ERROR: {e}', bold=True))