<?php

    include '../php/funciones.php';
    
    $contDatosGaraje = contadorDatosLucesExteriores();

    if ($contDatosGaraje == 0) {
        header("Location: ../mainPage.php?msj=sdle");
        exit();
    }
    
?>

<!DOCTYPE html PUBLIC>

<html>
    
    <head>
        <meta charset="utf-8">
        <title>Domotica</title>
        <link type="text/css" href="../CSS/Style_001.css" rel="stylesheet">
        <link href="https://fonts.googleapis.com/css?family=Sulphur+Point&display=swap" rel="stylesheet">
    </head>

    <body>

        <section>
            <article class="encabezado">
                <article class="info">
                    <h1>Alejandra Lozano, Estefania Delgado, Mario Gonzalez, Maycol Canastero, Natalia Zamudio</h1>
                    <h2>Estudiantes ingeniería de sistemas</h2>
                    <h3>IOT internet de las cosas - Electiva</h3>
                </article>
            </article>
            <article class="cuerpo">
                <h1>Datos de ingreso al garaje</h1>
                <table id="table_ingresos">
                    <tr>
                        <th>Id</th>
                        <th>Id ESP8266</th>
                        <th>Lectura sensor</th>
                    </tr>
                    <?php
                    $datos = conexionDBLucesExteriores();
                    while($fila = mysqli_fetch_array($datos)) {
                        ?><tr>
                            <td><?php echo $fila['IdDatoIngreso'];?></td>
                            <td><?php echo $fila['IdESP'];?></td>
                            <td><?php echo $fila['LecturaSensor'];?></td>
                        </tr>
                    <?php
                    }
                ?>
                </table>
                <a class="boton" href="../mainPage.php"><button type="button" class="boton">Home</button></a>
        </section>

    </body>

</html>