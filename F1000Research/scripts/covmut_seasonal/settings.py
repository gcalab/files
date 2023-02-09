"""
Project-wide configuration variables, such as environment variables.

"""

import os
from pathlib import Path
from dotenv import load_dotenv

load_dotenv()

# Get the parent directory of this file's parent directory
PROJECT_DIR = Path(os.getenv('PROJECT_DIR', Path(__file__).parents[1])).absolute()

METADATA_VERSION = os.getenv('METADATA_VERSION', "unversioned")

METADATA_TAR_FNAME = f'metadata_tsv_{METADATA_VERSION}.tar.xz'
METADATA_FNAME = f'metadata_{METADATA_VERSION}.tsv'

DATA_DIR = Path(os.getenv('DATA_DIR', PROJECT_DIR / 'data')).absolute()
EXTERNAL_DIR = DATA_DIR / 'external'
RAW_DIR = DATA_DIR / 'raw'
INTER_DIR = DATA_DIR / 'interim' / METADATA_VERSION
PROC_DIR = DATA_DIR / 'processed' / METADATA_VERSION
REPORTS_DIR = PROJECT_DIR / 'reports' / METADATA_VERSION

EXTERNAL_DIR.mkdir(parents=True, exist_ok=True)
RAW_DIR.mkdir(parents=True, exist_ok=True)
INTER_DIR.mkdir(parents=True, exist_ok=True)
PROC_DIR.mkdir(parents=True, exist_ok=True)
REPORTS_DIR.mkdir(parents=True, exist_ok=True)

temporal_resolutions = ['year', 'quarter', 'month']
geographic_resolutions = ['continent', 'country', 'region', 'zone']
climate_zone_types = ['simple', 'real', 'subtropic']
climate_zone_names = {
    0: 'tropics',
    1: 'northern temperate',
    -1: 'southern temperate',
    2: 'arctic',
    -2: 'antarctic',
    0.5: 'northern subtropic',
    -0.5: 'southern subtropic',
}

lat_degrees = {
    'simple': {
        0: ('30° S', '30° N'),
        1: ('30° N', '60° N'),
        -1: ('30° S', '60° S'),
        2: ('60° N', '90° N'),
        -2: ('60° S', '90° S'),
        0.5: ('20° N', '30° N'),
        -0.5: ('20° S', '30° S'),
    },
    'real': {
        0: ('23.27° S', '23.27° N'),
        1: ('23.27° N', '66.33° N'),
        -1: ('23.27° S', '66.33° S'),
        2: ('66.33° N', '90° N'),
        -2: ('66.33° S', '90° S'),
        0.5: ('23.27° N', '35° N'),
        -0.5: ('23.27° S', '35° S'),
    },
    'subtropic': {
        0: ('23.27° S', '23.27° N'),
        1: ('35° N', '66.33° N'),
        -1: ('35° S', '66.33° S'),
        2: ('66.33° N', '90° N'),
        -2: ('66.33° S', '90° S'),
        0.5: ('23.27° N', '35° N'),
        -0.5: ('23.27° S', '35° S'),
    },
}