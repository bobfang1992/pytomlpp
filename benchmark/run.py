import pytomlpp
import toml
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
        num = time_taken
        denom = compare_to
        relation = 'slower'
        if num < denom:
            num = compare_to
            denom = compare_to
            relation = 'faster'
        delta = int((num / denom) * 10.0) / 10.0
        print(f' ({delta}x {relation})', end='')
    print('')
    return time_taken

def run(run_count = 5000):
    test_data = ''
    with open('data.toml', 'r', encoding='utf-8') as f:
        test_data = f.read()
    print(f'Parsing data.toml {run_count} times:')
    baseline = benchmark('pytomlpp', run_count, lambda: pytomlpp.loads(test_data))
    benchmark('toml', run_count, lambda: toml.loads(test_data), compare_to=baseline)
    benchmark('qtoml', run_count, lambda: qtoml.loads(test_data), compare_to=baseline)
    benchmark('tomlkit', run_count, lambda: tomlkit.parse(test_data), compare_to=baseline)

if __name__ == '__main__':
    run()
