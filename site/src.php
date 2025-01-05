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
			<?php
			// Vérifier et afficher toutes les variables d'environnement
			echo "<pre>";
			print_r($_ENV); // Affiche les variables d'environnement connues par $_ENV
			echo "</pre>";

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