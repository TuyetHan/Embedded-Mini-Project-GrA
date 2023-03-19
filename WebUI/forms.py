from flask_wtf import FlaskForm
from wtforms import FloatField, SelectField
from wtforms.validators import InputRequired, NumberRange

class DataForm(FlaskForm):
    """
    FlaskForm class to ask the usr for some data to ake more accurate calculations
    about calorie consumption.

    The form has two fields:
        - weight: the user will have to introduce its weight in kg
        - activity: the user will have to choose between 'Walking' and 'Running', which correspond to a specific MET value
    """
    weight = FloatField('Weight (in kg)', validators=[InputRequired(), NumberRange(min=0.1)])
    activity = SelectField('Activity', validators=[InputRequired()], choices=[(3.8, 'Walking'), (9.8, 'Running')])