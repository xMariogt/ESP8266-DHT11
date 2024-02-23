from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
import mysql.connector

app = Flask(__name__)

CORS(app)

# Configuración de la conexión a la base de datos con mysql-connector
db_config = {
    'host': 'localhost',
    'user': 'root',
    'password': 'Mariochava11',
    'database': 'practica3',
}

conn = mysql.connector.connect(**db_config)
cursor = conn.cursor()

# Ruta para renderizar el formulario HTML
@app.route('/')
def index():
    return render_template('./index.html')

# Ruta para manejar la petición AJAX desde el cliente
@app.route('/actualizarTemp', methods=['POST'])
def actualizar_bd():
    try:
        # Obtener el número enviado desde el formulario
        numero = request.json['temperatura']

        # Actualizar el campo en la tabla correspondiente
        update_query = "UPDATE temperatura SET temp = %s WHERE id = 1"
        cursor.execute(update_query, (numero,))
        conn.commit()

        # Respuesta exitosa
        return jsonify({'success': True})

    except Exception as e:
        # Manejar errores
        return jsonify({'success': False, 'error': str(e)})
    
@app.get('/getLimite')
def getTemp():
    try:
        cursor.execute("SELECT temp from temperatura WHERE id = 1")

        temp = cursor.fetchone()

        return jsonify({'success': True, 'value': temp})
    except Exception as e:
        return jsonify({'success': False,'error': str(e) })

if __name__ == '__main__':
    app.run(debug=True)
