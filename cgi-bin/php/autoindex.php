<?php

require_once("utils.php");

function generate_autoindex($path) {
	$content = '<html>
				<head>
					<title>AutoIndex</title>
				</head>
				<body>
					<h1>Auto index</h1>
				<ul>';

	$dir = scandir($path);
	foreach ($dir as $v) {
		if (!in_array($v, ['.', '..']))
			$content .= ('<li>' . $v . '</li>');
	}
	$content .= '</ul>
				</body>
				</html>';
	return $content;
}

$body = generate_autoindex(getenv('ROOT_PATH'));

$dic_header = [
	'Connection' => getenv('CONNECTION'),
	'Content-Type' => getenv('CONTENT_TYPE'),
	'Content-Length' => strlen($body),
];
printHeaderBody($dic_header, $body, getenv('STATUS_CODE'), getenv('STATUS_MESSAGE'));

?>