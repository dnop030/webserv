#!/usr/bin/env python

import os
import utils

def generate_autoindex(path):
    content = "<html><head><title>AutoIndex</title></head><body><h1>Index of {path}</h1><ul>".format(path=path)

    try:
        dir = os.listdir(path)
        for sub_dir in dir:
            full_path = os.path.join(path, sub_dir)
            if os.path.isdir(full_path):
                content += "<li>{sub_dir}/</li>".format(sub_dir=sub_dir)
    except OSError:
       raise utils.InternalServerError

    content += "</ul></body></html>"
    return content

try:
    #utils.parseEnv() => wait for parsing env

    # Set the directory you want to generate autoindex for
    dir_path = utils.getEnvValue("PATH")

    # Generate and print the autoindex content
    content = generate_autoindex(dir_path)

    # Print the HTTP headers
    print("HTTP/1.1 200 OK\r\n")
    print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
    print("Content-Type: " + utils.getEnvValue("CONTENT_TYPE") + "\r\n")
    print("Content-Length: " + len(content) + "\r\n")
    print("Location: " + "http://" + utils.getEnvValue("HOSTNAME") + "/page/" + utils.getEnvValue("FILENAME") + "\r\n")
    print(content)

except utils.InternalServerError:
    try:
        with open("../page/500.html", "r") as file:
            err_page = file.read()
        print("HTTP/1.1 500 Internal Server Error\r\n")
        print("Connection: " + utils.getEnvValue("CONNECTION") + "\r\n")
        print("Content-Type: text/html\r\n")
        print("Content-Length: " + str(len(err_page)) + "\r\n")
        print("\r\n")
        print(err_page)
    except Exception as e:
        print(f"Find Error!!! => f{e}")
