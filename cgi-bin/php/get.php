<?php
require_once("utils.php");

$ofile = fopen(getenv('FILENAME'), "r");
$body = "";

if ($ofile == FALSE) {
	$body = "File not found";
} else {
	$body = fread($ofile, filesize(getenv('FILENAME')));
	fclose($ofile);
}

$dic_header = [
	'Connection' => getenv('CONNECTION'),
	'Content-Type' => getenv('CONTENT_TYPE'),
	'Content-Length' => filesize(getenv('FILENAME')),
];
printHeaderBody($dic_header, $body, getenv('STATUS_CODE'), getenv('STATUS_MESSAGE'));

?>