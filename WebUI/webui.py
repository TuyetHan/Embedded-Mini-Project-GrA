from flask import Flask
from flask import render_template
from flask import request

from forms import DataForm
from bt import BTConnect
from db import DBAccess

app = Flask(__name__)
app.config['SECRET_KEY'] = 'EMBEDDEDGA'
database = DBAccess()
user_data = {}

@app.route('/')
def index():
    form = DataForm()
    return render_template('index.html', user_data=user_data, form=form)

@app.route('/process_data', methods=['POST'])
def process_data():

    form = DataForm()

    if form.validate_on_submit():
        user_data['weight'] = form.weight.data
        user_data['MET'] = float(form.activity.data)
    else:
        form = DataForm()
        msg = "Introduce valid data"

        return render_template('index.html', user_data=user_data, form=DataForm(), form_msg=msg)

    return render_template('index.html', user_data=user_data, form=form, form_msg='')



@app.route('/connected')
def connect():

    bt = BTConnect()

    if bt.connect():
        data = bt.receive_data()

        database.save_session(data, user_data)

        retrieved_data = [data]

        return render_template('connected.html', data=retrieved_data, header="Your retrieved data:")

    conn_msg = "It was not possible to establish connection. Try again, maybe with your watch closer."
    return render_template('index.html', user_data=user_data, form=DataForm(), form_msg='', conn_msg=conn_msg)


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