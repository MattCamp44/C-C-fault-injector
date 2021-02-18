import matplotlib.pyplot as plt
import csv
import time
import sys

inputFile = []


def readFile(fileName):
    with open(fileName, "r") as Report:
        reader = csv.reader(Report, delimiter=",")
        for row in reader:
            inputFile.append(row)
    

def BarPlot(x, y,xlabel,ylabel,title):
    fig = plt.figure()

    #ax = fig.add_axes([0, 0, 1, 1])  # t : 0 ,z : 0 ,y : 1 ,x :1
    ax = fig.add_subplot(111)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.bar(x, y)

    plt.show()

def FunctionsGraph():

    # this create a bargraph that display the number of injections made in that function
    # another the number of correct execution
    # another the number of hang

    # during the simulation
    listFunctions = [] # functions name
    NumberOfRun = [] # number of entry simulation
    NumberOfCorrect = [] # number of corrected execution
    NumberOfUncorrected = []
    NumberOfOutputDifferent = []
    fname = ""

    for row in inputFile: # extract name of functions
        fname = row[0]
        if fname not in listFunctions:
            listFunctions.append(fname)


    NumberOfRun = [0 for i in range(len(listFunctions))] # inizializzo liste
    NumberOfCorrect = [0 for i in range(len(listFunctions))]
    NumberOfHang = [0 for i in range(len(listFunctions))]
    NumberOfUncorrected = [0 for i in range(len(listFunctions))]
    NumberOfOutputDifferent = [0 for i in range(len(listFunctions))]

    for row in inputFile:
        fname = row[0]
        pos = listFunctions.index(fname)
        NumberOfRun[pos] = NumberOfRun[pos] + 1
        if(row[3] == '1' ): # run with no problem
            NumberOfCorrect[pos] = NumberOfCorrect[pos] + 1
        if(row[3] == '0' and row[6] == '1'):
            NumberOfHang[pos] = NumberOfHang[pos] + 1
        if(row[3] == '0' ):
            NumberOfUncorrected[pos] = NumberOfUncorrected[pos] + 1
        if (row[4] == '1'):
            NumberOfOutputDifferent[pos] = NumberOfOutputDifferent[pos] + 1
    #print(listFunctions)
    #print(NumberOfCorrect)
    #print(NumberOfHang)


    

    # BarPlot(listFunctions,NumberOfRun,"Functions","Number of Injection","Tested runs")
    # BarPlot(listFunctions, NumberOfCorrect, "Functions", "Number of Correct functions", "Correct runs")
    # BarPlot(listFunctions, NumberOfHang, "Functions", "Number of Hang", "Hang runs")
    # BarPlot(listFunctions, NumberOfUncorrected, "Functions", "Number of Uncorrected", "Different output runs")
    # BarPlot(listFunctions, NumberOfOutputDifferent, "Functions", "Number of different output","Number of different output Functions")

   
    fig = plt.figure()
    
    #ax = fig.add_axes([0, 0, 1, 1])  # t : 0 ,z : 0 ,y : 1 ,x :1
    ax = fig.add_subplot(111)
    ax.set_xlabel("Functions")
    ax.set_ylabel("N of runs")
    ax.set_title("Faulty runs over total runs")
    # ax.bar(x, y)
    ax.bar(listFunctions,NumberOfRun,width=0.2, color='b' )
    ax.bar(listFunctions, NumberOfUncorrected,width=0.2, color='r' )
    # ax.bar(listFunctions, NumberOfHang,width=0.2, color='y' )
    plt.show()
    
    
    
    
    fig = plt.figure()
    
    #ax = fig.add_axes([0, 0, 1, 1])  # t : 0 ,z : 0 ,y : 1 ,x :1
    ax = fig.add_subplot(111)
    ax.set_xlabel("Functions")
    ax.set_ylabel("N of runs")
    ax.set_title("Hung runs over faulty runs")
    # ax.bar(x, y)
    ax.bar(listFunctions,NumberOfUncorrected,width=0.2, color='r' )
    ax.bar(listFunctions, NumberOfHang,width=0.2, color='k' )
    # ax.bar(listFunctions, NumberOfHang,width=0.2, color='y' )
    plt.show()
    
    
    
    fig = plt.figure()
    
    #ax = fig.add_axes([0, 0, 1, 1])  # t : 0 ,z : 0 ,y : 1 ,x :1
    ax = fig.add_subplot(111)
    ax.set_xlabel("Functions")
    ax.set_ylabel("N of runs")
    ax.set_title("Faulty output runs over faulty runs")
    # ax.bar(x, y)
    ax.bar(listFunctions,NumberOfUncorrected,width=0.2, color='r' )
    ax.bar(listFunctions, NumberOfOutputDifferent,width=0.2, color='k' )
    # ax.bar(listFunctions, NumberOfHang,width=0.2, color='y' )
    plt.show()


if __name__ == "__main__":
    
    if(len(sys.argv) <= 1):
        print("[Error] give me a csv report")
        exit(1)

    print("[Main] Starting script")
    fileName = sys.argv[1]
    readFile(fileName)
    FunctionsGraph()
    exit(0)
    print("[Main] Exit..")