from matplotlib import pyplot as plt

x = []
y = []

ycount = 1
with open("out.txt") as file_in:
    lines = []
    for line in file_in:
        if ycount == 500:
            break
        if "add" in line:
            x.append(int(line[line.find("R ")+2:]))
            y.append(ycount)
            ycount += 1

plt.xlabel('sectors') 
plt.ylabel('order') 
plt.figure(figsize=(5,15))
plt.plot(x, y)
plt.savefig("outfifo.png")


x = []
y = []

ycount = 1
with open("out.txt") as file_in:
    lines = []
    for line in file_in:
        if ycount == 500:
            break
        if "dsp" in line:
            x.append(int(line[line.find("R ")+2:]))
            y.append(ycount)
            ycount += 1

plt.clf()
plt.xlabel('sectors') 
plt.ylabel('order') 
#plt.figure(figsize=(5,10))
plt.plot(x, y)
plt.savefig("outsstf.png")