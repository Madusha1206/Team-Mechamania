#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Camera.hpp>
#include <unordered_set>
#include <utility>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

using namespace webots;

#define MAX_SPEED 6.28
#define THRESHOLD 80 // Wall proximity threshold
#define GRID_SIZE 0.1
#define TURN_SPEED (0.5 * MAX_SPEED)
#define SHARP_TURN_SPEED (0.8 * MAX_SPEED)


struct PositionHash {
  std::size_t operator()(const std::pair<int, int>& pos) const {
    return std::hash<int>()(pos.first) ^ std::hash<int>()(pos.second);
  }
};


std::string detectColor(Camera *camera) {
  const unsigned char *image = camera->getImage();
  int width = camera->getWidth();
  int height = camera->getHeight();
  int red = 0, green = 0, blue = 0;
  
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      red += camera->imageGetRed(image, width, i, j);
      green += camera->imageGetGreen(image, width, i, j);
      blue += camera->imageGetBlue(image, width, i, j);
    }
  }

  
  int pixelCount = width * height;
  red /= pixelCount;
  green /= pixelCount;
  blue /= pixelCount;
 //std::cout << "RGB: (" << red << ", " << green << ", " << blue << ")" << "isColorMatch"<<std::endl;


 const int tolerance = 50;
  auto isColorMatch = [&](int r, int g, int b, int targetR, int targetG, int targetB) {
    return std::abs(r - targetR) < tolerance &&
           std::abs(g - targetG) < tolerance &&
           std::abs(b - targetB) < tolerance;
  };

  if (isColorMatch(red, green, blue, 255, 0, 0)) return "Red";
  if (isColorMatch(red, green, blue, 255,255,0)) return "Yellow";
  if (isColorMatch(red, green, blue, 255,0,255)) return "Pink";
  if (isColorMatch(red, green, blue, 165,105,30)) return "Brown";
  if (isColorMatch(red, green, blue, 0,255,0)) return "Green";
  return "Unknown";
}

int main() {
  Robot *robot = new Robot();
  int timeStep = (int)robot->getBasicTimeStep();

 
  Motor *leftMotor = robot->getMotor("left wheel motor");
  Motor *rightMotor = robot->getMotor("right wheel motor");
  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);
  leftMotor->setVelocity(0.0);
  rightMotor->setVelocity(0.0);

  
  const char *sensorNames[8] = {"ps0", "ps1", "ps2", "ps3", "ps4", "ps5", "ps6", "ps7"};
  DistanceSensor *sensors[8];
  for (int i = 0; i < 8; ++i) {
    sensors[i] = robot->getDistanceSensor(sensorNames[i]);
    sensors[i]->enable(timeStep);
  }

 
  Camera *camera = robot->getCamera("camera");
  camera->enable(timeStep);
  std::vector<std::string> colorPattern = {"Red", "Yellow" , "Pink", "Brown", "Green"};
  size_t colorIndex = 0;
  std::string lastDetectedColor = "Unknown";
 
  std::unordered_set<std::pair<int, int>, PositionHash> visitedPositions;
  int currentX = 0, currentZ = 0;
  visitedPositions.insert({currentX, currentZ});

  while (robot->step(timeStep) != -1) {
    double frontSensor = sensors[0]->getValue();
    double leftSensor = sensors[5]->getValue();
    double rightSensor = sensors[2]->getValue();

    std::string detectedColor = detectColor(camera);
    
     if (detectedColor == colorPattern[colorIndex]  && detectedColor != lastDetectedColor) {
      std::cout << "Detected: " << detectedColor << std::endl;
      lastDetectedColor = detectedColor;
      colorIndex++;
      if (colorIndex ==colorPattern.size()) {
        leftMotor->setVelocity(0);
        rightMotor->setVelocity(0);
        break;
      }
    }

    if (frontSensor > THRESHOLD) {
    
      if (leftSensor < rightSensor) {
        leftMotor->setVelocity(-MAX_SPEED / 2);
        rightMotor->setVelocity(MAX_SPEED / 2);
      } else {
        leftMotor->setVelocity(MAX_SPEED / 2);
        rightMotor->setVelocity(-MAX_SPEED / 2);
      }
    } else {
     
      leftMotor->setVelocity(MAX_SPEED);
      rightMotor->setVelocity(MAX_SPEED);

      currentX += (int)(std::cos(0) * GRID_SIZE);
      currentZ += (int)(std::sin(0) * GRID_SIZE);

      visitedPositions.insert({currentX, currentZ});
    }
  }

  delete robot;
  return 0;
}