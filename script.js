
var mqtt_server = 'ws://localhost:8083/mqtt'; 

// Crear un cliente MQTT utilizando WebSocket
var client = mqtt.connect(mqtt_server);

// Variables para el gráfico
var labels = [];
var temperatureData = [];
var humidityData = [];
var SensacionTermicaData = [];
var temperaturaConfigurada = 0;

document.addEventListener("DOMContentLoaded", function () {
    var menuConfig = document.getElementById("menuConfig");
    var mostrarMenuBtn = document.getElementById("mostrarMenu");
    var cancelBtn = document.getElementById("cancelbtn");
    var okBtn = document.getElementById("okbtn");
    var temperatureInput = document.getElementById("Celsius");

    mostrarMenuBtn.addEventListener("click", function () {
        menuConfig.style.display = "block";
    });

    cancelBtn.addEventListener("click", function () {
        menuConfig.style.display = "none";
    });

    okBtn.addEventListener("click", function () {
        // Guardar el parámetro
        temperaturaConfigurada = parseFloat(temperatureInput.value);
        console.log("Temperatura configurada:", temperaturaConfigurada);
        // Enviar la configuración al servidor Flask
        fetch('http://127.0.0.1:5000/actualizarTemp', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ temperatura: temperaturaConfigurada }),
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                console.log('Configuración exitosa:', data.message);
            } else {
                console.error('Error en la configuración:', data.error);
            }
        })
        .catch(error => {
            console.error('Error en la configuración:', error);
        });
        client.publish('UserTemp', temperaturaConfigurada.toString()); //Publicar el valor en el topico UserTemp
        // Cerrar el menú
        menuConfig.style.display = "none";
    });
});

// Evento de conexión exitosa
client.on('connect', function () {
    console.log('Conectado al MQTT broker via WebSocket');
    
    // Suscribirse al tópico "inTopic" para recibir mensajes
    client.subscribe('inTopic', function (err) {
        if (err) {
            console.log('Subscription facrasada:', err);
        } else {
            console.log('Subscrito al topic "inTopic"');
        }
    });
});

// Manejar los mensajes recibidos
client.on('message', function (topic, message) {
    console.log('Received message on topic', topic, ':', message.toString());

    // Parsear el mensaje JSON recibido
    var data = JSON.parse(message);

    // Agregar datos al gráfico
    labels.push(new Date().toLocaleTimeString());
    temperatureData.push(data.temperature);
    humidityData.push(data.humidity);
    SensacionTermicaData.push(data.feels);

    // Actualizar el gráfico
    updateTemperatureChart();
    updateHumidityChart();
    updateSensacionTermicaChart();
});

// Manejar errores de conexión
client.on('error', function (error) {
    console.log('Connection failed:', error);
});


// Función para actualizar el gráfico
var ctxTemperature = document.getElementById('temperatureChart').getContext('2d');
var temperatureChart = new Chart(ctxTemperature, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [{
            label: 'Temperatura (°C)',
            data: temperatureData,
            fill: false,
            borderColor: 'rgba(255, 99, 132, 1)',
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

// Configurar el gráfico de Chart.js para Humedad
var ctxHumidity = document.getElementById('humidityChart').getContext('2d');
var humidityChart = new Chart(ctxHumidity, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [{
            label: 'Humedad (%)',
            data: humidityData,
            fill: false,
            borderColor: 'rgba(54, 162, 235, 1)',
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

// Configurar el gráfico de Chart.js para Sensacion Termica
var ctxSensacionTermica = document.getElementById('sensacionTermicaChart').getContext('2d');
var sensacionTermicaChart = new Chart(ctxSensacionTermica, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [{
            label: 'Sensacion Termica (°C)',
            data: SensacionTermicaData,
            fill: false,
            borderColor: 'rgba(0, 0, 255, 1)',
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

// Función para actualizar el gráfico de Temperatura
function updateTemperatureChart() {
    temperatureChart.data.labels = labels;
    temperatureChart.data.datasets[0].data = temperatureData;
    temperatureChart.update();
}

// Función para actualizar el gráfico de Humedad
function updateHumidityChart() {
    humidityChart.data.labels = labels;
    humidityChart.data.datasets[0].data = humidityData;
    humidityChart.update();
}

// Función para actualizar el gráfico de Sensacion Termica
function updateSensacionTermicaChart() {
    sensacionTermicaChart.data.labels = labels;
    sensacionTermicaChart.data.datasets[0].data = SensacionTermicaData;
    sensacionTermicaChart.update();
}