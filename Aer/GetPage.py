from urllib.request import urlopen
import urllib, json
transferData = open("GetPage.tran", "r")
url = transferData.readline()
outputLocation = transferData.readline()
byteOut = urllib.request.urlopen(url).read()
stringOut = str(byteOut, 'utf-8')
dataSave = open(outputLocation, "w")
dataSave.write(stringOut)