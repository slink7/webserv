<!DOCTYPE html>

<html>
	<head>
		<title> Titre </title>
	</head>
	<body>
		<div>
			<?php 
				echo "Salut\n";
				foreach ($_ENV as $env) {
					echo $env . PHP_EOL;
				}
				print_r($_ENV);
				echo getenv('TEST');
				echo getenv('HOST');
			?>
		</div>
	</body>
</html>