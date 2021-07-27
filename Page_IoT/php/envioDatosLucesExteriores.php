<?php

    include 'funciones.php';

    $IdESP=$_POST['chipid'];
    $LecturaSensor=$_POST['LS'];
    
    $conn = conexionBD();
    $sql = "INSERT INTO datoslucesexteriores (IdESP ,LecturaSensor) VALUES ('$IdESP','$LecturaSensor')";

    if (mysqli_query($conn, $sql)) {
        mysqli_close($conn);
        exit();
    } else {
        mysqli_close($conn);
        exit();
    }

?>