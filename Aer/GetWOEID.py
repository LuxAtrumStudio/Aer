from urllib.request import urlopen
import urllib, json
transfer = open("TransferData.txt", "r")
location = transfer.read()
yql_url = "http://where.yahooapis.com/v1/places.q(" + location + ")?appid=[dj0yJmk9T2tDdFJ3bHlBa0ZyJmQ9WVdrOVdHdGlUemxWTnpBbWNHbzlNQS0tJnM9Y29uc3VtZXJzZWNyZXQmeD1kOQ--]" + "&format=json"
result = urllib.request.urlopen(yql_url).read()
out = open("WOEID.json", "w")
step = str(result, 'utf-8')
out.write(step)