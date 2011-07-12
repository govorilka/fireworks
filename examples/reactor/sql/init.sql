
PRAGMA FOREIGN_KEYS = ON;

PRAGMA ENCODING="UTF-8";

CREATE TABLE IF NOT EXISTS question (
     questionid INTEGER PRIMARY KEY,
     htmltext TEXT,
     parentid INTEGER DEFAULT 0);

CREATE TABLE IF NOT EXISTS folder (
     folderid INTEGER PRIMARY KEY,
     parentid INTEGER DEFAULT 0,
     caption TEXT,
     description TEXT);
