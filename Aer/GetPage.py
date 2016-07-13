from urllib.request import urlopen
import urllib, json
transferData = open("GetPage.tran", "r")
url = transferData.readline()
print(url)
outputLocation = transferData.readline()
print(outputLocation)
byteOut = urllib.request.urlopen(url).read()
stringOut = str(byteOut, 'utf-8')
dataSave = open(outputLocation, "w")
print(stringOut)
dataSave.write(stringOut)