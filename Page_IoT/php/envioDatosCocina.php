<?php

    include 'funciones.php';

    $IdESP=$_POST['chipid'];
    $NivelLecturaSensor=$_POST['NLS'];
    $LecturaSensor=$_POST['LS'];
    
    $conn = conexionBD();
    $sql = "INSERT INTO datoscocina (IdESP ,NivelLecturaSensor, LecturaSensor) VALUES ('$IdESP','$NivelLecturaSensor', '$LecturaSensor')";

    if (mysqli_query($conn, $sql)) {
        mysqli_close($conn);
        exit();
    } else {
        mysqli_close($conn);
        exit();
    }

?>