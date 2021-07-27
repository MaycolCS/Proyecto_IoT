<?php

    include 'funciones.php';

    $IdESP=$_POST['chipid'];
    $EstadoLecturaSensor=$_POST['ELS'];
    $LecturaSensor=$_POST['LS'];
    
    $conn = conexionBD();
    $sql = "INSERT INTO datospasillos (IdESP ,EstadoLecturaSensor, LecturaSensor) VALUES ('$IdESP','$EstadoLecturaSensor', '$LecturaSensor')";

    if (mysqli_query($conn, $sql)) {
        mysqli_close($conn);
        exit();
    } else {
        mysqli_close($conn);
        exit();
    }

?>