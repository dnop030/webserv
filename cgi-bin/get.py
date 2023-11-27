#!/usr/bin/env python3

import cgi
import os
import utils

try:
	status_code = utils.getEnvValue("STATUS_CODE")
	status_message = utils.getEnvValue("STATUS_MESSAGE")
	connection = utils.getEnvValue("CONNECTION")
	content_type = utils.getEnvValue("CONTENT_TYPE")
	with open(utils.getEnvValue("FILENAME"), "r") as file:
		body_field = file.read()
	print(f"HTTP/1.1 {status_code} {status_message}\r")
	print(f"Connection: {connection}\r")
	print(f"Content-Type: {content_type}\r")
	print(f"Content-Length: {len(body_field) + 1}\r")
	print("\r")
	print(body_field)
except (utils.InternalServerError,  FileNotFoundError):
	connection = utils.getEnvValue("CONNECTION")
	content_type = utils.getEnvValue("CONTENT_TYPE")
	with open("./page/error/500.html", "r") as file:
		err_page = file.read()
	print("HTTP/1.1 500 Internal Server Error\r")
	print(f"Connection: {connection}\r")
	print(f"Content-Type: {content_type}\r")
	print(f"Content-Length: {len(err_page) + 1}\r")
	print("\r")
	print(err_page)
	
