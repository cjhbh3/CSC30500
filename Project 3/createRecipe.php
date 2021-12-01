<?php

session_start();

$host = $_SESSION["host"];
$user = $_SESSION["user"];
$pass = $_SESSION["passw"];
$port = $_SESSION["port"];


$dbName = $user;

echo "Attempting to connect to DB server: $host ...";
$conn = new mysqli($host, $user, $pass, $dbName, $port);

if ($conn->connect_error)
        die("Could not connect:".mysqli_connect_error());
else
        echo " connected!<br>";

$queryString = "create table if not exists Recipe (RecipeName varchar(20), Ingredient varchar(20), Quantity INT);";

if (! $conn->query($queryString))
        echo "Dead<br>" . htmlspecialchars($conn->error);
        //die("Could not create table" . $conn->error());

if (!$stmt = $conn->prepare("insert into Recipe values (?,?,?);"))
        echo "Issue here" . htmlspecialchars($conn->error);

$stmt->bind_param("ssi", $recipeName, $ingredientName, $ingredientQuantity);

$recipeName = $_POST["recipeName"];
$ingredientName = $_POST["ingredientName"];
$ingredientQuantity = $_POST["ingredientQuantity"];

$stmt->execute();

echo "Rows changed: ".$stmt->affected_rows."<br>";

$conn->close();

header("Location: /mainMenu.html")

?>
