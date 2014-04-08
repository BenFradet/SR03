<?php
	header( "Content-type: image/jpeg");
	$file_name = "1.jpg";
	$file = fopen($file_name, "r");
	$content = fread($file, filesize($file_name));
	fclose($file);

	echo $content
?>