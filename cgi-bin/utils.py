import os
import utils

#Specify the path to html file
html_path = "../page/" + os.environ["FILE_NAME"]

def printHeader(header, key):
	value = os.environ.get(key, "NULL")
	if value == "NULL" and key == "CONTENT_TYPE":
		raise utils.InternalServerError()
	if value != "NULL":
		print(header + os.environ[key] + "\r\n")

def getEnvValue(key):
	res = os.environ.get(key, "NULL")
	if res == "NULL":
		raise utils.InternalServerError()
	return (res)
