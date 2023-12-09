#!/usr/bin/env python

import os
import sys
import utils

try:
	path = utils.getEnvValue("ROOT_PATH") + ":" + utils.getEnvValue("PORT")
	os.makedirs(path, exist_ok=True)
	filename = utils.getEnvValue("UPLOAD_FILENAME")
	file_path = os.path.join(path, filename)
	message = ""
	
	with open(file_path, "w") as w_file:
		for line in sys.stdin:
			w_file.write(line)
		message = f"{filename} has been posted successfully."
	
	
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(message) + 1,
	}
	utils.printHeaderBody(dic_header, message, "200", "ok")

except (utils.InternalServerError,  FileNotFoundError):
	with open("./page/error/500.html", "r") as file:
		err_page = file.read()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(err_page) + 1,
	}
	utils.printHeaderBody(dic_header, err_page, "500", "Internal Server Error")
