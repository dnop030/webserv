<?php

require_once("utils.php");

$body = "";
$dic_header = [
	'Content-Length' => strlen($body),
	'Location' => getenv('REDIRECT'),
];

printHeaderBody($dic_header, $body, getenv('STATUS_CODE'), getenv('STATUS_MESSAGE'));

?>