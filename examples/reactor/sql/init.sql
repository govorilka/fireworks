
PRAGMA FOREIGN_KEYS = ON;

PRAGMA ENCODING="UTF-8";

CREATE TABLE IF NOT EXISTS settings (
     settingid INTEGER PRIMARY KEY,
     settingkey TEXT,
     settingvalue TEXT);

CREATE TABLE IF NOT EXISTS questions (
     questionid INTEGER PRIMARY KEY,
     questionindex INTEGER DEFAULT 0,
     htmltext TEXT);
