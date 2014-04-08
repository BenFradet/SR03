<?php
    session_start();
    if(!isset($_SESSION['noms'])) {
        $_SESSION['noms'] = array();
    }
    if(!isset($_SESSION['count'])) {
        $_SESSION['count'] = 0;
    }
?>
<html>
    <head>
        <meta char="utf-8">
        <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js"></script>
        <script src="getListOfStudents.js" type="text/javascript"></script>
    </head>
    <body>
        <label>Nom de l'&eacute;tudiant:</label>
        <form action="" method="post">
            <input type="text" id="chaine" name="chaine"><br>
            <input type="button" value="get" onclick="getListOfStudents($('#chaine').val(), 'get')">
            <input type="submit" value="submit">
        </form>
        <?php
            if(isset($_POST['chaine'])) {
                $_SESSION['noms'][$_SESSION['count']] = $_POST['chaine'];
                $_SESSION['count'] = ($_SESSION['count'] + 1) % 10;
            }
        ?>
        <div id="result"></div>
        <?php
            echo $_SESSION['count'];
            echo "\r\n";
            foreach($_SESSION['noms'] as $key=>$value) {
                echo $value.'<br>';
            }
        ?>
    </body>
</html>
