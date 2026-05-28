# Maze Navigation and Color Pattern Following Robot using Webots

## Overview

This project presents an autonomous mobile robot developed in the Webots simulation environment using C++. The robot is capable of:

* Navigating a maze autonomously
* Avoiding obstacles using distance sensors
* Detecting colored walls using a camera
* Following a predefined color sequence
* Making navigation decisions without using flood-fill algorithms

The entire robot arena and simulation environment were designed in Webots, while the robot controller and navigation logic were implemented in C++.

---

# Features

## ✅ Autonomous Maze Navigation

The robot continuously explores the maze while analyzing sensor inputs to find open paths and avoid collisions.

## ✅ Obstacle Avoidance

The robot uses front, left, and right distance sensors to detect nearby walls and dynamically adjust its movement.

### Navigation Logic

* Move forward when the path is clear
* Stop and rotate when an obstacle is detected
* Choose the direction with more free space

```cpp id="vtz6d9"
if (frontSensor > THRESHOLD) {
  if (leftSensor < rightSensor) {
    leftMotor->setVelocity(-MAX_SPEED / 2);
    rightMotor->setVelocity(MAX_SPEED / 2);
  } else {
    leftMotor->setVelocity(MAX_SPEED / 2);
    rightMotor->setVelocity(-MAX_SPEED / 2);
  }
}
```

---

## ✅ Color Detection and Pattern Recognition

A camera sensor is integrated into the robot for wall color detection.

The robot:

* Captures RGB image data
* Calculates average color values
* Detects predefined colors
* Follows a specific color order

### Implemented Color Sequence

```cpp id="q6h2dk"
std::vector<std::string> colorPattern = {
  "Red",
  "Yellow",
  "Pink",
  "Brown",
  "Green"
};
```

### Supported Colors

* Red
* Yellow
* Pink
* Brown
* Green

---

## ✅ Real-Time Color Matching

The robot compares camera RGB values against predefined thresholds using tolerance-based matching.

```cpp id="2qjvha"
if (isColorMatch(red, green, blue, 255, 0, 0))
  return "Red";
```

---

## Technologies Used

* Webots Simulator
* C++
* Distance Sensors
* Camera Sensor
* Autonomous Robotics Algorithms

---

# System Components

## Hardware Components (Simulated)

* Differential drive robot
* 2 wheel motors
* 8 proximity sensors
* Camera module

## Software Components

* Sensor processing
* Autonomous navigation logic
* Obstacle avoidance system
* Color detection algorithm
* Pattern-following mechanism

---

# Project Structure

```bash id="m6jv8n"
├── controllers/
│   └── robot_controller/
│       └── main.cpp
│
├── worlds/
│   └── maze_world.wbt
│
├── protos/
│
├── textures/
│
└── README.md
```

---

# Working Principle

## Step 1 — Sensor Monitoring

The robot continuously reads:

* Front sensor
* Left sensor
* Right sensor
* Camera image data

## Step 2 — Obstacle Detection

If the front sensor value exceeds the threshold:

* The robot stops
* Rotates toward the clearer direction

## Step 3 — Color Detection

The camera processes RGB values to identify wall colors.

## Step 4 — Pattern Verification

The detected color is compared with the required sequence.

## Step 5 — Task Completion

Once all colors in the sequence are detected:

* The robot stops
* Simulation ends

---

# Key Algorithms

## Obstacle Avoidance Algorithm

* Reactive wall avoidance
* Direction selection using side sensors

## Color Detection Algorithm

* Average RGB extraction
* Tolerance-based color comparison

## Path Navigation

* Autonomous forward navigation
* Dynamic turning behavior

---

# Future Improvements

* Implement SLAM-based mapping
* Add shortest-path optimization
* Integrate PID motor control
* Use machine learning for path prediction
* Add real-time visualization dashboard

---

# Authors

Developed as a Webots robotics simulation project using C++ for autonomous maze solving and color pattern navigation.
