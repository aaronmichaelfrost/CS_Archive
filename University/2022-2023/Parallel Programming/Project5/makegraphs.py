import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import matplotlib.ticker as ticker

# Read the csv file
df = pd.read_csv('Project5Data.csv')

# Function to format x-axis
def format_func(value, tick_number):
    return f'{value:.0f}'

# Graph of performance vs. NUMTRIALS with multiple curves of BLOCKSIZE
fig, ax = plt.subplots(figsize=(10,6))
sns.lineplot(data=df, x='Trials', y='MegatrialsPerSecond', hue='Blocksize', palette='tab10', ax=ax)
ax.xaxis.set_major_formatter(ticker.FuncFormatter(format_func))
plt.title('Performance vs. NUMTRIALS with different BLOCKSIZE')
plt.xlabel('NUMTRIALS')
plt.ylabel('Performance')
plt.legend(title='BLOCKSIZE')
plt.grid(True)
plt.show()

# Graph of performance vs. BLOCKSIZE with multiple curves of NUMTRIALS
fig, ax = plt.subplots(figsize=(10,6))
sns.lineplot(data=df, x='Blocksize', y='MegatrialsPerSecond', hue='Trials', palette='tab10', ax=ax)
plt.title('Performance vs. BLOCKSIZE with different NUMTRIALS')
plt.xlabel('BLOCKSIZE')
plt.ylabel('Performance')
plt.legend(title='NUMTRIALS')
plt.grid(True)
plt.show()

# Correct Probability plot
fig, ax = plt.subplots(figsize=(10,6))
sns.lineplot(data=df, x='Trials', y='Probability', hue='Blocksize', palette='tab10', ax=ax)
ax.xaxis.set_major_formatter(ticker.FuncFormatter(format_func))
plt.title('Correct Probability vs. NUMTRIALS with different BLOCKSIZE')
plt.xlabel('NUMTRIALS')
plt.ylabel('Correct Probability')
plt.legend(title='BLOCKSIZE')
plt.grid(True)
plt.show()

# Monte Carlo performance table
monte_carlo_performance = df.groupby('Blocksize')['MegatrialsPerSecond'].mean().reset_index()
print(monte_carlo_performance)