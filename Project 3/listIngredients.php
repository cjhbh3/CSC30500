<?php

session_start();

$host = $_SESSION["host"];
$user = $_SESSION["user"];
$pass = $_SESSION["passw"];
$port = $_SESSION["port"];

$recipeName = $_POST["recipeName"];

$dbName = $user;

echo "Attempting to connect to DB server: $host ...";
$conn = new mysqli($host, $user, $pass, $dbName, $port);

if ($conn->connect_error)
        die("Could not connect:".mysqli_connect_error());
else
        echo " connected!<br>";

if (!$stmt = $conn->prepare("select * from Recipe where RecipeName = ?;")){
        echo "Issue here" . htmlspecialchars($conn->error);
}


$stmt->bind_param("s", $recipeName);

if (!$stmt->execute())
        echo "Fail<br>";

$stmt->bind_result($rName, $iName, $iQuantity);

echo "<table border=1>";
echo "<tr> <th>Recipe Name</th> <th>Ingredientt Name</th> <th>Quantity</th> </tr>";

while($stmt->fetch() ) 
    {
        echo "<tr> <td>".$rName."</td>". 
                  "<td>".$iName."</td>". 
                  "<td>".$iQuantity."</td> </tr>";
    }

echo "</table>";

echo ("All done!<br>");

echo "<a href=/mainMenu.html>Main Menu</a>";
$conn->close();
?>
