<?php

require_once("utils.php");

$path = getenv('ROOT_PATH') . ':' . getenv('PORT');
$filename = getenv('FILENAME_DOWNLOAD');
$file_path = $path . '/' . $filename;
$read_file = '';

if (file_exists($file_path)) {
	$ofile = fopen($file_path, "r");
	$read_file = fread($ofile, filesize($file_path));
	fclose($ofile);
	$status = 200;
	$status_message = 'OK';
	$dic_header = [
		'Connection' => getenv('CONNECTION'),
		'Content-Type' => 'application/octet-stream',
		'Content-Length' => strlen($read_file),
		'Content-Disposition' => 'attachment; filename=' . $filename,
	];
} else {
	$ofile = fopen("./page/error/404.html", "r");
	$read_file = fread($ofile, filesize('./page/error/404.html'));
	fclose($ofile);
	$status = 404;
	$status_message = 'Not Found';
	$dic_header = [
		'Connection' => getenv('CONNECTION'),
		'Content-Type' => 'text/html',
		'Content-Length' => strlen($read_file),
	];
}

printHeaderBody($dic_header, $read_file, $status, $status_message);
?>