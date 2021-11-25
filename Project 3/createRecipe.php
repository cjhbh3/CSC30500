<?php

//$host = $_POST["host"];
//$user = $_POST["user"];
//$pass = $_POST["password"];

//$dbName = $user;

echo "Attempting to connect to DB server: $host ...";
//$conn = new mysqli($host, $user, $pass, $dbName);
$conn = new mysqli("luredir.hopto.org", "chess", "Lu05107162", "chess", "40306");

if ($conn->connect_error)
        die("Could not connect:".mysqli_connect_error());
else
        echo " connected!";


//header('Location:/Users/cj_hess510/Desktop/Principles of Database System/Projects/Project 3/mainMenu.html');

?>