import csv
import datetime
import math
import os
import json
import struct
import sys
import subprocess
from timeit import default_timer as timer
from pathlib import Path

# libpath = "../../cmake-build-debug/src/main/passes/energy/Energy.so"
# modelpath = "../../cmake-build-debug/profile.json"
core = 1


# iterations = 100


def runprogram(spear, file, iterations):
    process = subprocess.Popen([spear, "-t", str(iterations), file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()

    return float(stdout)


def execute_analysis(file, strategy, loopbound, libpath, modelpath, iterations):
    total_diff = 0

    for i in range(iterations):
        command = 'opt -disable-output ' \
              '-load-pass-plugin {0} ' \
              '--passes="function(mem2reg,loop-rotate),energy" ' \
              '--profile {1} ' \
              '--mode program ' \
              '--format json ' \
              '--strategy {2} ' \
              '--loopbound {3} {4}'.format(libpath, modelpath, strategy, loopbound, file)

        outputstream = os.popen(command)
        output = outputstream.read()
        energy = json.loads(output)

        total_diff = energy["duration"]

    return total_diff/iterations


def write_analysis_result_to_csv(result):
    toround = 9

    with open('durationanalysis.csv', 'w') as f:
        w = csv.writer(f)
        w.writerow(["File", "Analyse", "Ausführung"])
        for key in result.keys():
            entry = result[key]

            w.writerow([entry["name"], round(entry["analysis"], toround), round(entry["execution"], toround)])


def main(builddir, bound, iterations, analysispath):
    simpledirpath = analysispath
    simpledir = os.listdir(simpledirpath)
    stategies = ["worst", "best", "average"]

    analysis_dict = {}

    for file in simpledir:
        if file.endswith(".ll"):
            filename = Path(file).stem

            relpath = "{0}/{1}".format(simpledirpath, file)
            print("Running time analysis for file {} [{}]".format(filename, relpath))
            analysis_dict[file] = {}

            analysis_duration = execute_analysis(relpath, "worst", int(bound),
                                                 "{}/src/main/passes/energy/Energy.so".format(builddir),
                                                 "{}/profile.json".format(builddir), int(iterations))
            analysis_dict[file]["analysis"] = analysis_duration

            analysis_dict[file]["name"] = file

            execution_duration = runprogram("{}/spear".format(builddir), "{}/{}".format(simpledirpath, filename),
                                            int(iterations))
            analysis_dict[file]["execution"] = execution_duration

    write_analysis_result_to_csv(analysis_dict)

    # for i in range(1, 1000):
    #     engergy_before = readRapl()
    #     sleep(10)
    #     energy_after = readRapl()
    #
    #     print(energy_after - engergy_before)


if __name__ == "__main__":
    if len(sys.argv) == 5:
        main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        print("Please provide a path to a folder containing .ll files")
