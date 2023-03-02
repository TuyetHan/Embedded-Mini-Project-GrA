from flask import Flask
from flask import render_template
from flask import jsonify
from flask import Response

from bt import BTConnect

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/connected')
def connect():

    bt = BTConnect()

    if bt.connect():
        data = bt.receive_data()

    return render_template('connected.html', data=data)

if __name__ == "__main__":
    app.run('0.0.0.0', debug=True)