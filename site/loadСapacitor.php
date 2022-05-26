<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <link rel="stylesheet" href="myCSS.css">
  <script src="https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.js" type="text/javascript"></script>
  <!-- mqtt   -->
  <script src="myMQTT.js"></script> <!-- mqtt   -->
  <script src="addRow.js"></script> <!-- добавить сточку   -->
  <script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script>
  <script type="text/javascript" src="html2csvtoHTML.js"></script>
  <script type="text/javascript" src="html2csv.js"></script>
</head>
<body>
<h1>Чипосчет</h1> 
<h2>Конденсаторы на складе</h2>
<br>  

<?php
// Соединение, выбор базы данных
$db_host = 'morozov.arduinonsk.ru'; 
$db_user = 'root'; // Логин БД
$db_password = 'nstu'; // Пароль БД
$db_base = 'SMD'; // Имя БД
$db_table = 'smd_user1'; // Имя Таблицы БД
// Подключение к базе данных
//$mysqli = new mysqli("localhost: 3306",$db_user,$db_password,$db_base);
$mysqli = new mysqli("localhost: 3306",$db_user,$db_password,$db_base);
// Если есть ошибка соединения, выводим её и убиваем подключение
if ($mysqli->connect_error)
{
     die('Ошибка : ('. $mysqli->connect_errno .') '. $mysqli->connect_error);
}
//$mysqli->query("SET NAMES 'utf8'",$mysqli);

$query ="SELECT * FROM `smd_user1` WHERE `type` = 'Конденсатор'";
$result = $mysqli->query($query);

echo "<table>";
echo "<thead>
<tr>
  <th>№</th>
  <th>Тип</th>
  <th>Наименование</th>
  <th>Артикул</th>
  <th>Склад</th>
  <th>Место хранения</th>
  <th>Множитель</th>
  <th>Метка</th>
  <th>Количество</th>
  <th>Проект</th>
  <th>Дата</th>
  <th>Списать</th>
</tr>
</thead>";
while ($myrow = $result->fetch_array(MYSQLI_ASSOC)){
    echo "<tr>";
    printf ("<td>%d</td> <td>%s</td> <td>%s</td> <td>%s</td><td>%s</td> <td>%s</td> <td>%s</td> <td>%s</td><td>%d</td> <td>%s</td> <td>%s</td>", $myrow["id"], $myrow["type"], $myrow["name"], $myrow["code"], $myrow["storage"], $myrow["place"], $myrow["factor"], $myrow["RFID"], $myrow["count"], $myrow["project"], $myrow["data"] );
    echo "<td><input type=\"checkbox\"></td></tr>";
    
}
echo "</table>";
?>
<br>
<center>
  <a href="index.html"class="button">Списать выбранные компоненты</a>
  <a href="index.html"class="button">На главную</a>
  <a href="find.html"class="button">Страница Поиска</a>
</center>

<form action="" id="find_components" method="POST" onsubmit="findLabel();return false;">
    <fieldset>
      <legend>Поиск копмонентов по меткам</legend>
      <div class="col-25">
        <label for="factor">Введите номер</label>
      </div>
      <div class="col-75">
        <input type="text" name="outLabel" id="outLabel" value="" size="12" tabindex="5" />
      </div>
      <div class="col-25">
        <label for="subm"></label>
        <input type="submit" name="subm" class="bigbutton" value="Отправить на плату" tabindex="6" />
      </div>      
    </fieldset>
  </form>
  <br>
  
  
  <center>
    <input value="Найти ящик с резисторами" type="button" class="button" onclick="findResistor()">
    <input value="Найти ящик с конденсаторами" type="button" class="button" onclick="findСapacitor()">
    <input value="Отменить поиск" type="button" class="button" onclick="findStop()">
    <br>
    <a href="index.html"class="button">На главную</a>
    <!-- <a href="make_project.html"
    style="text-decoration:none;color:#000;background-color:#ddd;border:1px solid #ccc;padding:8px;">Собрать комплектацию</a>-->
    <a href="load.php"  class="button">Обновить базу</a>
  </center>
</body>
</html>
