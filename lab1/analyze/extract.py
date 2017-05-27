
import re

files=[]
num=2

while (num<=4096):
	files.append("times/"+str(num)+"_lab1.txt");
	num*=2;

for filename in files:
	file = open(filename, "r");
	newfile = filename.replace("times/", "analyze/data/").replace(".txt", ".csv")
	newfile = open(newfile, "w");

	summation=0.0

	for line in file:
		newProc = re.search(r'Testing', line);
		if(newProc):
			newfile.write(str(summation/5)+"\n");
			summation=0.0

		else:
			real = re.search(r'real.*', line);
			if(real):
				summation += float(re.search(r'\d+\.\d+', line).group(0));

	newfile.write(str(summation/5)+"\n");

	newfile.close();