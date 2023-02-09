"""
Plotting and graphing functions for figure creation.

"""

from collections import Counter, defaultdict
from math import ceil

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import pandas as pd

import covmut_seasonal.utils as utils
from covmut_seasonal.settings import PROC_DIR, REPORTS_DIR, climate_zone_names

def plot_per_substitution(temporal_resolution:int=1, climate_zone:int=0, plot_num_cols:int=4):
    """
        Create individual plots for each substitution, where the X-axis represents
        temporal periods and the Y-axis, incidence.

        Parameters
        ----------

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

        plot_num_cols : int
            The number of columns in the plot.
            If the number of substitutions is less than 4, then the plot will be
            truncated to that number.
    """


    temporal_resolution_str = utils.convert_temporal_resolution_to_string(temporal_resolution)
    climate_zone_str = utils.convert_climate_zone_type_to_string(climate_zone)
    climate_zone_str_full = f'zone_{climate_zone_str}'

    climate_zone_props = pd.read_csv(PROC_DIR / f'{temporal_resolution_str}_{climate_zone_str_full}_props.csv')
    climate_zone_props = climate_zone_props.rename(
        columns={
            climate_zone_str_full: 'Zone',
            temporal_resolution_str: temporal_resolution_str.capitalize()
        }
    )
    climate_zone_props['Zone'] = climate_zone_props['Zone'].apply(lambda x: climate_zone_names.get(x).title())

    zone_grps = climate_zone_props.set_index(temporal_resolution_str.capitalize()).groupby('Zone')

    dst_dirpath= REPORTS_DIR / 'figures' / temporal_resolution_str
    if not dst_dirpath.exists(): dst_dirpath.mkdir(parents=True, exist_ok=True)

    gene_aa_sub_names = climate_zone_props.columns[2:]

    climate_zone_names = utils.get_climate_zone_full_names(climate_zone)

    _gene_aa_sub_names = defaultdict(list)
    gene_aa_sub_counts = Counter()
    for gene_aa_sub in gene_aa_sub_names:
        # if (match := utils.AA_SUB_NAME_GRP_REGEX.match(gene_aa_sub)) is not None:
        gene_name, aa_sub_name = gene_aa_sub.split('_')
        _gene_aa_sub_names[gene_name].append(aa_sub_name)
        gene_aa_sub_counts[gene_name] += 1

    gene_aa_sub_names = _gene_aa_sub_names

    def sort_fn(key):
        # key = key[0]
        if (match := utils.AA_SUB_NAME_GRP_REGEX.match(key)) is not None:
            grp_dict = match.groupdict()
            pos = grp_dict.get("position")
            aa_ref = grp_dict.get("aa_ref")
            aa_sub = grp_dict.get("aa_sub")

            sort_str = f"{pos:0>5},{aa_ref:a<3},{aa_sub:a<3}"
            return sort_str
        else:
            raise(Exception('No way'))

    for gene_name, aa_sub_names in gene_aa_sub_names.copy().items():
        gene_aa_sub_names[gene_name] = sorted(aa_sub_names, key=sort_fn)

    # subcounts = {name: ceil(count/plot_num_cols) for name, count in subcounts.items()}

    figs = defaultdict(list)

    with utils.tqdm(total=len(gene_aa_sub)) as pbar:

        for gene_name, aa_sub_names in gene_aa_sub_names.items():

            plot_num_rows = ceil(gene_aa_sub_counts[gene_name] / plot_num_cols)

            print(f"Cols: {plot_num_cols}, Rows: {plot_num_rows}")
            _fig, _axs = plt.subplots(plot_num_rows, plot_num_cols, figsize=(20, 5 * plot_num_rows), tight_layout=True)
            _fig.suptitle(f'{temporal_resolution_str.capitalize()}ly Incidence of Amino Acid Substitutions in "{gene_name}"', y=0.9975, size=18, weight='bold')
            # _fig.supxlabel('Month')
            # _fig.supylabel('Incidence')
            figs[gene_name] = [_fig, _axs.flatten(), 0]

            for aa_sub_name in aa_sub_names:

                fig, axs, idx = figs[gene_name]
                ax = axs[idx]

                pbar.set_description(f"Plotting axes for `{gene_name} {aa_sub_name}` using '{climate_zone_str}' climate zones @ a '{temporal_resolution_str}ly' temporal resolution")

                gene_aa_sub_name = f'{gene_name}_{aa_sub_name}'

                zone_grps[gene_aa_sub_name].plot(ax=ax, marker='o', lw=0.5, ls='--')
                ax.set_ylabel('Incidence')

                ax.set_title(f'p.{aa_sub_name}')

                labels = [f'{m} \'{y}' for y in utils.year_names for m in utils.month_names][11:]

                ax.xaxis.set_major_locator(ticker.FixedLocator(np.arange(1, len(labels), 6)))
                ax.xaxis.set_major_formatter(ticker.FixedFormatter(labels[1::6]))
                ax.xaxis.set_minor_locator(ticker.FixedLocator(np.arange(len(labels))))

                ax.set_ylim(-0.05, 1.05)
                ax.grid(alpha=0.5)
                ax.grid(which='minor', axis='x', alpha=0.2)

                # reordering the labels
                handles, labels = ax.get_legend_handles_labels()
                # specify order
                order = [0, 1, 3, 2]
                # pass handle & labels lists along with order as below
                ax.legend([handles[i] for i in order], [labels[i] for i in order], title='Climate Zone')

                figs[gene_name][2] = idx + 1
                pbar.update()

                # fig.savefig(dst_dirpath / f'{aa_sub.lower()}.pdf')
                # clear plot from memory
                # fig.clear()
                # plt.close(fig)

            break
        pbar.set_description('Plotting axes complete')

    with utils.tqdm(total=len(figs)) as pbar:
        for gene_name, (fig, axs, _) in figs.items():
            pbar.set_description(f'Saving plot {gene_name} with {len(axs)} subs. Axes: ({ceil(gene_aa_sub_counts[gene_name] / plot_num_cols)} x {plot_num_cols}).')


            fig.savefig(dst_dirpath / f'{gene_name.lower()}.pdf')
            # clear plot from memory
            fig.clear()
            plt.close(fig)
            pbar.update()

    pbar.set_description('Saving plots complete')
