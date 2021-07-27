<?php

    function conexionBD() {
        $conexion = mysqli_connect('localhost','root','','datos_iot');
        return $conexion;
    }

    function conexionDBGaraje() {
        $conexion = conexionBD();
        $consulta = "SELECT * FROM datosgaraje";
        $datos = mysqli_query($conexion, $consulta);
        return $datos;
    }

    function contadorDatosGaraje() {
        $datos = conexionDBGaraje();
        $listaDatos = array();
        $contDatos = 0;
        while($fila = mysqli_fetch_array($datos)) {
            $contDatos = $contDatos+1;
        }
        return $contDatos;
    }

    function conexionDBCocina() {
        $conexion = conexionBD();
        $consulta = "SELECT * FROM datoscocina";
        $datos = mysqli_query($conexion, $consulta);
        return $datos;
    }

    function contadorDatosCocina() {
        $datos = conexionDBCocina();
        $listaDatos = array();
        $contDatos = 0;
        while($fila = mysqli_fetch_array($datos)) {
            $contDatos = $contDatos+1;
        }
        return $contDatos;
    }

    function conexionDBPasillos() {
        $conexion = conexionBD();
        $consulta = "SELECT * FROM datospasillos";
        $datos = mysqli_query($conexion, $consulta);
        return $datos;
    }

    function contadorDatosPasillos() {
        $datos = conexionDBPasillos();
        $listaDatos = array();
        $contDatos = 0;
        while($fila = mysqli_fetch_array($datos)) {
            $contDatos = $contDatos+1;
        }
        return $contDatos;
    }

    function conexionDBLucesExteriores() {
        $conexion = conexionBD();
        $consulta = "SELECT * FROM datoslucesexteriores";
        $datos = mysqli_query($conexion, $consulta);
        return $datos;
    }

    function contadorDatosLucesExteriores() {
        $datos = conexionDBLucesExteriores();
        $listaDatos = array();
        $contDatos = 0;
        while($fila = mysqli_fetch_array($datos)) {
            $contDatos = $contDatos+1;
        }
        return $contDatos;
    }

?>