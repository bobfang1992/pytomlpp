import pytomlpp
import toml
import tomlkit
import timeit

def benchmark(name, func, number=5000):
    print(f'{name:>10}: Running...', end='', flush=True)
    res = str(timeit.timeit(func, number=number)).split('.')
    print('\b'*10 + f'{res[0]:>3}.{res[1]} s')

test_data = ''
with open('data.toml', 'r', encoding='utf-8') as f:
    test_data = f.read()

benchmark('pytomlpp', lambda: pytomlpp.loads(test_data))
benchmark('toml', lambda: toml.loads(test_data))
benchmark('tomlkit', lambda: tomlkit.parse(test_data))
