import pandas as pd
import matplotlib.pyplot as plt

# Load the data
df = pd.read_csv('plot.csv')

# Convert 'sums' column to float
df['sums'] = pd.to_numeric(df['sums'], errors='coerce')

# Create a new figure
plt.figure(figsize=(10,6))

# Remove outlier
df = df[df['sums'] < 1e20]

# Plot the data
plt.plot(df['index'], df['sums'], 'o-')

# Add labels for x and y axis
plt.xlabel('Index')
plt.ylabel('Sums')

# Optional: add a title
plt.title('Plot of sums against index')

# Show the plot
plt.show()