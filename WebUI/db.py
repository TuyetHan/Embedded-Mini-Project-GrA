import sqlite3
import threading

class DBAccess:

    #lock = threading.Lock()

    def __init__(self):
        self.connection = sqlite3.connect('sessions.db', check_same_thread=False)

        with open('schema.sql') as f:
            self.connection.executescript(f.read())

        self.cursor = self.connection.cursor()

        self.cursor.execute("INSERT INTO sessions (id, distance, steps, calories, session_time) VALUES (?, ?, ?, ?, ?)",
            (0, 5.5, 10000, 460, 4280)
            )

    def save_session(self, session):

        print("Saving session")
        print(session)
        try:
            self.cursor.execute("INSERT INTO sessions (id, distance, steps, calories, session_time) VALUES (?, ?, ?, ?, ?)",
        (session['id'], session['distance'], session['steps'], session['calories'], session['time'])
        )
        except sqlite3.IntegrityError:
            print("WARNING: Session ID already exists in database! Aborting saving current session.")

        self.connection.commit()

    def get_session(self):

        rows = self.cursor.execute(f"SELECT * FROM sessions ORDER BY id").fetchall()
        print(rows)

        return rows[-1]

    def get_sessions(self):

        rows = self.cursor.execute(f"SELECT * FROM sessions").fetchall()
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

