import random

def keepLooping(exitChance):
    if random.randint(0, exitChance) == 1:
        return False
    return True

def writeData(sets, outfile):
    with open(outfile, "w") as output:
        for i, s in enumerate(sets):
            if i % 100000 == 0:
                print(f"writing vertex {i/len(sets)}...")
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

def newData(n, theta, outfile, exitChance):
    sets = []
    for i in range(n):
        if i % 100000 == 0:
            print(f"generating vertex {i/n}...")
        newSet = set()
        for i in range(exitChance):
            newSet.add(random.randint(0, theta))
        sets.append(newSet)
    
    writeData(sets, outfile)


def cleanData(filename, outfile):
    sets = []

    with open(filename, "r") as input:
        for line in input:
            newline = line.split('\t')
            newline.pop()
            sets.append(set([ int(x) for x in newline ]))

    writeData(sets, outfile)

cleanData("data/coverage.txt", "data/realWorldSets.txt")
# newData(64*64, 999, "data/syntheticGlobal.txt", 10)