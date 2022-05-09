import timeit
from pathlib import Path

_parsers = []

import pytomlpp
_parsers.append(('pytomlpp', pytomlpp.loads))

try:
    import rtoml
    _parsers.append(('rtoml', rtoml.loads))
except ImportError:
    print("Couldn't import 'rtoml'")

try:
    import tomli
    _parsers.append(('tomli', tomli.loads))
except ImportError:
    print("Couldn't import 'tomli'")

try:
    import toml
    _parsers.append(('toml', toml.loads))
except ImportError:
    print("Couldn't import 'toml'")

try:
    import qtoml
    _parsers.append(('qtoml', qtoml.loads))
except ImportError:
    print("Couldn't import 'qtoml'")

try:
    import tomlkit
    _parsers.append(('tomlkit', tomlkit.parse))
except ImportError:
    print("Couldn't import 'tomlkit'")

try:
    import tomllib # PEP 680
    _parsers.append(('tomllib', tomllib.loads))
except ImportError:
    # doesn't make sense to report this one since it will eventually be a part of the standard lib and can't be installed separately
    pass 

def run(run_count = 1000):
    test_data = ''
    with open(str(Path(Path(__file__).parent, 'data.toml').resolve()), 'r', encoding='utf-8') as f:
        test_data = f.read()

    print(f'Parsing data.toml {run_count} times:')
    durations = []
    for name, func in _parsers:
        # pylint: disable=cell-var-from-loop
        # pylint: disable=bare-except
        run_str = f'  Running {name}...'
        print(run_str, end='', flush=True)
        try:
            durations.append((name, timeit.timeit(lambda: func(test_data), number=run_count), True))
        except:
            durations.append((name, 9999999999999999999999999999999, False))
        print('\b'*len(run_str), end='')
        print(' '*len(run_str), end='')
        print('\b'*len(run_str), end='')

    durations.sort(key=lambda dur: dur[1])
    baseline = durations[0][1]
    for name, duration, ok in durations:
        if ok:
            res = str(duration).split('.')
            if len(res) == 1:
                res.append('000')
            print(f'{name:>10}: {res[0]:>3}.{res[1][:3]} s', end='')
            if duration != baseline:
                delta = str(duration / baseline).split('.')
                if len(delta) == 1:
                    delta.append('00')
                print(rf' ({delta[0]:>2}.{delta[1][:2]}x)', end='')
            print('')
        else:
            print(f'{name:>10}: Parsing failed. Likely not TOML 1.0.0-compliant.')


if __name__ == '__main__':
    run()
