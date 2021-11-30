<?php

$host = $_POST["host"];
$user = $_POST["user"];
$pass = $_POST["password"];
$port = $_POST["port"];

$recipeName = $_POST["recipeName"];

$dbName = $user;

echo "Attempting to connect to DB server: $host ...";
$conn = new mysqli($host, $user, $pass, $dbName, $port);

if ($conn->connect_error)
        die("Could not connect:".mysqli_connect_error());
else
        echo " connected!<br>";


?>