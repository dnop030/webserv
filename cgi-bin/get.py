#!/usr/bin/env python3

#shebang => used env command to find python interpreter

#import necessary module
import cgi
import os

class InternalServerError(Exception):
	pass

#Specify the path to html file
html_path = "../page/" + os.environ["FILE_NAME"]

def printHeader(header, key):
	value = os.environ.get(key, "NULL")
	if value == "NULL" and key == "CONTENT_TYPE":
		raise InternalServerError()
	if value != "NULL":
		print(header + os.environ[key] + "\r\n")

try:
	#Open and read html file
	with open(html_path, "r") as file:
		body_field = file.read()

	print("HTTP/1.1 200 OK\r\n")
	printHeader("Date: ", "DATE")
	printHeader("Allow: ", "ALLOW")
	printHeader("Content-Type: ", "CONTENT_TYPE")
	printHeader("Content-Length: ", "CONTENT_LENGTH")
	printHeader("Location: ", "LOCATION")
	printHeader("Accept-Language: ", "HTTP_ACCEPT_LANGUAGE")
	printHeader("Server: ", "SERVER_SOFTWARE")
	printHeader("Content-Location: ", "CONTENT_LOCATION")
	printHeader("Last-Modified: ", "LAST_MODIFICATION")
	print("\r\n")
except (InternalServerError,  FileNotFoundError):
	try:
		with open("../page/500.html", "r") as file:
			err_page = file.read()
		print("HTTP/1.1 500 Internal Server Error\r\n")
		print("Content-Type: text/html\r\n")
		print("Content-Length: " + len(err_page) + "\r\n")
		print("\r\n")
		print(err_page)
	except FileNotFoundError as e:
		print(f"Find Error!!! => f{e}")
