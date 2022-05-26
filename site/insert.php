<?php

if( isset( $_POST['subm'] ) )
{

$type = $_POST['type'];
$name = $_POST['name'];
$code = $_POST['code'];
$storage = $_POST['storage'];
$place = $_POST['place'];
$factor = $_POST['factor'];
$label = $_POST['label']; #RFID
$count = $_POST['count']; #count
$project = $_POST['project']; #project
$mydata = date("d/m/Y");

$db_host = 'morozov.arduinonsk.ru'; 
$db_user = 'root'; // Логин БД
$db_password = 'nstu'; // Пароль БД
$db_base = 'SMD'; // Имя БД
$db_table = 'smd_user1'; // Имя Таблицы БД
// Подключение к базе данных
$mysqli = new mysqli("localhost: 3306",$db_user,$db_password,$db_base);
// Если есть ошибка соединения, выводим её и убиваем подключение
if ($mysqli->connect_error) 
{
     die('Ошибка : ('. $mysqli->connect_errno .') '. $mysqli->connect_error);
}


$result = $mysqli->query("INSERT INTO ".$db_table." (type, name, code, storage, place, factor, RFID, count, project, data) VALUES ('$type', '$name','$code', '$storage', '$place', '$factor', '$label', '$count', '$project', '$mydata')");

if ($result == true){
	echo "Информация занесена в базу данных";
}else{
	echo "Информация не занесена в базу данных";
}
}
?>