<?php

require_once("utils.php");

$file_path = getenv('ROOT_PATH') . ':' . getenv('PORT') . '/' . getenv('FILENAME_DELETE');

if (file_exists($file_path)) {
	unlink($file_path);
	$body = getenv('FILENAME_DELETE') . ' has been deleted successfully.';
	$dic_header = [
		'Connection' => getenv('CONNECTION'),
		'Content-Type' => getenv('CONTENT_TYPE'),
		'Content-Length' => strlen($body),
	];
	printHeaderBody($dic_header, $body, 200, 'OK');
} else {
	$ofile = fopen("/home/pmikada/Desktop/webserv/page/error/404.html", "r");
	$body = fread($ofile, filesize("/home/pmikada/Desktop/webserv/page/error/404.html"));
	fclose($ofile);
	$dic_header = [
		'Connection' => getenv('CONNECTION'),
		'Content-Type' => getenv('CONTENT_TYPE'),
		'Content-Length' => strlen($body),
	];
	printHeaderBody($dic_header, $body, 404, 'Not Found');
}


?>