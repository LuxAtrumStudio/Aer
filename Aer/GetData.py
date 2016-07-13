from urllib.request import urlopen
import urllib, json
transfer = open("TransferData.txt", "r")
id = transfer.read()
baseurl = "https://query.yahooapis.com/v1/public/yql?"
yql_query = "select * from weather.forecast where woeid=" + id
yql_url = baseurl + urllib.parse.urlencode({'q':yql_query}) + "&format=json"
result = urllib.request.urlopen(yql_url).read()
step = str(result, 'utf-8')
out = open("WeatherData.json", "w") 
out.write(step)