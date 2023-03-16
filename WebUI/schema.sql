
DROP TABLE IF EXISTS sessions;

CREATE TABLE sessions (
    id INTEGER PRIMARY KEY,
    distance REAL,
    steps INTEGER,
    calories INTEGER,
    session_time INTEGER
);
