import sys

def installPrograms(filenames: list()) -> None:
    programsCode = ""
    for filename in filenames:
        try:
            with open(filename, 'r') as subfile:
                filenameEncoded = list(filename)
                filenameOrd = ""
                for f in filenameEncoded:
                    filenameOrd = filenameOrd + str(ord(f)) + " "
                filenameOrd = filenameOrd + "0 "
                
                readData = subfile.read()
                readDataList = readData.split();
                
                code = "1453 " + str(len(filename)+1) + " " + filenameOrd + str(len(readDataList)) + " " + readData
                programsCode = programsCode + code
        except FileNotFoundError:
            print(filename + " could not be found")
            pass
    with open("drive", "w+") as driveFile:
        freeMem = 5000
        for i in programsCode.split():
            driveFile.write(i + " ")
            freeMem = freeMem - 1
        for j in range(freeMem):
            driveFile.write("-1 ")


if __name__ == "__main__":
    argIter = iter(sys.argv)
    next(argIter)
    filenames = list()
    for filename in argIter:
        filenames.append(filename)

    installPrograms(filenames)

