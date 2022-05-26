//ADD
var d = document;
var type;
var name;
var code;
var storage = "";
var place;
var factor = 1;
var count = 0; 
var label = 0; //RFID
var project = "";
//FIND
var outData  = "0";

function addRow()   
{   
    count++;
    // Считываем значения с формы
    type= d.getElementById('type').value;
    name = d.getElementById('name').value;
    code = d.getElementById('code').value;
    storage = d.getElementById('storage').value;
    place = d.getElementById('place').value;
    factor = d.getElementById('factor').value;
    project = d.getElementById('project').value;
    // Находим нужную таблицу
    var tbody = d.getElementById('tab1').getElementsByTagName('TBODY')[0];

    // Создаем строку таблицы и добавляем ее
    var row = d.createElement("TR");
    tbody.appendChild(row);

    // Создаем ячейки в вышесозданной строке
    // и добавляем тх
    var td1 = d.createElement("TD");//id
    var td2 = d.createElement("TD");//type  
    var td3 = d.createElement("TD");//name  
    var td4 = d.createElement("TD");//code
    var td5 = d.createElement("TD");//storage
    var td6 = d.createElement("TD");//place
    var td7 = d.createElement("TD");//factor
    var td8 = d.createElement("TD");//project
    var td9 = d.createElement("TD");//RFID
    var td10 = d.createElement("TD");//count


    row.appendChild(td1);
    row.appendChild(td2);
    row.appendChild(td3);
    row.appendChild(td4);
    row.appendChild(td5);
    row.appendChild(td6);
    row.appendChild(td7);
    row.appendChild(td8);
    row.appendChild(td9);
    row.appendChild(td10);

    // Наполняем ячейки
    td1.innerHTML = count;
    td2.innerHTML = type;
    td3.innerHTML = name;
    td4.innerHTML = code;
    td5.innerHTML = storage;
    td6.innerHTML = place;
    td7.innerHTML = factor;
    td8.innerHTML = project;
    td9.innerHTML = inpData.label;
    td10.innerHTML = factor * inpData.count;
    $.ajax({
        type: "POST",
        url: "insert.php",
        data: {type: type, name: name, code: code, storage : storage, place : place, factor :factor, label : inpData.label,
        count: factor * inpData.count, project :project},
        success: function(result) {
            console.log(result);
        }
    });

}
    
