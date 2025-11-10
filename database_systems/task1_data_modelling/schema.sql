CREATE DATABASE RoboticsControlLogs;
USE RoboticsControlLogs;

CREATE TABLE Missions (
    mission_id INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    start_date TIMESTAMP,
    end_date TIMESTAMP
);

CREATE TABLE Robots (
    robot_id INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    type VARCHAR(50),
    status VARCHAR(20) DEFAULT 'ACTIVE'
);

CREATE TABLE Commands (
    command_id INT PRIMARY KEY,
    robot_id INT NOT NULL,
    mission_id INT NOT NULL,
    command_text TEXT NOT NULL,
    status VARCHAR(20) DEFAULT 'PENDING',
    priority INT DEFAULT 1,
    created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    FOREIGN KEY (robot_id) REFERENCES Robots(robot_id),
    FOREIGN KEY (mission_id) REFERENCES Missions(mission_id)
);
