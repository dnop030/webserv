#!/usr/bin/env python3

import os
import utils

def generate_autoindex(path):
	content = f"""
			<html>
			<head>
				<title>AutoIndex</title>
			</head>
			<body>
				<h1>Index of {path}</h1>
			<ul>"""

	try:
		dir = os.listdir(path)
		for sub_dir in dir:
			full_path = os.path.join(path, sub_dir)
			if os.path.isdir(full_path):
				content += f"<li>{sub_dir}/</li>"
			else:
				content += f"<li>{sub_dir}</li>"
	except OSError:
		raise utils.InternalServerError

	content += "</ul></body></html>"
	return content

try:
	content = generate_autoindex(utils.getEnvValue("ROOT_PATH"))

	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(content) + 1,
	}
	utils.printHeaderBody(dic_header, content, utils.getEnvValue("STATUS_CODE"), utils.getEnvValue("STATUS_MESSAGE"))
except (utils.InternalServerError,  FileNotFoundError):
	with open("./page/error/500.html", "r") as file:
		err_page = file.read()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(err_page) + 1,
	}
	utils.printHeaderBody(dic_header, err_page, "500", "Internal Server Error")
