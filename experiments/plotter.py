import matplotlib.pyplot as plt
import pandas as pd


filepath = 'C:\\Users\\Илья\\source\\repos\\deeplom\\deeplom\\experiments\\ex3\\ex4.csv'

df = pd.read_csv(filepath, sep=';', decimal=',', names=['n', 'm', 'q', 'r', 'time1', 'time2'],
                 dtype={'n': int, 'm': int, 'q': int, 'r': int, 'time1': float, 'time2': float})

fig, ax = plt.subplots(figsize=(10, 6), dpi=100)

ax.plot(df['m'], df['time1'], color='blue', label='2-куча')
ax.plot(df['m'], df['time2'], color='red', label='биномиальная куча')

ax.set_title('Время работы алгоритма Дейкстры', fontsize=15)
ax.set_xlabel("Число рёбер m")
ax.set_ylabel("Время работы t, секунды")

ax.ticklabel_format(style='sci', useMathText=True)
ax.locator_params(axis='x', min_n_ticks=10)
ax.locator_params(axis='y', min_n_ticks=10)

ax.legend(loc='lower right')
plt.show()


fig.savefig('C:\\Users\\Илья\\source\\repos\\deeplom\\deeplom\\experiments\\ex3\\plot_4.png', dpi=300)
