from flask import Flask
from flask import render_template
from flask import jsonify
from flask import Response

from bt import BTConnect
from db import DBAccess

app = Flask(__name__)
database = DBAccess()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/connected')
def connect():

    bt = BTConnect()

    if bt.connect():
        data = bt.receive_data()

    database.save_session(data)

    retrieved_data = [data]

    return render_template('connected.html', data=retrieved_data, header="Your retrieved data:")

@app.route('/last_session')
def last_session():

    data = database.get_session()

    retrieved_data = []

    retrieved_data.append(database.row_to_dict(data))

    print(retrieved_data)

    return render_template('connected.html', data=retrieved_data, header="Your last session statistics:")

@app.route('/all_sessions')
def all_sessions():

    data = database.get_sessions()

    retrieved_data = []

    for row in data:
        retrieved_data.append(database.row_to_dict(row))

    print(retrieved_data)

    return render_template('connected.html', data=retrieved_data, header="All your sessions:")

if __name__ == "__main__":
    app.run('0.0.0.0', debug=True)