import csv
import math
import os
import json
import sys
import matplotlib.pyplot as plt
import numpy as np

result = {}
stats = {"mean": {}, "variant": {}, "stddeviation": {}}
group = ["duration", "call", "division", "memory", "other", "programflow"]


def plot():
    tpllist = {}
    singlegraphs = ["duration", "call", "division", "memory", "other", "programflow"]
    allgraphs = ["call", "division", "memory", "other", "programflow"]

    for gro in singlegraphs:
        tpllist[gro] = []

    for key in result.keys():
        for gro in singlegraphs:
            tpllist[gro].append((key, result[key][gro]))

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
        plt.savefig("stability/plot_{}.png".format(gro))

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
    plt.savefig("stability/plot_gesamt.png")


def addToResult(iterations, data):
    result[iterations] = {}
    start = int(data["startOfExecution"])
    end = int(data["endOfExecution"])
    result[iterations]["duration"] = (end - start) / 1000000000.0
    result[iterations]["call"] = data["profile"]["Call"]
    result[iterations]["division"] = data["profile"]["Division"]
    result[iterations]["memory"] = data["profile"]["Memory"]
    result[iterations]["other"] = data["profile"]["Other"]
    result[iterations]["programflow"] = data["profile"]["Programflow"]


def write_result_to_file():
    with open('./stability/stability_result.csv', 'w') as f:
        size = len(result.keys())
        groupvals = {}

        for gro in group:
            stats["mean"][gro] = 0.0
            stats["variant"][gro] = 0.0
            stats["stddeviation"][gro] = 0.0
            groupvals[gro] = []

        # Calc mean
        for key in result.keys():
            for gro in group:
                groupvals[gro].append(result[key][gro])

        for gro in group:
            stats["mean"][gro] = np.mean(groupvals[gro])
            stats["variant"][gro] = np.var(groupvals[gro])
            stats["stddeviation"][gro] = np.std(groupvals[gro])

        w = csv.writer(f)
        w.writerow(
            ["iterations", "duration", "call", "division", "memory", "other", "programflow"])
        for key in result.keys():
            entry = result[key]
            w.writerow([key, entry["duration"], entry["call"], entry["division"], entry["memory"], entry["other"],
                        entry["programflow"]])

        w.writerow(["mean", *list(stats["mean"].values())])
        w.writerow(["variant", *list(stats["variant"].values())])
        w.writerow(["stddeviation", *list(stats["stddeviation"].values())])


def main(spearpath, profilepath):
    iterations = range(0, 5000, 10)

    for its in iterations:
        abspahts = {"profile": os.path.abspath(profilepath), "savedir": os.path.abspath("./stability")}

        isExist = os.path.exists(abspahts["savedir"])
        if not isExist:
            os.makedirs(abspahts["savedir"])

        command = "sudo {} -p {} {} {}".format(spearpath, its, abspahts["profile"], abspahts["savedir"])
        print(command)
        outputstream = os.popen(command)
        output = outputstream.read()
        print(output)
        with open("{}/profile.json".format(abspahts["savedir"]), 'r') as f:
            data = json.load(f)
            print(data)
            addToResult(its, data)

    print("Printing result to file...")
    write_result_to_file()
    plot()


if __name__ == "__main__":
    if len(sys.argv) == 3:
        main(sys.argv[1], sys.argv[2])
    else:
        print("Please provide a path to a folder containing .ll files")
