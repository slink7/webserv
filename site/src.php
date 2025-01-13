<!DOCTYPE html>

<html>
	<head>
		<title> Titre </title>
	</head>
	<body>
		<div>
			<pre>
<?php
foreach (getenv() as $key => $value) {
	echo "\t\t\t$key => $value\n";
}
?>
			</pre>
		</div>
	</body>
</html>