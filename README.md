# Innovation Engineers — Rover Programming Course

A hands-on robotics-programming course for the **ELEGOO Smart Robot Car V4.0**
(TB6612 + MPU6050 mainboard, Arduino UNO). The robot is already built — this
course is entirely about **programming, sensing, calibration, and autonomy**.

A **Day 0 warm-up** lets students drive the rover by hand (TeleOp). Days 1–2
then follow the **Bull City Robotics "Mars Rover Systems Engineering Lab"**
2-day camp (build sessions skipped — our rover is assembled), and Days 3–5
extend it into the engineering that makes autonomy actually work: calibration,
gyros, filtering, sensor fusion, and maze-solving algorithms.

> **Instructors:** see `TEACHING_GUIDE.md` for schedule, talking points, demos,
> common failure modes, solution hints, and checkpoints.

## How the exercises work
Each `ExNN_*/ExNN_*.ino` is a **starter sketch that already compiles**. The
boilerplate (pins, motor helpers) is provided; the learning happens in the
numbered **`TODO` blocks**, which spell out exactly what each function must do.
Students fill those in, upload, and test. Full reference solutions for the
hardest ones live one level up in `~/projects/code/` (`LineFollower`, `Bug1`,
`Bug2`, `MazeSolverLSRB`, `MazeSolverTremaux`).

## Course map

| # | Exercise | Lab / Day | Key idea | New hardware |
|---|----------|-----------|----------|--------------|
| 0 | TeleOp: Drive It Yourself | 0 Warm-up | read input → act; TeleOp vs Auton | IR remote |
| 1 | Motor Basics | 1 Foundations | H-bridge, PWM = speed | — |
| 2 | Drive a Square (open-loop) | 1 | dead-reckoning + its error | — |
| 3 | Line Following (basic) | 1 | first closed-loop behavior | line sensors |
| 4 | Line Following with **PID** | 1 | error → PD control, tuning | — |
| 5 | Ultrasonic Science Lab | 2 Sensors | ping timing, range/beam | ultrasonic |
| 6 | Servo Scan | 2 | servo radar, pick clearest | servo |
| 7 | Obstacle Avoidance | 2 | sense→think→act, thresholds | — |
| 8 | Line Sensor Calibration & Offsets | 3 Calibration | per-sensor min/max, normalize | — |
| 9 | Filtering a Noisy Sensor | 3 | mean vs median vs EMA | — |
| 10 | Reading the Gyro (raw) | 4 Gyro & Fusion | rate, bias, noise | IMU |
| 11 | Gyro Bias Calibration & Integration | 4 | bias removal, drift | — |
| 12 | Complementary Filter (fusion) | 4 | gyro+accel fusion | accel |
| 13 | Closed-Loop Turn-by-Angle | 4 | closed vs open loop | — |
| 14 | Junction Detection | 5 Navigation | 3-sensor capture+nudge | — |
| 15 | Maze Solving — Left-Hand (LSRB) | 5 | turn priority + simplify | — |
| 16 | Maze Solving — Trémaux | 5 | marking, loops, odometry | — |
| 17 | Capstone: Mars Exploration Course | 5 | integrate everything | — |

Day 0 is a short warm-up and can fold into the start of Day 1. Days 1–5 each
fill a camp-style day of four ~60–90 min sessions; cut the stretch goals to run
half-days.

## Calibration constants (set these before expecting good behavior)
These appear as `#define`s at the top of the sketches that use them:

- `MS_PER_CM`, `MS_PER_90_TURN`, `CM_PER_MS` — timing of forward/turn motion.
  Measure with a ruler + stopwatch (Exercise 2 teaches this).
- `LINE_THRESHOLD` / `LINE_IS_HIGH` — line-sensor cutoff and polarity
  (Exercise 8 replaces the guess with real per-sensor calibration).
- `GYRO_SIGN` — flip `+1`↔`-1` if turns/heading go the wrong way.
- `CELL_CM` — maze grid pitch, for the Trémaux solver.

## Uploading
Board = **Arduino UNO**, Port = `/dev/ttyUSB0`. From the IDE, or:
```
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno <exercise-folder>
```
Open the **Serial Monitor / Serial Plotter** at 9600 baud — many exercises
print data meant to be watched there. (Exercise 0 is driven by the kit's IR
remote, not the keyboard.)

## Engineering Design Process
The camp's through-line, used at every step: **Imagine → Build → Test →
Improve.** Test on the real robot, find the weakest part, improve one thing,
retest.
