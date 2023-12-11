<?php

require_once("utils.php");

$path = getenv('ROOT_PATH') . ':' . getenv('PORT');

if (!file_exists($path)) {
	mkdir($path, 0764, true);
}

$file_path = $path . '/'. getenv('UPLOAD_FILENAME');
$status = 201;
$status_message = 'Created';
$message = '';

if (file_exists($file_path)) {
	$message = getenv('UPLOAD_FILENAME') . ' already exists';
	$status = 400;
	$status_message = 'Bad Request';
} else {
	$ofile = fopen($file_path, "w");
	while ($v = fgets(STDIN)) {
		$input .= $v;
	}
	$input = fgets(STDIN);
	fwrite($ofile, $input);
	fclose($ofile);
	$message = getenv('UPLOAD_FILENAME') . ' has been uploaded successfully.';
}

$dic_header = [
	'Connection' => getenv('CONNECTION'),
	'Content-Type' => getenv('CONTENT_TYPE'),
	'Content-Length' => strlen($message),
];
printHeaderBody($dic_header, $message, $status, $status_message);

?>