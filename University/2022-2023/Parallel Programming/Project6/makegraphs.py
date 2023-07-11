import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

# Read the data
df = pd.read_csv("output.csv")

# Prepare the figures
fig, ax = plt.subplots(2, 1, figsize=(12, 16))

# Function to format large numbers
def human_readable(val, _):
    return f'{val / 1e6:.0f}M' if val >= 1e6 else f'{val / 1e3:.0f}k' if val >= 1e3 else f'{val:.0f}'

# Set the formatter
formatter = ticker.FuncFormatter(human_readable)

# Plot matrix multiply performance versus total matrix size
for local_size in df['WORK_ELEMENTS'].unique():
    subset = df[df['WORK_ELEMENTS'] == local_size]
    ax[0].plot(subset['MATW'] * subset['MATW'], subset['GigaMultsPerSecond'], label=f'Local Size {local_size}')
ax[0].xaxis.set_major_formatter(formatter)
ax[0].set_xlabel('Total Matrix Size (MATW*MATW)')
ax[0].set_ylabel('Performance (GigaMultsPerSecond)')
ax[0].set_title('Matrix Multiply Performance vs Total Matrix Size')
ax[0].legend()

# Plot matrix multiply performance versus total Local Size
for mat_size in df['MATW'].unique():
    subset = df[df['MATW'] == mat_size]
    ax[1].plot(subset['WORK_ELEMENTS'] * subset['WORK_ELEMENTS'], subset['GigaMultsPerSecond'], label=f'Matrix Size {mat_size}')
ax[1].xaxis.set_major_formatter(formatter)
ax[1].set_xlabel('Total Local Size (LOCALGROUP*LOCALGROUP)')
ax[1].set_ylabel('Performance (GigaMultsPerSecond)')
ax[1].set_title('Matrix Multiply Performance vs Total Local Size')
ax[1].legend()

plt.tight_layout()
plt.show()