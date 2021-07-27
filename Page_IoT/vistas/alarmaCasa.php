<?php

    $action = "";
    if (isset($_GET['action'])) {
        $action = $_GET['action'];
        echo ("action=" . $action);
        if ($action == "desactivar") {
            echo ('<script>alert("La alarma de la casa se desactivará")</script>');
        } 
        if ($action == "activar") {
            echo ('<script>alert("La alarma de la casa se activará")</script>');
        }
        
    }

?>
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
                <h1>Control de alarma de la casa</h1>
                <a class="boton" href="alarmaCasa.php?action=activar"><button type="button" class="boton">Activar alarma</button></a>
                <a class="boton" href="alarmaCasa.php?action=desactivar"><button type="button" class="boton">Desactivar alarma</button></a>
                <a class="boton" href="../index.html"><button type="button" class="boton">Home</button></a>
            </article>
        </section>
    </body>
</html>