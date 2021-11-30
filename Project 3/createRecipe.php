<?php

$host = $_POST["host"];
$user = $_POST["user"];
$pass = $_POST["password"];
$port = $_POST["port"];

$recipeName = $_POST["recipeName"];
$ingredientName = $_POST["ingredientName"];
$ingredientQuantity = $_POST["ingredientQuantity"];

$dbName = $user;

echo "Attempting to connect to DB server: $host ...";
$conn = new mysqli($host, $user, $pass, $dbName, $port);

if ($conn->connect_error)
        die("Could not connect:".mysqli_connect_error());
else
        echo " connected!<br>";

$queryString = "create table if not exists Recipe (RecipeName varchar(20), Ingredient varchar(20), Quantity number)";

if (! $conn->query($queryString))
        die("Could not create table" . $conn->error());

$stmt = $conn->prepare("insert into Recipe values (?, ? , ?)");
$stmt->bind_param( "ssi", $recipeName, $ingredientName, $ingredientQuantity);

$stmt->execute();

$conn->close();

//header('Location:/Users/cj_hess510/Desktop/Principles of Database System/Projects/Project 3/mainMenu.html');

?>