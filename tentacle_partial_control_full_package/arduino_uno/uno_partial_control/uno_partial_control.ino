#include <Servo.h>

Servo servoBase, servoShoulder, servoElbow, servoWrist, servoGripper;

const int PIN_BASE = 3;
const int PIN_SHOULDER = 5;
const int PIN_ELBOW = 6;
const int PIN_WRIST = 9;
const int PIN_GRIPPER = 10;

const int PIN_GRIP_SWITCH = 2;
const int PIN_FIXTURE_SENSOR = 4;
const int PIN_ESTOP = 7;

struct Pose { int base; int shoulder; int elbow; int wrist; int gripper; };

Pose HOME             = {90, 90, 90, 90, 20};
Pose TRAY_APPROACH    = {60, 80, 110, 70, 20};
Pose TRAY_PICK        = {60, 85, 115, 72, 20};
Pose TRAY_LIFT        = {60, 70, 95, 75, 55};
Pose FIXTURE_APPROACH = {120, 75, 100, 80, 55};
Pose FIXTURE_INSERT   = {125, 85, 105, 88, 55};
Pose FIXTURE_PULL     = {110, 70, 90, 80, 55};
Pose RECOVERY_APPROACH= {40, 75, 95, 70, 55};
Pose RECOVERY_RELEASE = {40, 80, 100, 72, 20};

int curBase=90, curShoulder=90, curElbow=90, curWrist=90, curGripper=20;
String inputCmd="";

void setup() {
  Serial.begin(115200);
  pinMode(PIN_GRIP_SWITCH, INPUT_PULLUP);
  pinMode(PIN_FIXTURE_SENSOR, INPUT_PULLUP);
  pinMode(PIN_ESTOP, INPUT_PULLUP);

  servoBase.attach(PIN_BASE);
  servoShoulder.attach(PIN_SHOULDER);
  servoElbow.attach(PIN_ELBOW);
  servoWrist.attach(PIN_WRIST);
  servoGripper.attach(PIN_GRIPPER);

  moveToPose(HOME, 10);
  Serial.println("UNO_READY");
}

void loop() {
  if (digitalRead(PIN_ESTOP) == LOW) { safeStop(); return; }
  if (Serial.available()) {
    inputCmd = Serial.readStringUntil('\n');
    inputCmd.trim();
    handleCommand(inputCmd);
  }
}

void handleCommand(String cmd) {
  if (cmd == "HOME") { moveToPose(HOME, 10); Serial.println("OK:HOME"); return; }
  if (cmd == "OPEN_GRIPPER") { moveSingle(curGripper, 20, servoGripper, 8); Serial.println("OK:OPEN_GRIPPER"); return; }
  if (cmd == "CLOSE_GRIPPER") { moveSingle(curGripper, 55, servoGripper, 8); Serial.println(gripDetected() ? "OK:CLOSE_GRIPPER_CONTACT" : "OK:CLOSE_GRIPPER"); return; }
  if (cmd == "STATUS") { printStatus(); return; }
  if (cmd == "DEMO") { runDemoSequence(); return; }

  if (cmd.startsWith("POSE:")) {
    String pose = cmd.substring(5);
    if (pose == "TRAY_APPROACH") { moveToPose(TRAY_APPROACH,10); Serial.println("OK:POSE:TRAY_APPROACH"); return; }
    if (pose == "TRAY_PICK") { moveToPose(TRAY_PICK,10); Serial.println("OK:POSE:TRAY_PICK"); return; }
    if (pose == "TRAY_LIFT") { moveToPose(TRAY_LIFT,10); Serial.println("OK:POSE:TRAY_LIFT"); return; }
    if (pose == "FIXTURE_APPROACH") { moveToPose(FIXTURE_APPROACH,10); Serial.println("OK:POSE:FIXTURE_APPROACH"); return; }
    if (pose == "FIXTURE_INSERT") { moveToPose(FIXTURE_INSERT,10); Serial.println("OK:POSE:FIXTURE_INSERT"); return; }
    if (pose == "FIXTURE_PULL") { moveToPose(FIXTURE_PULL,10); Serial.println("OK:POSE:FIXTURE_PULL"); return; }
    if (pose == "RECOVERY_APPROACH") { moveToPose(RECOVERY_APPROACH,10); Serial.println("OK:POSE:RECOVERY_APPROACH"); return; }
    if (pose == "RECOVERY_RELEASE") { moveToPose(RECOVERY_RELEASE,10); Serial.println("OK:POSE:RECOVERY_RELEASE"); return; }
    Serial.print("FAIL:UNKNOWN_POSE:"); Serial.println(pose); return;
  }

  if (cmd.startsWith("SET:")) { handleSet(cmd); return; }

  Serial.print("FAIL:UNKNOWN_CMD:"); Serial.println(cmd);
}

