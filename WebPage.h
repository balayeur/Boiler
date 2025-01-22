#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
    integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
      font-family: Arial;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
    }

    h2 {
      font-size: 3.0rem;
    }

    p {
      font-size: 3.0rem;
    }

    .units {
      font-size: 1.2rem;
    }

    .ds-labels {
      font-size: 1.5rem;
      vertical-align: middle;
      padding-bottom: 15px;
    }


    /* Styles for toggle switch */
    .switch {
        position: relative;
        display: inline-block;
        width: 60px;
        height: 34px;
    }

    .switch input {
        opacity: 0;
        width: 0;
        height: 0;
    }

    .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 34px;
    }

    .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: white;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 50%;
    }

    input:checked + .slider {
        background-color: #2196F3;
    }

    input:focus + .slider {
        box-shadow: 0 0 1px #2196F3;
    }

    input:checked + .slider:before {
        -webkit-transform: translateX(26px);
        -ms-transform: translateX(26px);
        transform: translateX(26px);
    }

    /* Rounded sliders */
    .slider.round {
        border-radius: 34px;
    }

    .slider.round:before {
        border-radius: 50%;
    }

    /* Styles for toggle container */
    .toggle-container {
        margin-top: 20px;
    }

    /* Styles for burner state text */
    #burnerStateText {
        font-size: 1.5rem;
    }
  </style>
</head>

<body>
  <h2>Boiler Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="ds-labels">Cuve</span>
    <span id="temperature0">%TEMP_TANK%</span>
    <sup class="units">&deg;C</sup>
 
    <!-- <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> -->
    <span class="ds-labels">Aller</span>
    <span id="temperature1">%TEMP_DIRECT%</span>
    <sup class="units">&deg;C</sup>

    <!-- <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> -->
    <span class="ds-labels">Retour</span>
    <span id="temperature2">%TEMP_BACK%</span>
    <sup class="units">&deg;C</sup>
  </p>

  <!-- Toggle element -->
  <div class="toggle-container">
    <label class="switch">
      <input id="burnerToggle" type="checkbox">
      <span class="slider"></span>
    </label>
    <span id="burnerStateText">Burner State</span>
  </div>

  <h1>Boiler Control</h1>
  <p><a href="/scheduler">Go to Scheduler</a></p>


  <!-- Chart canvase -->
  <canvas id="temperatureChart" width="400" height="200"></canvas>

  <!-- Chart canvases -->
  <!-- <canvas id="temperatureChart0" width="400" height="200"></canvas> -->
  <!-- <canvas id="temperatureChart1" width="400" height="200"></canvas> -->
  <!-- <canvas id="temperatureChart2" width="400" height="200"></canvas> -->


  
  <h1>Boiler Control</h1>
  <form action="/setSchedule" method="POST">
    <label for="startTime">Start Time:</label>
    <input type="time" id="startTime" name="startTime"><br><br>
    <label for="endTime">End Time:</label>
    <input type="time" id="endTime" name="endTime"><br><br>
    <label for="minTemp">Min Temperature:</label>
    <input type="number" id="minTemp" name="minTemp"><br><br>
    <label for="maxTemp">Max Temperature:</label>
    <input type="number" id="maxTemp" name="maxTemp"><br><br>
    <input type="submit" value="Set Schedule">
  </form>

</body>

<script>
  document.addEventListener("DOMContentLoaded", function () {



    // // Function to update the toggle state
    // function updateBurnerState(state) {
    //     var toggle = document.getElementById("burnerToggle");
    //     if (state === "on") {
    //         toggle.checked = true; // Turn on
    //     } else {
    //         toggle.checked = false; // Turn off
    //     }
    // }

    // // Function to update the burner state text
    // function updateBurnerStateText(state) {
    //     var stateText = document.getElementById("burnerStateText");
    //     stateText.textContent = "Burner State: " + state;
    // }


    // -----------------------------------------------------------------------

  var ctx = document.getElementById('temperatureChart').getContext('2d');

  var temperatureChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: [], // Временные метки
      datasets: [
        {
          label: 'Cuve',
          data: [], // Данные для Cuve
          borderColor: 'rgb(255, 99, 132)',
          tension: 0.1
        },
        {
          label: 'Aller',
          data: [], // Данные для Aller
          borderColor: 'rgb(54, 162, 235)',
          tension: 0.1
        },
        {
          label: 'Retour',
          data: [], // Данные для Retour
          borderColor: 'rgb(255, 206, 86)',
          tension: 0.1
        }
      ]
    }
  });

  // Обновление данных графика
  function updateChart(temperatures) {
    var timeLabel = new Date().toLocaleTimeString();

    // Добавление временной метки
    temperatureChart.data.labels.push(timeLabel);

    // Добавление новых данных для каждой линии
    temperatureChart.data.datasets[0].data.push(temperatures[0]); // Cuve
    temperatureChart.data.datasets[1].data.push(temperatures[1]); // Aller
    temperatureChart.data.datasets[2].data.push(temperatures[2]); // Retour

    // Ограничение количества точек на графике
    const maxDataPoints = 50;

    if (temperatureChart.data.labels.length > maxDataPoints) {
      temperatureChart.data.labels.shift();
      temperatureChart.data.datasets.forEach(dataset => dataset.data.shift());
    }

    // Обновление графика
    temperatureChart.update();
  }

  setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var temperatures = this.responseText.split(",").map(parseFloat);
        console.log("temp[]", temperatures);

        // Обновление значений температуры
        document.getElementById("temperature0").innerHTML = temperatures[0];
        document.getElementById("temperature1").innerHTML = temperatures[1];
        document.getElementById("temperature2").innerHTML = temperatures[2];

        // Обновление данных графика
        updateChart(temperatures);
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 10000);

    // -----------------------------------------------------------------------




    });

