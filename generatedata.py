import random

def keepLooping():
    if random.randint(0, 20) == 5:
        return False
    return True

def writeData(sets, outfile):
    with open(outfile, "w") as output:
        for i, s in enumerate(sets):
            if s == set():
                output.write(f"")
            else: 
                output.write("".join([f"{x}, " for x in s]))
            output.write('\n')

def writeForDirectInput(sets, outfile):
    with open(outfile, "w") as output:
        for i, s in enumerate(sets):
            if s == set():
                output.write("{" + f"{i}, " + "{}" + "},")
            else: 
                output.write("{" + f"{i}, " + f"{s}" + "},")
            output.write('\n')

def newData():
    sets = []
    for i in range(300):
        newSet = set()
        while keepLooping():
            newSet.add(random.randint(0, 1000))
        sets.append(newSet)
    
    writeForDirectInput(sets, "sets.txt")


def cleanData(filename):
    sets = []

    with open(filename, "r") as input:
        for line in input:
            newline = line.split('\t')
            newline.pop()
            sets.append(set([ int(x) for x in newline ]))

    writeData(sets, "sets.txt")

cleanData("coverage.txt")