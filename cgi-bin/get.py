#!/usr/bin/env python3

#shebang => used env command to find python interpreter

#import necessary module
import cgi
import os
import utils

try:
	#Open and read html file
	with open("../page/" + utils.getEnvValue("FILENAME"), "r") as file:
		body_field = file.read()

	print("HTTP/1.1 200 OK\r\n")
	# utils.printHeader("Date: ", "DATE")
	# utils.printHeader("Allow: ", "ALLOW")
	utils.printHeader("Content-Type: ", "CONTENT_TYPE")
	utils.printHeader("Content-Length: ", "CONTENT_LENGTH")
	# utils.printHeader("Accept-Language: ", "HTTP_ACCEPT_LANGUAGE")
	# utils.printHeader("Server: ", "SERVER_SOFTWARE")
	# utils.printHeader("Content-Location: ", "CONTENT_LOCATION")
	# utils.printHeader("Last-Modified: ", "LAST_MODIFICATION")
	print("\r\n")
	print(body_field)
except (utils.InternalServerError,  FileNotFoundError):
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
