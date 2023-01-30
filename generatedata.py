import random

def keepLooping():
    if random.randint(0, 20) == 5:
        return False
    return True

sets = []
for i in range(300):
    newSet = set()
    while keepLooping():
        newSet.add(random.randint(0, 1000))
    sets.append(newSet)


with open("sets.txt", "w") as output:
    for i, s in enumerate(sets):
        if s == set():
            output.write("{" + f"{i}," + " {}" + "},")
        else: 
            output.write("{" + f"{i}, {s}" + "},")
        output.write('\n')