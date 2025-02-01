<!DOCTYPE html>

<html>
	<head>
		<title> Titre </title>
	</head>
	<body>
		<div>
			<?php
			// Vérification supplémentaire avec getenv()
			echo "<pre>";
			foreach (getenv() as $key => $value) {
				echo "$key => $value\n";
			}
			echo "</pre>";
			?>
		</div>
	</body>
</html>