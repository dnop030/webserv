#!/usr/bin/env python3

import os
import utils

try:
	path = utils.getEnvValue("ROOT_PATH") + ":" + utils.getEnvValue("PORT")
	filename = utils.getEnvValue("FILENAME_DOWNLOAD")
	file_path = os.path.join(path, filename)
	read_file = ""
	
	if (os.path.isfile(file_path)):
		with open(file_path, "r") as file:
			read_file = file.read()
		file.close()
	else:
		raise utils.InternalServerError
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : "application/octet-stream",
		"Content-Length" : len(read_file) + 1,
		"Content-Disposition" : f'attachment; filename="{filename}"'
	}
	utils.printHeaderBody(dic_header, read_file, "200", "ok")
except (utils.InternalServerError,  FileNotFoundError):
	with open("./page/error/404.html", "r") as file:
		err_page = file.read()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : "text/html",
		"Content-Length" : len(err_page) + 1,
	}
	utils.printHeaderBody(dic_header, err_page, "404", "Not Found")

