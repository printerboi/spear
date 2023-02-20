import csv
import os
import json
import struct
import sys
from time import sleep
import subprocess
from pathlib import Path

libpath = "../../cmake-build-debug/src/main/passes/energy/Energy.so"
modelpath = "../../cmake-build-debug/benchmarkresult.json"
core = 1
iterations = 100


def runprogram(file):
    ges_energy = 0
    beforeeng = readRapl()
    for iteration in range(0, iterations):
        os.sched_setaffinity(0, {core})
        process = subprocess.Popen([file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()

    aftereng = readRapl()

    ges_energy += aftereng - beforeeng

    return ges_energy / iterations


def readRapl():
    registerpath = "/dev/cpu/{}/msr".format(core)
    energyreg = 0x639
    unitreg = 0x606

    energy = 0
    unit = 0

    with open(registerpath, 'rb') as rf:
        rf.seek(energyreg)
        rawenergy = rf.read(8)
        energy = struct.unpack('L', rawenergy)
        cleaned_energy = energy[0]

        rf.seek(0)
        rf.seek(unitreg)
        rawenergy = rf.read(8)
        unit = struct.unpack('L', rawenergy)
        cleaned_unit = unit[0] & 0xf

    return cleaned_energy * pow(0.5, cleaned_unit)


def execute_analysis(file, strategy, loopbound):
    command = 'opt -disable-output ' \
              '-load-pass-plugin {0} ' \
              '--passes="function(instnamer,mem2reg,loop-simplify,loop-rotate),energy" ' \
              '--model {1} ' \
              '--mode program ' \
              '--format json ' \
              '--strategy {2} ' \
              '--loopbound {3} {4}'.format(libpath, modelpath, strategy, loopbound, file)

    outputstream = os.popen(command)
    output = outputstream.read()

    return json.loads(output)


def write_analysis_result_to_csv(result):
    with open('analysis_result.csv', 'w') as f:
        w = csv.writer(f)
        w.writerow(["Datei", "Worstcase", "Averagecase", "Bestcase", "Measurement"])
        for key in result.keys():
            entry = result[key]
            w.writerow([entry["name"], entry["worst"], entry["average"], entry["best"], entry["measurement"]])


def main(analysispath):
    simpledirpath = analysispath
    simpledir = os.listdir(simpledirpath)
    stategies = ["worst", "best", "average"]
    bound = "100"

    analysis_dict = {}

    for file in simpledir:
        if file.endswith(".ll"):
            filename = Path(file).stem

            relpath = "{0}/{1}".format(simpledirpath, file)
            print("Running analysis for file {}".format(filename))
            analysis_dict[file] = {}
            for strategy in stategies:
                # print("\t{}...".format(strategy))
                json_result = execute_analysis(relpath, strategy, bound)
                analysis_dict[file][strategy] = json_result["main"]["energy"]

            analysis_dict[file]["name"] = file

            measurement = runprogram("{}/{}".format(simpledirpath, filename))
            analysis_dict[file]["measurement"] = measurement

    write_analysis_result_to_csv(analysis_dict)

    # for i in range(1, 1000):
    #     engergy_before = readRapl()
    #     sleep(10)
    #     energy_after = readRapl()
    #
    #     print(energy_after - engergy_before)


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print("Please provide a path to a folder containing .ll files")