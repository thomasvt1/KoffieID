<?php
ini_set('display_errors', 1);

if(!isset($_GET['uid'])) {
	http_response_code(403);
	die("Missing post information");
}

$uid = $_GET['uid'];

//die($uid);

if ($uid == "abcdefgh")
	die("12");
elseif ($uid == "abcdefgh")
	die("13");
elseif ($uid == "abcdefgh")
	die("12");
echo "";
?>