void handleSet(String cmd) {
  int p1 = cmd.indexOf(':');
  int p2 = cmd.indexOf(':', p1 + 1);
  int p3 = cmd.indexOf(':', p2 + 1);
  if (p1 < 0 || p2 < 0 || p3 < 0) { Serial.println("FAIL:BAD_SET_FORMAT"); return; }

  String joint = cmd.substring(p2 + 1, p3);
  int target = constrain(cmd.substring(p3 + 1).toInt(), 0, 180);

  if (joint == "J1") { moveSingle(curBase, target, servoBase, 8); Serial.println("OK:SET:J1"); return; }
  if (joint == "J2") { moveSingle(curShoulder, target, servoShoulder, 8); Serial.println("OK:SET:J2"); return; }
  if (joint == "J3") { moveSingle(curElbow, target, servoElbow, 8); Serial.println("OK:SET:J3"); return; }
  if (joint == "J4") { moveSingle(curWrist, target, servoWrist, 8); Serial.println("OK:SET:J4"); return; }
  if (joint == "G")  { moveSingle(curGripper, target, servoGripper, 8); Serial.println("OK:SET:G"); return; }
  Serial.print("FAIL:UNKNOWN_JOINT:"); Serial.println(joint);
}

void runDemoSequence() {
  moveToPose(HOME, 10); Serial.println("STEP:HOME");
  moveToPose(TRAY_APPROACH, 10); Serial.println("STEP:TRAY_APPROACH");
  moveToPose(TRAY_PICK, 8); Serial.println("STEP:TRAY_PICK");
  moveSingle(curGripper, 55, servoGripper, 8); Serial.println("STEP:GRIP");
  moveToPose(TRAY_LIFT, 8); Serial.println("STEP:TRAY_LIFT");
  moveToPose(FIXTURE_APPROACH, 10); Serial.println("STEP:FIXTURE_APPROACH");
  moveToPose(FIXTURE_INSERT, 8); Serial.println("STEP:FIXTURE_INSERT");
  moveToPose(FIXTURE_PULL, 8); Serial.println("STEP:FIXTURE_PULL");
  moveToPose(RECOVERY_APPROACH, 10); Serial.println("STEP:RECOVERY_APPROACH");
  moveToPose(RECOVERY_RELEASE, 8); Serial.println("STEP:RECOVERY_RELEASE");
  moveSingle(curGripper, 20, servoGripper, 8); Serial.println("STEP:OPEN");
  moveToPose(HOME, 10); Serial.println("OK:DEMO");
}

void moveToPose(Pose target, int stepDelay) {
  bool moving = true;
  while (moving) {
    moving = false;
    moving |= stepServo(curBase, target.base, servoBase);
    moving |= stepServo(curShoulder, target.shoulder, servoShoulder);
    moving |= stepServo(curElbow, target.elbow, servoElbow);
    moving |= stepServo(curWrist, target.wrist, servoWrist);
    moving |= stepServo(curGripper, target.gripper, servoGripper);
    delay(stepDelay);
    if (digitalRead(PIN_ESTOP) == LOW) return;
  }
}

void moveSingle(int &current, int target, Servo &servoObj, int stepDelay) {
  while (current != target) {
    if (current < target) current++;
    else current--;
    servoObj.write(current);
    delay(stepDelay);
    if (digitalRead(PIN_ESTOP) == LOW) return;
  }
}

bool stepServo(int &current, int target, Servo &servoObj) {
  if (current < target) { current++; servoObj.write(current); return true; }
  if (current > target) { current--; servoObj.write(current); return true; }
  return false;
}

bool gripDetected() { return digitalRead(PIN_GRIP_SWITCH) == LOW; }
bool fixtureDetected() { return digitalRead(PIN_FIXTURE_SENSOR) == LOW; }

void printStatus() {
  Serial.print("STATUS:J1="); Serial.print(curBase);
  Serial.print(",J2="); Serial.print(curShoulder);
  Serial.print(",J3="); Serial.print(curElbow);
  Serial.print(",J4="); Serial.print(curWrist);
  Serial.print(",G="); Serial.print(curGripper);
  Serial.print(",GRIP="); Serial.print(gripDetected() ? "1" : "0");
  Serial.print(",FIXTURE="); Serial.println(fixtureDetected() ? "1" : "0");
}

void safeStop() {
  moveToPose(HOME, 12);
  Serial.println("FAIL:ESTOP");
  delay(500);
}
