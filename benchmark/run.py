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


def run(run_count = 5000):
    test_data = ''
    with open(str(Path(Path(__file__).parent, 'data.toml').resolve()), 'r', encoding='utf-8') as f:
        test_data = f.read()

    print(f'Parsing data.toml {run_count} times:')
    durations = []
    for name, func in _parsers:
        # pylint: disable=cell-var-from-loop
        run_str = f'  Running {name}...'
        print(run_str, end='', flush=True)
        durations.append((name, timeit.timeit(lambda: func(test_data), number=run_count)))
        print('\b'*len(run_str), end='')
        print(' '*len(run_str), end='')
        print('\b'*len(run_str), end='')

    durations.sort(key=lambda dur: dur[1])
    baseline = durations[0][1]
    for name, duration in durations:
        res = str(duration).split('.')
        print(f'{name:>10}: {res[0]:>3}.{res[1][:3]} s', end='')
        if duration != baseline:
            delta = duration / baseline
            delta = int(delta * 100.0) / 100.0
            print(f' ({delta:>5}x)', end='')
        print('')


if __name__ == '__main__':
    run()
