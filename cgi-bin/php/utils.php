<?php

function printHeaderBody($dic_header, $body, $status_code, $status_message) {
	echo "HTTP/1.1 " . $status_code . " " . $status_message . "\r\n";
	foreach ($dic_header as $k => $v) {
		echo $k . ": " . $v . "\r\n";
	}
	echo "\r\n";
	echo $body;
}

?>