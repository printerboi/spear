"""
Little Python program to print out a graph for the benchmarked results.
Takes as first argument the relative path to the .json file

"""
import csv
import json
import sys
from pathlib import Path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams["figure.figsize"] = [10, 6]
entries = []


# Method converting all string values in a dict to floats
def conv_to_numeric(dictionary):
    for key in list(dictionary.keys()):
        dictionary[key] = float(dictionary[key])


# Removes the base value from the provided dict
def cleanData(reader):
    obj = {}

    for row in reader:
        key = row["Datei"]
        obj[key] = {}

        obj[key]["Name"] = row["Datei"]
        obj[key]["Worstcase"] = row["Worstcase"]
        obj[key]["Bestcase"] = row["Bestcase"]
        obj[key]["Averagecase"] = row["Averagecase"]
        obj[key]["Measurement"] = row["Measurement"]

    return obj


if __name__ == "__main__":
    if len(sys.argv) == 2:
        # Opening JSON file
        with open(Path(sys.argv[1]).resolve()) as csvfile:
            reader = csv.DictReader(csvfile)

            # Convert the entry data to float values
            # Remove the base value from the entry
            cleaned = cleanData(reader)

            # Construct the dataframe
            plotdata = pd.DataFrame({"Energy µJ": list(cleaned.values())}, index=list(cleaned.keys()))

            index = list(cleaned.keys())
            values = {}
            values["worst"] = []
            values["best"] = []
            values["average"] = []
            values["measurement"] = []
            colors = []

            for vobj in list(cleaned.values()):

                values["worst"].append(float(vobj["Worstcase"]))
                values["best"].append(float(vobj["Bestcase"]))
                values["average"].append(float(vobj["Averagecase"]))
                values["measurement"].append(float(vobj["Measurement"]))


            ax = plt.axes()
            ax.scatter(index, values["worst"], label="Worstcase")
            ax.scatter(index, values["best"], label="Bestcase")
            ax.scatter(index, values["average"], label="Averagecase")
            ax.scatter(index, values["measurement"], label="Measurement")
            ax.set_title("Ergebnis der Analyse")
            ax.set_ylabel("Energie in µJ")
            ax.set_xlabel("Program")
            ax.legend()


            plt.show()


    else:
        print("Please provide only one argument. The argument should be a valid .csv file")
