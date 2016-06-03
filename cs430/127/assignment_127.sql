CREATE TABLE student (
    name    VARCHAR(255)    NOT NULL,
    student_number    INT ,
    class    int,
    major    VARCHAR(255),
    PRIMARY KEY (student_number));
    
CREATE TABLE course (
    course_name    VARCHAR(255)    NOT NULL,
    course_number    INT,
    credit_hours    INT,
    department    VARCHAR(255),
    PRIMARY KEY (course_number));
    
CREATE TABLE section (
    section_identifier    INT,
    course_number    VARCHAR(255)    NOT NULL,
    semester    VARCHAR(255),
    year    INT,
    instructor    VARCHAR(255),
    PRIMARY KEY (section_identifier),
    FOREIGN KEY (course_number) REFERENCES course(course_number));
    
CREATE TABLE grade_report (
    student_number    INT,
    section_identifier    INT,
    grade    CHAR,
    PRIMARY KEY (student_number, section_identifier),
    FOREIGN KEY (student_number) REFERENCES student(student_number),
    FOREIGN KEY (section_identifier) REFERENCES section(section_identifier));
    
CREATE TABLE prerequisite (
    course_number    INT,
    prerequisite_number    INT,
    PRIMARY KEY (course_number, prerequisite_number),
    FOREIGN KEY (course_number) REFERENCES course(course_number),
    FOREIGN KEY (prerequisite_number) REFERENCES course(course_number));
