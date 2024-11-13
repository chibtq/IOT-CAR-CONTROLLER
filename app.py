from flask import Flask, url_for, render_template, request, redirect, url_for, session, flash, jsonify
import joblib
from request_to_arduino_sender import send_request_to_arduino
from flask_cors import CORS
from pulldatatest import get_data_from_think_speak, get_all_data_from_think_speak
import datetime
from urllib.parse import quote

app = Flask(__name__)
CORS(app)

@app.route('/')
def index():
    return render_template('test.html')

@app.route('/think_speak_data', methods=['GET'])
def fetch_think_speak_data():
    start_date = str(datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(minutes=5))
    end_date = str(datetime.datetime.now(datetime.timezone.utc))

    start_date = '%20'.join(start_date.split(' '))
    end_date = '%20'.join(end_date.split(' '))

    return get_data_from_think_speak(start_date=start_date, end_date=end_date)

@app.route('/all_think_speak_data', methods=['GET'])
def fetch_all_think_speak_data():
    start_date = str(datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(minutes=5))
    end_date = str(datetime.datetime.now(datetime.timezone.utc))

    start_date = '%20'.join(start_date.split(' '))
    end_date = '%20'.join(end_date.split(' '))

    return get_all_data_from_think_speak(start_date=start_date, end_date=end_date)

# @app.route('/controll', methods=['GET', 'POST'])
# def upload_file():    
#     data = request.get_json()
#     try:
#         speech = list()
#         speech.append(data['speech'])

#         # Load vectorizer và mô hình
#         loaded_vectorizer = joblib.load('text_vectorizer.joblib')
#         loaded_model = joblib.load('action_classification.joblib')

#         # Vector hóa dữ liệu mới
#         new_text_vectorized = loaded_vectorizer.transform(speech)

#         # Dự đoán với mô hình đã load
#         predictions = loaded_model.predict(new_text_vectorized)
#         predict_value = predictions[0]
#         print(predictions, predict_value)
        

#         switcher = {
#             1: "forward",
#             2: "left",
#             3: "right",
#             4: "backward",
#             5: "rotate",
#             6: "stop",
#         }

#         controll_require = switcher.get(predict_value, "Unknown command")
#         print(controll_require)
#         send_request_to_arduino(controll_require)

#         return jsonify({"controll_require": controll_require})
#     except:
#         print("Exception")
#         return "Invalid request data", 400
    
@app.route('/controll', methods=['POST'])
def control_car():
    # If the request is JSON, handle it as in your original code
    data = request.get_json()

    try:
        speech = list()
        speech.append(data['speech'])

        # Load vectorizer và mô hình
        loaded_vectorizer = joblib.load('text_vectorizer.joblib')
        loaded_model = joblib.load('action_classification.joblib')

        # Vector hóa dữ liệu mới
        new_text_vectorized = loaded_vectorizer.transform(speech)

        # Dự đoán với mô hình đã load
        predictions = loaded_model.predict(new_text_vectorized)
        predict_value = predictions[0]
        print(predictions, predict_value)

        switcher = {
            1: "forward",
            2: "left",
            3: "right",
            4: "backward",
            5: "rotate",
            6: "stop",
        }

        controll_require = switcher.get(predict_value, "Unknown command")
        print(controll_require)
        send_request_to_arduino(controll_require)

        return jsonify({"controll_require": controll_require})
    except KeyError:
        command = data['command']
        send_request_to_arduino(command=command)

        return jsonify({"command": command})
    
    except:
        print("Exception")
        return "Invalid request data", 400

    return "Invalid request data", 400

if __name__ == "__main__":
    app.run(debug=True, host = '0.0.0.0')