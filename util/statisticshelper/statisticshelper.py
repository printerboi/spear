"""
Little Python program to print out a graph for the benchmarked results.
Takes as first argument the relative path to the .csv file

"""

import csv
import sys
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams["figure.figsize"] = [10, 6]
entries = []


# Method converting all string values in a dict to floats
def conv_to_numeric(dictionary):
    for key in list(dictionary.keys()):
        dictionary[key] = float(dictionary[key])


# Removes the base value from the provided dict
def cleanData(dictionary):
    d = dict(dictionary)
    del d['Base']
    return d


if __name__ == "__main__":
    if len(sys.argv) == 2:
        # Read the provided csv file
        with open(Path(sys.argv[1]).resolve(), newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                # Append the read data to our entries array so we can process the data
                entries.append(row)

        # Iterate over the read entries. Each entry represents one data-series
        for entry in entries:
            # Convert the entry data to float values
            conv_to_numeric(entry)
            # Remove the base value from the entry
            cleaned = cleanData(entry)

            # Construct the dataframe
            plotdata = pd.DataFrame({"Energy µJ": list(cleaned.values())}, index=list(cleaned.keys()))
            # Calculate the standard deviation of the entry
            std = plotdata.std()
            std.name = "Standardabweichung"

            # Plot the data as bar-graph
            plotdata.plot(kind="bar")
            # Show the standard deviation as error bars
            plt.errorbar(list(cleaned.keys()), list(cleaned.values()), yerr=std)
            plt.show()
    else:
        print("Please provide only one argument. The argument should be a valid .csv file")
