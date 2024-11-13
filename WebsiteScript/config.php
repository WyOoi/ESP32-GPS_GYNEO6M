<?php 
define('DB_HOST', 'fdb1030.awardspace.net'); 
define('DB_USERNAME', '4548299_tracker'); 
define('DB_PASSWORD', 'P@ssword1'); 
define('DB_NAME', '4548299_tracker');

define('GOOGLE_MAP_API_KEY', 'AIzaSyDHNUG9E870MPZ38LzijxoPyPgtiUFYjTM');

//ESP32_API_KEY is the exact duplicate of, ESP32_API_KEY in ESP32 sketch file
//Both values must be same
define('ESP32_API_KEY', 'Ad5F10jkBM0');

//http://www.example.com/gpsdata.php
define('POST_DATA_URL', 'http://bustrackerutem.atwebpages.com/gpsdata.php');

date_default_timezone_set('Asia/Kuala_Lumpur');

// Connect with the database 
$db = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME); 
 
// Display error if failed to connect 
if ($db->connect_errno) { 
    echo "Connection to database is failed: ".$db->connect_error;
    exit();
}
