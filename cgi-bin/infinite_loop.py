#!/usr/bin/env python3

# test_cgi_error.py

import time

#duration in second
duration = 30

#Get the start time
start = time.time()

print("Test cgi file error by infinite loop")
while True:
	# Simulate some processing
	print("Infinite loop...will terminate in 30 seconds")

	if time.time() - start >= duration:
		print("Timeout => break out of infinite loop")
		break

	time.sleep(1)  # Sleep for 1 second
