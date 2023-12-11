#!/usr/bin/env python

import os
import sys
import utils

try:
	path = utils.getEnvValue("ROOT_PATH") + ":" + utils.getEnvValue("PORT")
	os.makedirs(path, exist_ok=True)
	filename = utils.getEnvValue("UPLOAD_FILENAME")
	file_path = os.path.join(path, filename)
	status = 201
	status_message = "Created"
	message = ""
	
	if (os.path.isfile(file_path)):
		message = f"{filename} already exists."
		status = "400"
		status_message = "Bad Request"
	else:
		with open(file_path, "w") as w_file:
			for line in sys.stdin:
				w_file.write(line)
		message = f"{filename} has been posted successfully."
	
	
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(message) + 1,
	}
	utils.printHeaderBody(dic_header, message, status, status_message)

except (utils.InternalServerError,  FileNotFoundError):
	with open("./page/error/500.html", "r") as file:
		err_page = file.read()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(err_page) + 1,
	}
	utils.printHeaderBody(dic_header, err_page, "500", "Internal Server Error")
