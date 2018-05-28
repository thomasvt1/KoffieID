<?php
//ini_set('display_errors', 1);

if(!isset($_GET['uid'])) {
	http_response_code(403);
	die();
}

$uid = $_GET['uid'];

if ($uid == "04e7f3f22f4d80") // X
	die("{
		'serial': [
		'COMMAND1',
		'COMMAND2',
		'COMMAND3'
		]
	}");
if ($uid == "04589df22f4d81") // A
	die("{ 'pin': 13 }");
if ($uid == "04609df22f4d81") // B
	die("13");
if ($uid == "04689cf22f4d81") // C
	die("12");
if ($uid == "046f9df22f4d81") // D
	die("13");
if ($uid == "048e99f22f4d80") // E
	die("12");
	
die($uid);
echo "";
?>
