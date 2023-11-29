import utils

try:
	print("HTTP/1.1 301 Moved Permanently \r\n")
	print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
	print("Content-Type: text/plain\r\n")
	print("Location: " + "http://" + utils.getEnvValue("HOSTNAME") + ":" + utils.getEnvValue("PORT") + "/" + "\r\n")
	print("\r\n")
except (utils.InternalServerError,  FileNotFoundError):
	with open("./page/error/500.html", "r") as file:
		err_page = file.read()
	dic_header = {
		"Connection" : utils.getEnvValue("CONNECTION"),
		"Content-Type" : utils.getEnvValue("CONTENT_TYPE"),
		"Content-Length" : len(err_page) + 1,
	}
	utils.printHeaderBody(dic_header, err_page, "500", "Internal Server Error")
