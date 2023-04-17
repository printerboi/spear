import csv
import math
import os
import json
import sys
import matplotlib.pyplot as plt

result = {}
stats = {"mean": {}, "variant": {}, "stddeviation": {}}
group = ["duration", "call", "division", "memory", "other", "programflow"]


def plot():
    tpllist = {}
    valgroups = ["call", "division", "memory", "other", "programflow"]

    for gro in valgroups:
        tpllist[gro] = []

    for key in result.keys():
        for gro in valgroups:
            tpllist[gro].append((key, result[key][gro]))

    for gro in valgroups:
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
    plt.savefig("stability/plot.png")


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

        for gro in group:
            stats["mean"][gro] = 0.0
            stats["variant"][gro] = 0.0
            stats["stddeviation"][gro] = 0.0

        # Calc mean
        for key in result.keys():
            for gro in group:
                stats["mean"][gro] = stats["mean"][gro] + result[key][gro]

        for gro in group:
            stats["mean"][gro] = stats["mean"][gro] / size

        # Calc variant
        for key in result.keys():
            for gro in group:
                stats["variant"][gro] = stats["variant"][gro] + (result[key][gro] - stats["mean"][gro]) ** 2

        for gro in group:
            stats["stddeviation"][gro] = math.sqrt(stats["variant"][gro])

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
    iterations = range(0, 1000, 100)

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
