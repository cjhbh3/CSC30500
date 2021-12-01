<?php

session_start();

$_SESSION["host"] = $_POST["host"];
$_SESSION["port"] = $_POST["port"];
$_SESSION["user"] = $_POST["user"];
$_SESSION["passw"] = $_POST["password"];

header("Location: /mainMenu.html");

?>