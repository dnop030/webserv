#!/usr/bin/env python

import os
import cgi
import sys
import utils

try:
    dir_name = utils.getEnvValue("DIRECTORY_NAME")
    filename = utils.getEnvValue("FILE_NAME")
    file_path = os.path.join("../upload", dir_name, filename)

    # Check if the file exists
    if os.path.exists(file_path):
        # Perform the deletion
        os.remove(file_path)
        message = f"{filename} has been deleted successfully."
        print("HTTP/1.1 200 OK\r\n")
        print("Content-Type: text/plain\r\n")
        print("Content-Length: " + str(len(message)) + "\r\n")
        print("\r\n")
        print(message)
    else:
        raise utils.InternalServerError

except utils.InternalServerError:
    try:
        with open("../page/500.html", "r") as file:
            err_page = file.read()
        print("HTTP/1.1 500 Internal Server Error\r\n")
        print("Content-Type: text/html\r\n")
        print("Content-Length: " + str(len(err_page)) + "\r\n")
        print("\r\n")
        print(err_page)
    except FileNotFoundError as e:
        print(f"Find Error!!! => f{e}")
