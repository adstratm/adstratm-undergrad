CREATE TABLE craft(
  cid   AUTO INT UNSIGNED    NOT NULL,
  type  ENUM('boat', 'helicopter'),
  name  VARCHAR(255)    NOT NULL,
  capacity  INT UNSIGNED,
  PRIMARY KEY(cid));
  

CREATE TABLE passenger(
  pid   AUTO INT UNSIGNED    NOT NULL,
  name  VARCHAR(255)
  PRIMARY KEY(pid));
  
  
CREATE TABLE routine_trip(
  rid   AUTO INT UNSIGNED   NOT NULL,
  dept_time   TIME,
  trip_no   INT UNSIGNED,
  origin    VARCHAR(255),
  destination   VARCHAR(255),
  craft_id    INT UNSIGNED,
  PRIMARY KEY(rid),
  FOREIGN KEY(craft_id) REFERENCES craft);
  
  
CREATE TABLE scheduled_trip(
  sid   AUTO INT UNSIGNED   NOT NULL,
  trip_date   date,
  rtrip_id    INT UNSIGNED,
  PRIMARY KEY(sid),
  FOREIGN KEY(rtrip_id) REFERENCES routine_trip);
  
  
CREATE TABLE ticket(
  tid   AUTO INT UNSIGNED    NOT NULL,
  pass_id   INT UNSIGNED,
  strip_id  INT UNSIGNED,
  cost    FLOAT,
  PRIMARY KEY(tid),
  FOREIGN KEY(pass_id) REFERENCES passenger,
  FOREIGN KEY(strip_id) REFERENCES scheduled_trip);
