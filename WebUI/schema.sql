
DROP TABLE IF EXISTS sessions;

CREATE TABLE sessions (
    id INTEGER PRIMARY KEY,
    distance REAL,
    steps INTEGER,
    calories REAL,
    session_time INTEGER,
    saved TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
