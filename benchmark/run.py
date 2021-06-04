import pytomlpp
import toml
import tomli
import tomlkit
import qtoml
import timeit

def benchmark(name, run_count, func, compare_to=None):
    print(f'{name:>10}: Running...', end='', flush=True)
    time_taken = timeit.timeit(func, number=run_count)
    res = str(time_taken).split('.')
    print('\b'*10, end='')
    print(f'{res[0]:>4}.{res[1][:3]} s', end='')
    if compare_to is not None:
        delta = time_taken / compare_to
        relation = 'slower'
        if delta < 1.0:
            delta = 1.0 / delta
            relation = 'faster'
        delta = int(delta * 10.0) / 10.0
        print(f' ({delta}x {relation})', end='')
    print('')
    return time_taken

def run(run_count = 5000):
    test_data = ''
    with open('data.toml', 'r', encoding='utf-8') as f:
        test_data = f.read()
    print(f'Parsing data.toml {run_count} times:')
    baseline = benchmark('pytomlpp', run_count, lambda: pytomlpp.loads(test_data))
    benchmark('tomli', run_count, lambda: tomli.loads(test_data), compare_to=baseline)
    benchmark('toml', run_count, lambda: toml.loads(test_data), compare_to=baseline)
    benchmark('qtoml', run_count, lambda: qtoml.loads(test_data), compare_to=baseline)
    benchmark('tomlkit', run_count, lambda: tomlkit.parse(test_data), compare_to=baseline)

if __name__ == '__main__':
    run()
