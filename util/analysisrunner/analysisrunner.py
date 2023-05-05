import csv
import math
import os
import json
import struct
import sys
import subprocess
import time
from pathlib import Path

# libpath = "../../cmake-build-debug/src/main/passes/energy/Energy.so"
# modelpath = "../../cmake-build-debug/profile.json"
core = 1


# iterations = 100


def runprogram(file, iterations):
    ges_energy = 0

    for iteration in range(0, iterations):
        beforeeng = readRapl()
        process = subprocess.Popen([file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()

        aftereng = readRapl()

        ges_energy += aftereng - beforeeng
        #os.system("/bin/insmod /home/maximiliank/Dokumente/workbench/cache_invalidator/clearer/cacheclearer.ko")
        #os.system("/bin/rmmod /home/maximiliank/Dokumente/workbench/cache_invalidator/clearer/cacheclearer.ko")
        time.sleep(2)
        os.system("../cacheclearer/cacheclearer")

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
        rawunit = rf.read(8)
        unit = struct.unpack('Q', rawunit)[0]
        cleaned_unit = (unit >> 8) & 0x1F

    return cleaned_energy * pow(0.5, cleaned_unit)


def execute_analysis(file, strategy, loopbound, libpath, modelpath):
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

    return json.loads(output)


def write_analysis_result_to_csv(result):
    toround = 5

    with open('analysis_result.csv', 'w') as f:
        w = csv.writer(f)
        w.writerow(["File", "Worstcase", "Averagecase", "Bestcase", "Mean", "Variance", "Standard Deviation", "Measurement"])
        for key in result.keys():
            entry = result[key]
            mean = (entry["worst"] + entry["average"] + entry["best"])/3
            variant = ((entry["worst"]-mean)**2 + (entry["average"]-mean)**2 + (entry["best"]-mean)**2)/3
            deviation = math.sqrt(variant)
            w.writerow([entry["name"], round(entry["worst"], toround), round(entry["average"], toround), round(entry["best"], toround), round(mean, toround), round(variant, toround), round(deviation, toround), round(entry["measurement"], toround)])


def main(libpath, modelpath, bound, iterations, analysispath):
    simpledirpath = analysispath
    simpledir = os.listdir(simpledirpath)
    stategies = ["worst", "best", "average"]

    analysis_dict = {}

    for file in simpledir:
        if file.endswith(".ll"):
            filename = Path(file).stem

            relpath = "{0}/{1}".format(simpledirpath, file)
            print("Running analysis for file {} [{}]".format(filename, relpath))
            analysis_dict[file] = {}
            for strategy in stategies:
                # print("\t{}...".format(strategy))
                json_result = execute_analysis(relpath, strategy, int(bound), libpath, modelpath)
                analysis_dict[file][strategy] = json_result["main"]["energy"]

            analysis_dict[file]["name"] = file

            measurement = runprogram("{}/{}".format(simpledirpath, filename), int(iterations))
            analysis_dict[file]["measurement"] = measurement

    write_analysis_result_to_csv(analysis_dict)

    # for i in range(1, 1000):
    #     engergy_before = readRapl()
    #     sleep(10)
    #     energy_after = readRapl()
    #
    #     print(energy_after - engergy_before)


if __name__ == "__main__":
    if len(sys.argv) == 6:
        main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
    else:
        print("Please provide a path to a folder containing .ll files")
