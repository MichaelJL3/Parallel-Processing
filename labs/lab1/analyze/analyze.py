
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator, FuncFormatter

files=[];
num=2;

while (num<=4096):
	files.append("analyze/data/"+str(num)+"_lab1.csv");
	num*=2;

coord=[[] for i in range(8)];
for filename in files:
	indx=0;

	file=open(filename, "r");
	for line in file:
		coord[indx].append(float(line));
		indx+=1;

coord=coord[1:]
proc=1;
for co in coord:
	plt.plot(co, linewidth=2.0, label=str(proc));
	proc=proc*2;

ax = plt.gca()
ax.xaxis.set_major_locator(MultipleLocator(1))
ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: int(2**(x+1))))

plt.gcf().autofmt_xdate()
plt.grid(True)
plt.ylabel('Time in seconds')
plt.xlabel('data size')
plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.);

plt.savefig('graph.jpg');
plt.show();