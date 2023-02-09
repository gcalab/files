"""
Cross-project functions, classes, and non-"settings" variables.

"""
import functools
import re
import timeit

from collections import namedtuple
from pprint import PrettyPrinter

##
# exported helpers
##
import tqdm as _tqdm

pprint = PrettyPrinter().pprint
tqdm = _tqdm.tqdm

from covmut_seasonal.settings import (
    temporal_resolutions, geographic_resolutions, climate_zone_types,
    climate_zone_names, lat_degrees
)

AA_SUB_NAME_GRP_REGEX = re.compile(r"((?P<gene_name>.+)_)?(?P<aa_ref>[A-z]{,3})(?P<position>[0-9]+)(?P<aa_sub>[A-z]+)$")

def log_runtime(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        t = timeit.Timer(lambda: func(*args, **kwargs))
        total_time, result = t.timeit(number=1)
        msg = f'Function `{func.__name__}` completed in {total_time:0.4f} seconds.'

        print(msg)

        return result
    return wrapper


def convert_temporal_resolution_to_string(temporal_resolution):

    # immediately return if `temporal_resolution` is a string and is in the list
    if temporal_resolution in temporal_resolutions:
        return temporal_resolution

    # Attempt to cast to integer
    try:
        temporal_resolution = int(temporal_resolution)
        return temporal_resolutions[temporal_resolution]
    except (ValueError, IndexError):
        # not an integer
        msg = (f'Period index must be a string '
               f'({", ".join(temporal_resolutions)}) '
               f'or integer ({", ".join(range(len(temporal_resolutions)))}). '
               f'Received "{temporal_resolution}".')
        raise ValueError(msg)


def convert_geographic_resolution_to_string(geographic_resolution):
        # immediately return if `geographic_index` is a string and is in the list
    if geographic_resolution in geographic_resolutions:
        return geographic_resolution

    # Attempt to cast to integer
    try:
        geographic_resolution = int(geographic_resolution)
        return geographic_resolutions[geographic_resolution]
    except (ValueError, IndexError):
        # not an integer
        msg = (f'Geographic Resolution must be a string '
               f'({", ".join(geographic_resolutions)}) '
               f'or integer ({", ".join(range(len(geographic_resolutions)))}). '
               f'Received "{geographic_resolution}".')
        raise ValueError(msg)


def convert_climate_zone_type_to_string(climate_zone_type):
    # immediately return if `geographic_index` is a string and is in the list
    if climate_zone_type in climate_zone_types:
        return climate_zone_type

    # Attempt to cast to integer
    try:
        climate_zone_type = int(climate_zone_type)
        return climate_zone_types[climate_zone_type]
    except (ValueError, IndexError):
        # not an integer
        msg = (f'Geographic Resolution must be a string '
               f'({", ".join(climate_zone_types)}) '
               f'or integer ({", ".join(range(len(climate_zone_types)))}). '
               f'Received "{climate_zone_type}".')
        raise ValueError(msg)


def get_climate_zone_full_names(climate_zone_type=0):

    name_dict = {
        i: f'{n} {lat_degrees.get(climate_zone_types[climate_zone_type]).get(i)}'
        for i, n in climate_zone_names.items()
    }

    names = list(zip(*sorted(name_dict.items(), key=lambda x: x[0], reverse=True)))[1]
    return [name.title() for name in names]

    # for climate_zone_name in (climate_zone_names):



def split_line(line, sep=','):
    """
        Format a single character-separated string into a list.

        Strips whitespace from both line and result

        Parameters
        ----------
        header : str
            A string of fields, delimited by `sep`.

        sep : str
            A character to split `header`.

        Returns
        -------
        list
            A list from the string.


    """
    return [item.strip() for item in line.strip().split(sep)]


def print_nicer_pandas_desc(data, title=None, xlabel='', ylabel=''):
    """
        Provides a more informative data description than Pandas' default `describe` function.

        Parameters
        ----------
        data : pandas.Series(dtype=numeric)
            An array of numeric datapoints.

        title : string
            A string to use as a heading for display.

    """

    if not (hasattr(data, 'describe') and hasattr(data, 'quantile')):
        print(f"Parameter `data` is {type(data)}. It must be a Pandas Series.")

    desc = data.describe()

    value_width = len(str(desc.max()))

    desc_fmts = {
        'min': ('Min', int, 'd'),
        'max': ('Max', int, 'd'),
        'mean': ('Mean', float, '.03f'),
        'std': ('SD', float, '.03f')
    }

    percentiles = data.quantile(
        q=[0.25, 0.5, 0.75, 0.8, 0.9, 0.99, 0.999, 0.9999]
    )

    display_name_offset = len('Percentiles')

    if title:
        print(f'{title:s}')
        print('-' * len(title))

    print(f'{"Count":>{display_name_offset}}: {str(desc["count"]):>{value_width}} {xlabel}(s)')

    for key, (display_name, dtype, fmt) in desc_fmts.items():
        val = f'{desc[key].astype(dtype):{fmt}}'
        print(f'{display_name:>{display_name_offset}}: {val:>{value_width}} {ylabel}(s)')

    print(f'\nPercentiles: {ylabel}(s) | {xlabel}(s)')

    x_col_width = max(len(str(xlabel) + '(s)'), len(str(desc["max"])))
    y_col_width = max(len(str(ylabel) + '(s)'), len(str(desc["count"])))

    for key, val in percentiles.items():
        key = f'{key:0.2%}'
        y_col = f'{val:.03f}'
        x_col = f'{len(data[data > val])}'
        print(f'{key:>{display_name_offset}}: {y_col:<{y_col_width}} | {x_col:<{x_col_width}} ')
    print()


class _TermColors:
    """
        Terminal Colors: 3-bit and 4-bit.
    """

    # https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

    _weights = ['BOLD', 'DIM', 'NORMAL']
    _colors = ['BLACK', 'RED', 'GREEN', 'YELLOW', 'BLUE', 'MAGENTA', 'CYAN', 'WHITE']

    _weight_tuple = namedtuple('Colors', _weights)
    _color_tuple = namedtuple('Colors', _colors)

    _reset = '\033[0m'
    _weight = _weight_tuple(BOLD='\033[01m', DIM='\033[02m', NORMAL='\033[22m')
    _fg = _color_tuple(**dict(zip(_colors, [f'\033[{c:02d}m' for c in range(30, 30 + len(_colors))])))
    _bg = _color_tuple(**dict(zip(_colors, [f'\033[{c:02d}m' for c in range(40, 40 + len(_colors))])))

    @classmethod
    @property
    def reset(cls):
        return cls._reset

    @classmethod
    @property
    def weight(cls):
        return cls._weight

    @classmethod
    @property
    def fg(cls):
        return cls._fg

    @classmethod
    @property
    def bg(cls):
        return cls._bg

    @classmethod
    def color_fg(cls, name, s, bold=False, dim=False):
        c = cls.fg._asdict().get(name.upper(), cls.reset)
        w = cls.weight._asdict().get('BOLD' if bold else 'DIM' if dim else None, '')
        return f'{c}{w}{s:s}{cls.reset}'

    @classmethod
    def color_bg(cls, name, s):
        c = cls.bg._asdict().get(name.upper(), cls.reset)
        return f'{c}{s:s}{cls.reset}'


TermColors = _TermColors()

overwrite_warning_msg = TermColors.color_fg('yellow', 'This will overwrite existing files. Proceed [y|N]? ')


month_names = [
    'Jan',
    'Feb',
    'Mar',
    'Apr',
    'May',
    'Jun',
    'Jul',
    'Aug',
    'Sep',
    'Oct',
    'Nov',
    'Dec'
]

year_names = ['19', '20', '21', '22']

if __name__ == "__main__":
    get_climate_zone_full_names()