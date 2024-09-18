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
    histos = []
    for i in range(10, 26):
        histos.append(f'Run0_h1_E{i}.csv')

    plt.figure(figsize=(10, 6))
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

        plt.step(x, df['entries'][1:-1], label=label, color=[0.9*i/len(histos)]*3)
    
    plt.legend()
    plt.ylabel('Entries')
    plt.xlabel('Energy [keV]')
    plt.title('Energy of incoming gammas')
    plt.tight_layout()
    plt.savefig('E.pdf', bbox_inches='tight')

    #read_ntuples(glob.glob('Run0_nt_Ntuple_t*.csv'))

    plt.show()


if __name__ == '__main__':
    main()
