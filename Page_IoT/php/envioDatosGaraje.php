<?php

    include 'funciones.php';

    $IdESP=$_POST['chipid'];
    $NombrePersona=$_POST['NP'];
    $ApellidoPersona=$_POST['AP'];
    $IdTarjetaPersona=$_POST['ITP'];
    
    $conn = conexionBD();
    $sql = "INSERT INTO datosgaraje (IdESP ,NombrePersona, ApellidoPersona, IdTarjetaPersona) VALUES ('$IdESP','$NombrePersona', '$ApellidoPersona', '$IdTarjetaPersona')";

    if (mysqli_query($conn, $sql)) {
        mysqli_close($conn);
        exit();
    } else {
        mysqli_close($conn);
        exit();
    }

?>