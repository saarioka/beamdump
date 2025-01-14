import sys
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
            if not line.startswith('#'):
                break
            ll = line.replace('\n', '')
            l += 1

    columns = ll.split(',')
    columns = [c.replace('#', '') for c in columns]
    columns = [c.replace(' ', '') for c in columns]
    columns = list(filter(lambda x: x != '', columns))
    print(columns)
    return pd.read_csv(file_name, skiprows=l, names=columns)


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


def main(files):
    for f in files:
        df = read_histo(f)
        plt.figure(figsize=(12, 9))

        df.iX -= 50
        df.iY -= 50
        
        plt.hist2d(df.iX, df.iY, weights=df.entry, bins=int(np.sqrt(len(df))), cmap='viridis', edgecolor='face')
        plt.colorbar(label='Entries')
        plt.title(f'{f.replace(".txt", "")} at 50 cm from the W disk\ntotal entries: {df.entry.sum()}')
        plt.xlabel('X (cm)')
        plt.xlabel('Y (cm)')

        plt.tight_layout()
        plt.savefig(f.replace('.txt', '.pdf'), bbox_inches='tight')

    plt.show()
        


if __name__ == '__main__':
    main(sys.argv[1:])
