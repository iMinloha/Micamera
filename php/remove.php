<?php
$filename = $_GET['f'];
if(file_exists("./albums/" .$filename)){
    unlink("./albums/" . $filename);
    echo '1';
}else{
    echo '0';
}
