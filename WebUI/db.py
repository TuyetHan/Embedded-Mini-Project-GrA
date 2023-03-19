import sqlite3
import threading

class DBAccess:
    """Class for system Database access

    An instance of this class allows to:
        - Save a session in the database (save_session)
        - Retrieve all the sessions from database (get_sessions)
        - Retrieve the last session added to database (get_session)

    Attributes:
        connection: sqlite3 connection object
        cursor: sqlite3 cursor object
    """
    def __init__(self):
        self.connection = sqlite3.connect('sessions.db', check_same_thread=False)
        self.cursor = self.connection.cursor()

    def save_session(self, session, user_data):
        # if we have user's data, we calculate calories more accurately
        if user_data:
            session['calories'] = float(session['time']) * user_data['MET'] * 3.5 * float(user_data['weight']) / (200 * 60)
            session['calories'] = round(session['calories'], 2)
        try:
            self.cursor.execute("INSERT INTO sessions (id, distance, steps, calories, session_time) VALUES (?, ?, ?, ?, ?)",
        (session['id'], session['distance'], session['steps'], session['calories'], session['time'])
        )
        except sqlite3.IntegrityError:
            print("WARNING: Session ID already exists in database! Aborting saving current session.")

        self.connection.commit()

    def get_session(self):
        rows = self.cursor.execute(f"SELECT * FROM sessions ORDER BY saved").fetchall()
        print(rows)

        return rows[-1]

    def get_sessions(self):
        rows = self.cursor.execute(f"SELECT * FROM sessions ORDER BY saved").fetchall()
        print(rows)

        return rows

    def row_to_dict(self, data):
        retrieved_data = {}
        retrieved_data['id'] = data[0]
        retrieved_data['distance'] = data[1]
        retrieved_data['steps'] = data[2]
        retrieved_data['calories'] = data[3]
        retrieved_data['time'] = data[4]

        return retrieved_data