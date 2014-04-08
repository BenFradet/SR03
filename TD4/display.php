<?php
    session_id($_GET['sessionid']);
    session_start();
    $_SESSION['var3'] = 'var3';
    $_SESSION['var4'] = 'var4';
    print_r($_SESSION);
?>