</script>

</html>

)rawliteral";

#endif


    // // // AJAX запрос для получения новых значений температур
    // // setInterval(function() {
    // //   var xhttp = new XMLHttpRequest();
    // //   xhttp.onreadystatechange = function() {
    // //     if (this.readyState == 4 && this.status == 200) {
    // //       var temperatures = this.responseText.split(",");
    // //       updateCharts(temperatures);
    // //     }
    // //   };
    // //   xhttp.open("GET", "/temperaturec", true);
    // //   xhttp.send();
    // // }, 10000);



    // // var ctx = document.getElementById('temperatureChart').getContext('2d');
    // // var temperatureChart = new Chart(ctx, {
    // //     type: 'line',
    // //     data: {
    // //         labels: [], // Сюда будут добавляться временные метки
    // //         datasets: [{
    // //             label: 'Temperature C0',
    // //             borderColor: 'rgb(255, 99, 132)',
    // //             data: [], // Сюда будут добавляться значения температуры
    // //             fill: false
    // //         }]
    // //     },
    // //     options: {
    // //         responsive: true,
    // //         scales: {
    // //             x: {
    // //                 type: 'time',
    // //                 time: {
    // //                     unit: 'minute'
    // //                 }
    // //             },
    // //             y: {
    // //                 beginAtZero: true
    // //             }
    // //         }
    // //     }
    // // });

    // // function updateChart(newTime, newTemperature) {
    // //   temperatureChart.data.labels.push(newTime);
    // //   temperatureChart.data.datasets[0].data.push(newTemperature);
    // //   temperatureChart.update();
    // // }



    // setInterval(function () {
    //   var xhttp = new XMLHttpRequest();
    //   xhttp.onreadystatechange = function () {
    //     if (this.readyState == 4 && this.status == 200) {
    //       var temperatures = this.responseText.split(",");
    //       console.log("temp[]", temperatures);
    //       document.getElementById("temperature0").innerHTML = temperatures[0];
    //       // updateChart(time, temperatures[0]);
    //       document.getElementById("temperature1").innerHTML = temperatures[1];
    //       document.getElementById("temperature2").innerHTML = temperatures[2];

    //       // document.getElementById("burnerState").innerHTML = temperatures[3];

    //       updateCharts(temperatures);
    //     }
    //   };
    //   xhttp.open("GET", "/temperature", true);
    //   xhttp.send();
    // }, 10000);



    // // // один график для трех температур
    // // document.addEventListener("DOMContentLoaded", function () {
    // //   // Initialize chart with multiple datasets
    // //   var ctx = document.getElementById('temperatureChart').getContext('2d');
    // //   var temperatureChart = new Chart(ctx, {
    // //     type: 'line',
    // //     data: {
    // //       labels: [], // Array for time labels
    // //       datasets: [
    // //         {
    // //           label: 'Temperature Cuve',
    // //           borderColor: 'rgb(255, 99, 132)',
    // //           data: [], // Array for Cuve temperature values
    // //           fill: false
    // //         },
    // //         {
    // //           label: 'Temperature Aller',
    // //           borderColor: 'rgb(54, 162, 235)',
    // //           data: [], // Array for Aller temperature values
    // //           fill: false
    // //         },
    // //         {
    // //           label: 'Temperature Retour',
    // //           borderColor: 'rgb(255, 206, 86)',
    // //           data: [], // Array for Retour temperature values
    // //           fill: false
    // //         }
    // //       ]
    // //     },
    // //     options: {
    // //       responsive: true,
    // //       scales: {
    // //         x: {
    // //           type: 'time',
    // //           time: {
    // //             unit: 'minute'
    // //           }
    // //         },
    // //         y: {
    // //           beginAtZero: true
    // //         }
    // //       }
    // //     }
    // //   });
    // //   // Update function to handle multiple datasets
    // //   function updateChart(newTime, temperatures) {
    // //     temperatureChart.data.labels.push(newTime);
    // //     temperatureChart.data.datasets[0].data.push(temperatures[0]); // Cuve temperature
    // //     temperatureChart.data.datasets[1].data.push(temperatures[1]); // Aller temperature
    // //     temperatureChart.data.datasets[2].data.push(temperatures[2]); // Retour temperature

    // //     // Limit the number of displayed data points
    // //     const maxDataPoints = 50;
    // //     if (temperatureChart.data.labels.length > maxDataPoints) {
    // //       temperatureChart.data.labels.shift();
    // //       temperatureChart.data.datasets.forEach(dataset => {
    // //         dataset.data.shift();
    // //       });
    // //     }

    // //     temperatureChart.update();
    // //   }
    //   // AJAX request to get new temperature values
    //   // setInterval(function () {
    //   //   var xhttp = new XMLHttpRequest();
    //   //   xhttp.onreadystatechange = function () {
    //   //     if (this.readyState == 4 && this.status == 200) {
    //   //       // var response = this.responseText.split(",");
    //   //       // var time = new Date().toLocaleTimeString();
    //   //       // updateChart(time, response);
            
    //   //       var temperatures = response.split(",");
    //   //       var time = new Date().toLocaleTimeString();
    //   //       updateChart(time, temperatures.slice(0, 3)); // Update chart with temperatures
    //   //       var burnerState = temperatures[3]; // Extract burner state from response
    //   //       updateBurnerState(burnerState); // Update toggle
    //   //       updateBurnerStateText(burnerState); // Update burner state text
    //   //     }
    //   //   };
    //   //   xhttp.open("GET", "/temperature", true);
    //   //   xhttp.send();
    //   // }, 10000);