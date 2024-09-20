import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import yaml
import glob
import mplhep as hep

hep.style.use('CMS')

def read_histo(file_name):
    #print('Reading file: ', file_name)
    with open(file_name, 'r') as f:
        l = 0
        for line in f.readlines():
            ll = line.replace('\n', '')
            if ll.startswith('#title'):
                title = ll.replace('#title', '')
            if ll.startswith('#axis'):
                _, _, bins, lower, upper = ll.split()
            if not line.startswith('#'):
                break
            l += 1

    df = pd.read_csv(file_name, skiprows=l)

    info = {
        'Bins': int(bins),
        'Lower_lim': float(lower),
        'Upper_lim': float(upper),
        'Underflow': df['entries'].iloc[0],
        'Overflow': df['entries'].iloc[-1],
        'Total_entries': df['entries'].sum()
    }

    return df, info, title


def read_ntuples(file_names):
    data_start = 0
    dfs = []
    for i, fn in enumerate(file_names):
        if i == 0:
            print('Reading file: ', fn)

            with open(fn, 'r') as f:
                cols = []
                for line in f.readlines():
                    ll = line.replace('\n', '')
                    if ll.startswith('#title'):
                        title = ll.replace('#title', '')
                    elif ll.startswith('#column'):
                        cols.append(ll.split()[-1])
                    if not line.startswith('#'):
                        break
                    data_start += 1

        dfs.append(pd.read_csv(fn, skiprows=data_start, names=cols))

    return pd.concat(dfs)

def main():
    good_detectors = list(range(10, 26))

    histos = []
    for i in good_detectors:
        histos.append(f'Run0_h1_E{i}.csv')

    plt.figure(figsize=(12, 6))
    for i,h in enumerate(histos):
        df, info, title = read_histo(h)

        if df['entries'].sum() == 0:
            continue

        info_str = '\n'.join([f'{k} {v}' for k, v in info.items()])

        x = np.linspace(info['Lower_lim'], info['Upper_lim'], info['Bins'])

        print(h, info['Total_entries'], 'entries')

        #plt.title(title)
        #plt.step(x, df['entries'][1:-1], label='Entries per bin')
        #plt.savefig(h.replace('.csv', '.pdf'))
        #plt.close()

        label = None
        if i == 0:
            label = 'Largest angle'
        elif i == len(histos)-1:
            label = 'Smallest angle'
        
        if 'E18.csv' in h:
            label = '90 deg'

        plt.step(x, df['entries'][1:-1], label=label, color='red' if 'E18.csv' in h else [0.9*i/len(histos)]*3)
    
    plt.legend()
    plt.ylabel('Entries')
    plt.xlabel('Energy [keV]')
    plt.title('Energy of incoming gammas')
    plt.tight_layout()
    plt.savefig('E.pdf', bbox_inches='tight')

    df = read_ntuples(glob.glob('Run0_nt_Ntuple_t*.csv'))

    plt.figure(figsize=(12, 6))
    for i,ind in enumerate(good_detectors):
        det = df[df['Detector'] == ind]

        label = None
        if i == 0:
            label = 'Largest angle'
        elif i == len(good_detectors)-1:
            label = 'Smallest angle'

        if ind == 18:
            label = '90 deg'
        
        color = [0.9*i/len(good_detectors)]*3 if ind != 18 else 'red'
        linewidth = 1 if ind != 18 else 2

        print(ind, len(det))

        plt.hist(det['Energy'], bins=200, range=(0, 200), histtype='step', label=label, color=color, linewidth=linewidth, zorder=10-i if ind != 18 else 100)

    plt.legend()
    plt.title('Energy of incoming gammas')
    plt.xlabel('Energy [keV]')
    plt.ylabel('Entries')
    plt.tight_layout()
    plt.savefig(f'E_ntuples.pdf', bbox_inches='tight')

    plt.show()


if __name__ == '__main__':
    main()
