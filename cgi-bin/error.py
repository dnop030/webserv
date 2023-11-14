import utils
import re

try:
	#utils.parseEnv() => wait for parsing env
	status = utils.getEnvValue("STATUS_CODE")
	with open("../page/" + status + ".html", "r") as file:
		err_file = file.read()
	# .*? => . mean Matches any char except newline
	# *? => matches zero or more occurrence of the preceding element (.) in a non-greedy way => just once
	status_message = re.search(r'<title>(.*?)</title>')
	print("HTTP/1.1" + status + " " + status_message + "\r\n")
	print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
	print("Content-Type: text/html\r\n")
	print("Content-Length: " + len(err_file) + "\r\n")
	print("Location: " + "http://" + utils.getEnvValue("HOSTNAME") + "/page/" + status + ".html" + "\r\n")
	print("\r\n")
	print(err_file)

except (utils.InternalServerError,  FileNotFoundError):
	try:
		with open("../page/500.html", "r") as file:
			err_page = file.read()
		print("HTTP/1.1 500 Internal Server Error\r\n")
		print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
		print("Content-Type: text/html\r\n")
		print("Content-Length: " + len(err_page) + "\r\n")
		print("\r\n")
		print(err_page)
	except Exception as e:
		print(f"Find Error!!! => f{e}")
