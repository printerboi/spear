import csv
import math
import os
import json
import sys
import matplotlib.pyplot as plt
import numpy as np

stepvalue = 1
offsetvalue = 0

stats = {"mean": {}, "variant": {}, "stddeviation": {}}

def calcStats(dataset):
    groupvals = {}

    for gro in dataset.keys():
        stats["mean"][gro] = 0.0
        stats["variant"][gro] = 0.0
        stats["stddeviation"][gro] = 0.0
        groupvals[gro] = []

    # Calc mean
    for key in dataset.keys():
        for it in range(offsetvalue, len(dataset[key]), 1):
            groupvals[key].append(dataset[key][it])


    for gro in dataset.keys():
        stats["mean"][gro] = np.mean(groupvals[gro])
        stats["variant"][gro] = np.var(groupvals[gro])
        stats["stddeviation"][gro] = np.std(groupvals[gro])


def plot(dataset):
    tpllist = {}
    singlegraphs = ["duration", "call", "division", "memory", "other", "programflow"]
    allgraphs = ["call", "division", "memory", "other", "programflow"]

    for gro in singlegraphs:
        tpllist[gro] = []

    for key in singlegraphs:
        for it in range(offsetvalue, len(dataset[key]), 1):
            tpllist[key].append((it*10, dataset[key][it]))


    for gro in singlegraphs:
        xs = []
        ys = []
        for tpl in tpllist[gro]:
            xs.append(tpl[0])
            ys.append(tpl[1])

        plt.clf()

        if gro != "duration":
            plt.axhline(y=stats["mean"][gro], color='r', linestyle='-')
            plt.axhline(y=stats["mean"][gro] + stats["stddeviation"][gro], color='g', linestyle='-')
            plt.axhline(y=stats["mean"][gro] - stats["stddeviation"][gro], color='g', linestyle='-')

            plt.plot(xs, ys, label="Gruppe " + gro)
            plt.xlabel('Iterationen')
            plt.ylabel('Energie in J')
            plt.title("Entwicklung der Energiewerte bei Änderung der Messwiederholungen")
        else:
            plt.plot(xs, ys, label="Laufzeit")
            plt.xlabel('Iterationen')
            plt.ylabel('Zeit in s')
            plt.title("Entwicklung der Laufzeit abhängig von den Messwiederholungen")

        plt.legend()
        plt.savefig("stability/plotted/plot_{}.png".format(gro))

    plt.clf()
    for gro in allgraphs:
        xs = []
        ys = []
        for tpl in tpllist[gro]:
            xs.append(tpl[0])
            ys.append(tpl[1])

        plt.plot(xs, ys, label="Gruppe " + gro)

    plt.xlabel('Iterationen')
    plt.ylabel('Energie in J')
    plt.title("Entwicklung der Energiewerte bei Änderung der Messwiederholungen")
    plt.legend()
    plt.savefig("stability/plotted/plot_gesamt.png")

def createDataset(data):
    dataset = {}
    for key in data[0].keys():
        dataset[key] = []

    for row in data:
        for key in row.keys():
            dataset[key].append(float(row[key]))

    return dataset

def readCSV(datapath):
    data = []
    with open(datapath, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if row['iterations'] != "stddeviation" and row['iterations'] != "mean" and row['iterations'] != "variant":
                data.append(row)

    return data

def main(datapath, offset, step):
    global stepvalue
    global offsetvalue

    data = readCSV(datapath)
    dataset = createDataset(data)
    stepvalue = int(step)
    offsetvalue = int(int(offset)/stepvalue)

    calcStats(dataset)

    abspahts = {"savedir": os.path.abspath("./stability/plotted")}

    isExist = os.path.exists(abspahts["savedir"])
    if not isExist:
        os.makedirs(abspahts["savedir"])

    plot(dataset)


if __name__ == "__main__":
    if len(sys.argv) == 4:
        main(sys.argv[1], sys.argv[2], sys.argv[3])
    else:
        print("Please provide a path to a a stability result .csv")
