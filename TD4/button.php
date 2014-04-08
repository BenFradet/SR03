<?php
    session_start();
    $_SESSION['var1'] = 'var1';
    $_SESSION['var2'] = 'var2';
?>

<!doctype html>
<html>
    <head>
        <title>SR03</title>
        <script type="text/javascript">
            function redirect() {
                document.location = 
                    "display.php?sessionid=<?php echo session_id();?>";
            }
        </script>
    </head>
    <body>
        <input type="button" onclick="redirect()" value="vers display.php">
    </body>
</html>
