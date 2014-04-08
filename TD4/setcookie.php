<?php
	header("Content-Type: text/html; charset=UTF-8");
	$name = isset($_POST["name"])? $_POST["name"] : "cookieName";
	$value = isset($_POST["value"])? $_POST["value"] : "cookieValue";
	$path = (isset($_POST["domain"]) && $_POST["path"] != "")? $_POST["path"]:"/";
	$domain = isset($_POST["domain"])? $_POST["domain"]:"txua.utc.fr";
	$expires = (isset($_POST["domain"]) && $_POST["expireseconds"] != 0)? $_POST["expireseconds"] + time() : 0;

	setcookie($name, $value, $expires, $path, $domain);

	echo "<h3>cookie envoyé au navigateur (mais pas forcément accepté) :</h3>";

	//table
	echo "<br />";
	echo "<table>";
	echo "<tr>";
	echo "<td>name</td><td>$name</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td>value</td><td>$value</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td>path</td><td>$path</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td>domain</td><td>$domain</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td>expires</td><td>$expires</td>";
	echo "</tr>";
	echo "</table>";

	if(isset($_POST["phpinfo"])){
		phpinfo();
	}

?>