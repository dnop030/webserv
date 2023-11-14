#!/usr/bin/env python

import os
import cgi
import sys
import utils

try:
	#utils.parseEnv() => wait for parsing env
	dir_name = utils.getEnvValue("DIRECTORY_NAME")
	# Create a new directory (if it doesn't exist)
	new_dir = "../upload/" + dir_name
	os.makedirs(new_dir, exist_ok=True)

	body = sys.stdin.read()
	filename = utils.getEnvValue("FILE_NAME")
	file_path = os.path.join(dir_name, filename)

	with open(file_path, 'wb') as res_file:
		file_path.write(body)
		message = f"{filename} has been posted successfully."
	print("HTTP/1.1 200 OK\r\n")
	print("Content-Type: text/plain\r\n")
	print("Content-Length: " + str(len(message)) + "\r\n")
	#print("Location: " + "http://" + utils.getEnvValue("HOSTNAME") + "/page/" + utils.getEnvValue("FILENAME") + "\r\n")
	print("\r\n")
	print(message)

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
