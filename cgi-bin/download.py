#!/usr/bin/env python3

import os
import utils

try:
    #utils.parseEnv() => wait for parsing env
	# Get the path to the file you want to send
	dir_name = utils.getEnvValue("DIRECTORY_NAME")
	filename = utils.getEnvValue("FILE_NAME")
	file_path = os.path.join("../upload", dir_name, filename)

	# Set the Content-Disposition header
	content_disposition = f'attachment; filename="{filename}"'

	# Set the Content-Type header based on the file type
	content_type = utils.getEnvValue("CONTENT_TYPE")

	# Print the HTTP headers
	with open(file_path, 'rb') as file:
		res = file.read
	print("HTTP/1.1 200 OK\r\n")
	print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
	print("Content-Type: " + content_type)
	print("Content-Length: " + len(res))
	print(f'Content-Disposition: {content_disposition}' + "\r\n")
	print("\r\n")  # End of headers
	# Read and send the file content
	print(res)


except utils.InternalServerError:
    try:
        with open("../page/500.html", "r") as file:
            err_page = file.read()
        print("HTTP/1.1 500 Internal Server Error\r\n")
        print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
        print("Content-Type: text/html\r\n")
        print("Content-Length: " + str(len(err_page)) + "\r\n")
        print("\r\n")
        print(err_page)
    except Exception as e:
        print(f"Find Error!!! => f{e}")

