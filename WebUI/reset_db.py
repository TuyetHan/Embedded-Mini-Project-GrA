"""
Script to reset the system database
"""
import sqlite3


connection = sqlite3.connect('sessions.db', check_same_thread=False)

with open('schema.sql') as f:
    connection.executescript(f.read())

cursor = connection.cursor()


# introduce an example session
cursor.execute("INSERT INTO sessions (id, distance, steps, calories, session_time) VALUES (?, ?, ?, ?, ?)",
    (0, 5.5, 10000, 460, 4280)
            )

connection.commit()