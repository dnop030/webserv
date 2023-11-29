#!/usr/bin/env python3

import os
import utils

try:
	with open(utils.getEnvValue("FILENAME"), "r") as file:
		body_field = file.read()
		file.close()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(body_field) + 1,
	}
	utils.printHeaderBody(dic_header, body_field, utils.getEnvValue("STATUS_CODE"), utils.getEnvValue("STATUS_MESSAGE"))
except (utils.InternalServerError,  FileNotFoundError):
	with open("./page/error/500.html", "r") as file:
		err_page = file.read()
		file.close()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(err_page) + 1,
	}
	utils.printHeaderBody(dic_header, err_page, "500", "Internal Server Error")
	
