<?php

    $Mensaje="";
    if (isset($_GET['msj'])) {
        $Mensaje=$_GET['msj'];
    }

    if($Mensaje=="sdg") {
        echo '<script>alert("No se encontraron datos para la consulta del garaje")</script>';
    } else if($Mensaje=="sdc") {
        echo '<script>alert("No se encontraron datos para la consulta de la cocina")</script>';
    } else if($Mensaje=="sdp") {
        echo '<script>alert("No se encontraron datos para la consulta de los pasillos")</script>';
    } else if($Mensaje=="sdle") {
        echo '<script>alert("No se encontraron datos para la consulta de las luces exteriores")</script>';
    }

?>

<html>
    
    <head>
        <meta charset="utf-8">
        <title>Domotica</title>
        <link type="text/css" href="CSS/Style_001.css" rel="stylesheet">
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
                <a class="boton" href="vistas/consultaCocina.php"><button type="button" class="boton">Consulta datos cocina</button></a>
                <a class="boton" href="vistas/consultaPasillos.php"><button type="button" class="boton">Consulta datos pasillos</button></a>
                <a class="boton" href="vistas/consultaLucesExteriores.php"><button type="button" class="boton">Consulta datos luces exteriores</button></a>
                <a class="boton" href="vistas/consultaGaraje.php"><button type="button" class="boton">Consulta datos garaje</button></a>
            </article>

        </section>
    </body>
</html>