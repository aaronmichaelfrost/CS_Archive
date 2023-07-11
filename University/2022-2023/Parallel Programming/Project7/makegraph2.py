import pandas as pd
import matplotlib.pyplot as plt
import numpy as np  # You need to import numpy

# Load data
data = pd.read_csv('plot2.csv')

# Create a scatter plot
plt.figure(figsize=[8,6])
plt.scatter(data['Num_Processors'], data['Performance'], color='blue')

# Add a line of best fit
m, b = np.polyfit(data['Num_Processors'], data['Performance'], 1)
plt.plot(data['Num_Processors'], m*data['Num_Processors'] + b, color='red')

# Labels and title
plt.title('Performance vs Number of Processors')
plt.xlabel('Number of Processors')
plt.ylabel('Performance (Mega Multiplies per second)')

# Display the plot
plt.show()

