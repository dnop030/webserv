import utils

try:
	print("HTTP/1.1 301 Moved Permanently \r\n")
	print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
	print("Content-Type: text/plain\r\n")
	print("Location: " + "http://" + utils.getEnvValue("HOSTNAME") + ":" + utils.getEnvValue("PORT") + "/" + "\r\n")
	print("\r\n")
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
