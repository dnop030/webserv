#!/usr/bin/env python

import os
import cgi
import sys
import utils

try:
	path = utils.getEnvValue("ROOT_PATH") + ":" + utils.getEnvValue("PORT")
	filename = utils.getEnvValue("FILENAME_DELETE")
	file_path = os.path.join(path, filename)

	if os.path.exists(file_path):
		os.remove(file_path)
		message = f"{filename} has been deleted successfully."

		dic_header = {
			"Connection" : utils.getEnvValue("CONNECTION"),
			"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
			"Content-Length" : len(message) + 1,
		}
		utils.printHeaderBody(dic_header, message, "200", "ok")
	else:
		raise utils.InternalServerError
except (utils.InternalServerError,  FileNotFoundError):
	with open("./page/error/404.html", "r") as file:
		err_page = file.read()
		file.close()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(err_page) + 1,
	}
	utils.printHeaderBody(dic_header, err_page, "404", "Not Found")
