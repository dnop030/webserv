import os
import utils

class InternalServerError(Exception):
	pass
def printHeader(header, key):
	value = os.environ.get(key, "NULL")
	if value == "NULL" and key == "CONTENT_TYPE":
		raise utils.InternalServerError()
	if value != "NULL":
		print(header + os.environ[key] + "\r\n")

def getEnvValue(key):
	res = os.environ.get(key, "NULL")
	if res == "NULL":
		raise utils.InternalServerError()
	return (res)

def parseEnv(input):
	if input == "":
		raise utils.InternalServerError
	env_chunk = input.split("\n")
	for env_var in env_chunk:
		if env_var:
			key, value = env_var.split("=")
			os.environ[key] = value

def printHeaderBody(dic_header, body, status_code, status_message):
	print(f"HTTP/1.1 {status_code} {status_message}\r")
	for k, v in dic_header.items():
		print(f"{k}: {v}\r")
	print("\r")
	print(body)
