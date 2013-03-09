CREATE TABLE "keyword" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "keyword" TEXT,
    "description" TEXT
);
CREATE TABLE "pool" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "title" TEXT NOT NULL,
    "description" TEXT,
    "author" TEXT,
    "date" INTEGER,
    "type" INTEGER,
    "id_task" INTEGER DEFAILT(0)
);
CREATE TABLE "task" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "id_pool" INTEGER NOT NULL,
    "date" TEXT,
    "title" TEXT,
    "description" TEXT,
    "urgency" INTEGER NOT NULL DEFAULT (3),
    "importance" INTEGER NOT NULL DEFAULT (3),
    "due_date" INTEGER,
    "status" TEXT,
    "task" INTEGER DEFAULT (0),
    "next_check" INTEGER,
    "qs" INTEGER DEFAULT (0),
    "feedback_sent" INTEGER DEFAULT (0),
    "open" INTEGER DEFAULT (1),
    "reopen_count" INTEGER DEFAULT (0),
    "parent" INTEGER DEFAULT (0)
);
CREATE TABLE "task2keyword" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "id_task" INTEGER NOT NULL,
    "id_keyword" INTEGER NOT NULL
);
CREATE TABLE "tasktype" (
    "id" INTEGER PRIMARY KEY NOT NULL,
    "name" TEXT NOT NULL
);
CREATE TABLE "type" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "name" TEXT NOT NULL
);
CREATE VIEW "pool_open" AS
SELECT p.id, p.title, p.author, p.date, t.name
FROM pool p LEFT JOIN type t ON (p.type = t.id)
WHERE p.id_task IS NULL OR p.id_task = 0;

