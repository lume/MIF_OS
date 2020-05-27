import sys

def readDrive(filename: str) -> list:
    readList = list()
    with open(filename, 'r') as readFile:
        stringOfFile = readFile.read().replace('\n', '')
        readList = stringOfFile.split(' ')
    returnList = list()

    for el in readList:
        if el != '-1':
            returnList.append(el)
        else:
            break

    return returnList

def readWholeFile(filename: str) -> list:
    readList = list
    with open(filename, 'r') as readFile:
        stringOfFile = readFile.read().replace('\n', '')
        readList = stringOfFile.split(' ')
    return readList

def modifyEndOfFileList(fileList: list, listToAppend: list) -> list:
    newList = list()
    i = 0
    for l in fileList:
        if l == -1:
            break
        else:
            newList.append(l)

    for element in listToAppend:
        if element != '':
            newList.append(element)

    if len(newList) > 5000:
        return None
    else:
        return newList

def writeDrive(listToWrite: list) -> None:
    print(listToWrite)
    with open("drive", "w+") as driveFile:
        freeMem = 5000
        for i in listToWrite:
            driveFile.write(i + " ")
            freeMem = freeMem - 1
        freeMem = freeMem - 1
        for j in range(freeMem):
            driveFile.write("-1 ")

if __name__ == "__main__":
    argIter = iter(sys.argv)
    next(argIter)
    filenames = list()
    for filename in argIter:
        filenames.append(filename)

    listToAppend = list()

    for f in filenames:
        filenameEncoded = list(filename)
        filenameOrd = ""
        for fencoded in filenameEncoded:
            filenameOrd = filenameOrd + str(ord(fencoded)) + " "
    
        filenameOrd = filenameOrd + "0 "

        listToAppend.append('1454')
        listToAppend.append(str(len(f) + 1))
        for ford in filenameOrd.split(' '):
            if ford != '':
                listToAppend.append(ford)
        listToAppend.append(str(len(readWholeFile(f))))
        for fileEntry in readWholeFile(f):
            if fileEntry != '':
                listToAppend.append(fileEntry)  
        listToAppend.append('-2')

    fileData = readDrive("drive")
    
    fileData = modifyEndOfFileList(fileData, listToAppend)

    writeDrive(fileData)