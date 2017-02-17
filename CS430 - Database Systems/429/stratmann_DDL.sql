DROP TABLE IF EXISTS craft_type;
CREATE TABLE craft_type(
  id  INT UNSIGNED AUTO_INCREMENT  NOT NULL,
  type  VARCHAR(255)  NOT NULL,
  PRIMARY KEY(id));


DROP TABLE IF EXISTS island;  
CREATE TABLE island(
  id INT UNSIGNED AUTO_INCREMENT NOT NULL,
  name VARCHAR(255) NOT NULL,
  PRIMARY KEY(id));
  

DROP TABLE IF EXISTS craft;
CREATE TABLE craft(
  id   INT UNSIGNED AUTO_INCREMENT    NOT NULL,
  type_id INT UNSIGNED  NOT NULL,
  capacity  INT UNSIGNED,
  PRIMARY KEY(id),
  FOREIGN KEY(type_id) REFERENCES craft_type(id));
  

DROP TABLE IF EXISTS passenger;
CREATE TABLE passenger(
  id   INT UNSIGNED AUTO_INCREMENT    NOT NULL,
  name  VARCHAR(255),
  PRIMARY KEY(id));
  

DROP TABLE IF EXISTS routine_trip;
CREATE TABLE routine_trip(
  id   INT UNSIGNED AUTO_INCREMENT   NOT NULL,
  dept_time   TIME,
  arrival_time  TIME,
  origin    INT UNSIGNED,
  destination   INT UNSIGNED,
  craft_id    INT UNSIGNED,
  cost  FLOAT(4, 2),
  PRIMARY KEY(id),
  FOREIGN KEY(craft_id) REFERENCES craft(id),
  FOREIGN KEY(origin) REFERENCES island(id),
  FOREIGN KEY(destination) REFERENCES island(id));
  
  
DROP TABLE IF EXISTS ticket;
CREATE TABLE ticket(
  id   INT UNSIGNED AUTO_INCREMENT    NOT NULL,
  pass_id   INT UNSIGNED,
  rtrip_id  INT UNSIGNED,
  ticket_date DATE  NOT NULL,
  sell_time TIMESTAMP,
  is_canceled INT DEFAULT 0,
  PRIMARY KEY(id),
  FOREIGN KEY(pass_id) REFERENCES passenger(id),
  FOREIGN KEY(rtrip_id) REFERENCES routine_trip(id));
  

DROP PROCEDURE IF EXISTS get_island_name;
DELIMITER //
CREATE PROCEDURE get_island_name (IN the_id INT UNSIGNED)
BEGIN
  SELECT name
  FROM island
  WHERE island.id = the_id;
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS get_island_id;
DELIMITER //
CREATE PROCEDURE get_island_id (IN the_name VARCHAR(255))
BEGIN
  SELECT island.id
  FROM island
  WHERE island.name = the_name;
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS get_destinations;
DELIMITER //
CREATE PROCEDURE get_destinations (IN island_name VARCHAR(255))
BEGIN
  SELECT name
  FROM island
  JOIN routine_trip ON routine_trip.destination = island.id
  WHERE routine_trip.origin IN (SELECT id FROM island WHERE name = island_name)
  GROUP BY name;
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS get_origins;
DELIMITER //
CREATE PROCEDURE get_origins ()
BEGIN
  SELECT name
  FROM island;
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS get_trips;
DELIMITER //
CREATE PROCEDURE get_trips (IN org VARCHAR(255), IN dest VARCHAR(255))
BEGIN
  SELECT id, dept_time, arrival_time, cost
  FROM routine_trip
  WHERE origin IN (SELECT id FROM island WHERE name = org) AND destination IN 
    (SELECT id FROM island WHERE name = dest);
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS get_trip_capacity;
DELIMITER //
CREATE PROCEDURE get_trip_capacity (IN trip_id INT UNSIGNED)
BEGIN
  SELECT capacity
  FROM craft
  JOIN routine_trip ON routine_trip.craft_id = craft.id
  WHERE routine_trip.id = trip_id;
END //
DELIMITER ;

  
DROP PROCEDURE IF EXISTS get_current_bookings;
DELIMITER //
CREATE PROCEDURE get_current_bookings (IN trip_id INT UNSIGNED, IN the_date DATE)
BEGIN
  SELECT count(*)
  FROM ticket
  WHERE ticket.rtrip_id = trip_id AND ticket_date = the_date AND is_canceled = 0;
END //
DELIMITER ;
  

DROP PROCEDURE IF EXISTS make_reservation;
DELIMITER //
CREATE PROCEDURE make_reservation (IN passenger_id INT UNSIGNED,
  IN trip_id INT UNSIGNED, the_date DATE)
BEGIN
  START TRANSACTION;
  INSERT INTO ticket (id, pass_id, rtrip_id, ticket_date, sell_time, is_canceled)
  VALUES (NULL, passenger_id, trip_id, the_date, NULL, 0);
  
  SELECT id
  FROM ticket
  WHERE pass_id = passenger_id AND rtrip_id = trip_id AND ticket_date = the_date;
  COMMIT;
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS cancel_reservation;
DELIMITER //
CREATE PROCEDURE cancel_reservation (IN ticket_id INT UNSIGNED)
BEGIN
  START TRANSACTION;
  UPDATE ticket
  SET is_canceled = 1
  WHERE ticket.id = ticket_id;
  COMMIT;
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS show_island_count;
DELIMITER //
CREATE PROCEDURE show_island_count ()
BEGIN
  SELECT island.name, count(*), craft_type.type
  FROM island
  JOIN routine_trip ON island.id = routine_trip.destination
  JOIN ticket ON routine_trip.id = ticket.rtrip_id
  JOIN craft ON craft.id = routine_trip.craft_id
  JOIN craft_type ON craft_type.id = craft.type_id
  WHERE is_canceled = 0
  GROUP BY island.name, craft_type.type;
END //
DELIMITER ;


DROP PROCEDURE IF EXISTS get_future_info;
DELIMITER //
CREATE PROCEDURE get_future_info ()
BEGIN
  SELECT ticket.id, passenger.name AS passname, A.name AS orgname,
  B.name AS destname, ticket.ticket_date,dept_time, arrival_time, craft_type.type, cost
  FROM ticket
  JOIN passenger ON ticket.pass_id = passenger.id
  JOIN routine_trip ON ticket.rtrip_id = routine_trip.id
  JOIN island AS A ON A.id = routine_trip.origin
  JOIN island AS B ON B.id = routine_trip.destination
  JOIN craft ON craft.id = routine_trip.craft_id
  JOIN craft_type ON craft_type.id = craft.type_id
  WHERE ticket.ticket_date >= CURDATE() AND is_canceled = 0;
END //
DELIMITER ;  
