# Teaching Guide — Innovation Engineers: Rover Programming Course

Instructor companion to the exercises in this folder. Robot: **ELEGOO Smart
Robot Car V4.0** (TB6612 + MPU6050, Arduino UNO), already assembled.

A short **Day 0** warm-up gets students driving (TeleOp). Days 1–2 mirror the
**Bull City Robotics Mars Rover camp** (build sessions omitted — rover is
built); Days 3–5 add the engineering that turns a moving robot into a reliable
autonomous one. Each of Days 1–5 is ~4 sessions of ~60–90 min, matching the
camp's 9:00–3:45 shape; compress to a half-day each by cutting the stretch
goals. Day 0 folds into the start of Day 1 if you're short on time.

---

## How to run an exercise (the loop students repeat each time)
1. **Hook (2–5 min):** demo the goal behavior or the problem it fixes.
2. **Concept (5–10 min):** the one idea in the header comment, on a whiteboard.
3. **Code the TODOs (15–40 min):** students fill the numbered `TODO` blocks. The
   sketch already compiles, so they can upload early and often.
4. **Test on the robot (10–20 min):** the only real verification.
5. **Improve & checkpoint:** meet the success criterion below, then tune/stretch.

**Golden rules**
- Every sketch already compiles — *upload before writing any code* so students
  learn the upload loop on an empty exercise, not while debugging their logic.
- Serial Monitor / **Serial Plotter** at 9600 is your best teaching aid (Ex 5,
  9, 10, 11, 12 are built to be watched there; Ex 0 is driven by the IR remote).
- Put cars on **books/blocks (wheels off the table)** while testing motor logic
  so they don't drive off the bench.
- One variable at a time. "Change `Kp`, retest" beats "change five constants."

---

## Setup checklist (before Day 0)
- [ ] Arduino IDE / `arduino-cli` installed; AVR core present. (Already done on
      this machine.)
- [ ] One car per team, charged 18650s; spare batteries on a charger.
- [ ] Board = **Arduino UNO**, Port = `/dev/ttyUSB0`. Confirm one upload works.
- [ ] The kit's IR remote (with a fresh coin-cell) for Exercise 0; check the
      pin-9 receiver responds.
- [ ] Day 1+: tape a line + (later) a maze on the floor; cones / pool noodles
      for the Mars course; rulers + stopwatches/phones for calibration.
- [ ] Print the Exercise 5 data table and the Day-2 reflection prompts.

---

# DAY 0 — Warm-up: TeleOp

### Exercise 0 — TeleOp: Drive It Yourself
- **Concept:** the control loop in its simplest form — **read an input → decide
  → act**. Frame it with the camp's FTC/FRC connection: matches have a
  driver-controlled **TeleOp** period and a self-driving **Autonomous** period.
  "Today you ARE the autonomy; the rest of the course replaces you."
- **Hook:** let them drive immediately with the kit's **IR remote** —
  `setMotors` and the IR decode are provided, so the only TODO is the
  button→motion mapping. Hold-to-drive (release = stop) is built in. Races /
  obstacle slaloms make a fun first hour.
- **Pitfalls:** the kit ships one of two remote versions — if buttons do
  nothing, uncomment the `Serial.println(code, HEX)` to read the actual codes
  and update the `#defines` (the sketch already accepts both known sets). IR is
  line-of-sight: aim at the receiver dome on pin 9; bright sunlight/fluorescents
  can swamp it.
- **Checkpoint:** every team can drive forward/back and spin both ways on
  command; can explain TeleOp vs Autonomous.

---

# DAY 1 — Foundations & Line Following  ("Every rover moves")
*Mars camp Day 1 Session IV + the first real autonomy. Goal: understand the
motors, meet dead-reckoning, and get the rover following a line by itself.*

### Exercise 1 — Motor Basics
- **Concept:** an H-bridge needs a *direction* bit and a *speed* (PWM 0–255) per
  motor, plus `STBY` to enable. Now they BUILD the `setMotors` they were handed
  in Exercise 0.
- **Pitfalls:** missing `pinMode OUTPUT`, `STBY` left LOW, or A/B swapped (a
  wheel runs backward → flip that motor's boolean).
- **Checkpoint:** demo sequence runs — forward, back, spin both ways, stop.

### Exercise 2 — Drive a Square (open-loop)
- **Concept:** distance and angle become motor-on *time*. No sensors = "open
  loop."
- **The lesson is the failure:** the square won't close. **Mark the start corner
  with tape and measure the gap** after 4 sides. Ask *why* (slip, battery sag,
  one motor stronger). Bank that frustration — Exercise 13 fixes it with the gyro.
- **Pitfalls:** `MS_PER_CM` / `MS_PER_90_TURN` are guesses; teams must calibrate
  by measuring. Low battery changes them.
- **Checkpoint:** rover attempts a recognizable square; team names two reasons
  it drifted.

### Exercise 3 — Line Following (basic)
- **Concept:** first closed-loop behavior — the sensors decide the motors.
  Bang-bang: middle on → straight; a side sees the line → steer that way.
- **Hook:** it follows a taped line! That's the payoff that makes the week feel
  real on Day 1.
- **Pitfalls:** inverted logic (flip the `onLine` comparison / `LINE_IS_HIGH`).
  Sharp curves outrun a slow base speed. It WOBBLES — that's intentional.
- **Checkpoint:** follows a gentle line; students can describe the wobble.

### Exercise 4 — Line Following with PID
- **Concept:** steer **proportionally** to the error and **damp** it with the
  derivative term. **Tune Kp first** (raise until it tracks/just wobbles), **then
  Kd** (raise until the wobble dies). Compare directly against Exercise 3.
- **Pitfalls:** wrong steering sign (negate correction or swap wheels). Too much
  `Kp` = violent oscillation — the teachable moment for `Kd`.
- **Checkpoint:** noticeably smoother than Ex 3 on the same line.
- **Forward link:** uses a fixed threshold for now; Exercise 8 makes the sensing
  robust with calibration — have them return here and re-test after Lab 3.

---

# DAY 2 — Sensor Science & Autonomy  ("Explore Mars")
*Mars camp Day 2, Sessions I–III.*

### Exercise 5 — Ultrasonic Science Lab
- **Concept:** distance = echo time ÷ 58. Real sensors have min/max range, a
  beam *width*, and noise.
- **Run the camp data lab:** target at 10/20/30/40 cm, 10 reads each, fill the
  detection-success table. Then angle/slide the target to find the beam width.
- **Pitfalls:** TRIG/ECHO swapped → always 400. Soft/angled targets read long or
  drop out — that's real, it's the discussion.
- **Checkpoint:** completed data table + one sentence on the sensor's blind spots.

### Exercise 6 — Servo Scan
- **Concept:** pan the sensor to make a 1-pixel radar; pick the most open angle.
- **Pitfalls:** reading the sensor *before* the servo settles (must `delay`
  after `write`). Keep to ~30–150°.
- **Checkpoint:** prints a sensible sweep and reports the clearest direction.

### Exercise 7 — Autonomous Obstacle Avoidance
- **Concept:** the **sense → think → act** loop governed by the `STOP_DISTANCE`
  *threshold* ("modify speed, turning behavior, and sensor thresholds").
- **Pitfalls:** spinning forever between two walls (add the back-up stretch);
  turning the wrong way (swap the branch).
- **Checkpoint:** crosses a short field of cones without touching them — the
  Mars course in miniature.

---

# DAY 3 — Calibration & Filtering
*Why "it worked yesterday" breaks today: offsets, drift, and noise — and fixes.*

### Exercise 8 — Line Sensor Calibration & Offsets
- **Concept:** three "identical" sensors read different numbers (gain + offset
  vary; light and ride height shift them). Learn each one's min/max, then
  `map()` every reading onto a common 0–1000 scale.
- **Demo the problem first:** print raw L/M/R over the same surface — they
  differ. *That's* why a fixed threshold (Ex 3/4) is fragile.
- **Pitfalls:** seeding `minVal=1023, maxVal=0` (not 0/0); not sweeping over both
  line and floor during the window.
- **Checkpoint:** normalized values agree over the same surface though raw ones
  don't. **Call back:** re-run Exercise 4's PID with calibrated input.

### Exercise 9 — Filtering a Noisy Sensor
- **Concept:** never act on one raw reading. **Median** kills spikes (best for
  sonar); **moving average** smooths but lags; **EMA** is a one-line low-pass
  whose `alpha` trades smoothness vs responsiveness.
- **Use the Serial Plotter:** raw + 3 filters at once; wave a hand to inject
  spikes and watch median ignore them.
- **Pitfalls:** EMA starting at 0 (seed it); median sort indexing.
- **Checkpoint:** student says which filter they'd trust for obstacle distance
  and what it costs (latency). **Bridge:** the EMA is the math the complementary
  filter uses tomorrow.

---

# DAY 4 — Gyro, Drift & Sensor Fusion
*The heart of the course. Build heading you can trust, then use it.*

### Exercise 10 — Reading the Gyro (raw)
- **Concept:** a gyro measures **rate** (deg/s), not angle. Raw ÷131 = deg/s.
  Held still it reads *near* but not *at* zero — that offset is **bias**; the
  jitter is **noise**.
- **Pitfalls:** "MPU6050 NOT found" → I2C wiring / wrong board revision (this is
  the MPU6050 board; a QMI8658C board needs a different driver).
- **Checkpoint:** student points to the bias and the noise on the monitor.

### Exercise 11 — Gyro Bias Calibration & Integration
- **Concept:** subtract the averaged bias, then **integrate** corrected rate ×
  real `dt` to get heading. Even tiny leftover bias → **drift**.
- **Test:** calibrate, rotate by hand 90°, check the number; set it down and
  watch heading creep — drift made visible.
- **Pitfalls:** moving during calibration; using a fixed `delay` as `dt` instead
  of `micros()`; no deadband lets noise integrate.
- **Checkpoint:** hand-rotating ~90° prints ~90 (±a few); team explains drift.

### Exercise 12 — Complementary Filter (Sensor Fusion)
- **Concept:** the **headline fusion lesson.** Gyro: smooth, fast, drifts.
  Accel: absolute (gravity), noisy. Blend `angle = A·(angle+gyro·dt) +
  (1−A)·accelAngle`, `A≈0.98`.
- **Crucial caveat:** we fuse **pitch** (tilt) because gravity is an absolute
  reference. **Yaw/heading has none** — which is exactly why Exercise 11's
  heading can't be fully fixed by fusion and instead needs closed-loop correction
  (Ex 13) or a magnetometer. Students should leave knowing *what fusion can and
  can't fix.*
- **Serial Plotter:** gyro-only (drifts) vs fused (holds).
- **Checkpoint:** fused pitch returns to true after motion; gyro-only doesn't.

### Exercise 13 — Closed-Loop Turn-by-Angle
- **Concept:** **closed loop** — keep turning until the *measured* angle says
  stop, vs Exercise 2's open-loop guess. Re-run the square with `turnAngle(90)`.
- **Test:** same tape-corner gap as Ex 2 — it should shrink a lot. Make the
  before/after a showcase artifact.
- **Pitfalls:** overshoot at high speed (slow near target / P-control stretch);
  `GYRO_SIGN` / spin direction mismatched → flip it.
- **Checkpoint:** turns land within a few degrees; square closes much tighter
  than Day 1.

---

# DAY 5 — Navigation Algorithms & Capstone

### Exercise 14 — Junction Detection
- **Concept:** 3 sensors can't see a whole intersection, so **capture exits when
  an outer sensor first goes dark, then nudge forward and re-read** to test
  "straight" and detect the goal square.
- **Pitfalls:** confusing a curve with a junction (a real-hardware limit; a 5–8
  sensor array fixes it); `CM_PER_MS` off → nudge wrong distance.
- **Checkpoint:** correctly names L / S / R / dead-end / cross / GOAL.

### Exercise 15 — Maze Solving: Left-Hand Rule (LSRB)
- **Maze build (electrical tape):** a **tape-WALL** maze, not a line maze. Lay
  black electrical-tape strips as **walls laid ACROSS the corridors** on a light
  floor; the rover drives the lanes between them. Corridors should be wide enough
  for the body (**~18–20 cm**) and end in walls / turns / T-junctions. Mark the
  **finish** with a solid tape **pad** the rover drives onto (tape stays under all
  three sensors, unlike a thin wall line). Aim the rover down the first corridor
  to start. A small maze of turns and dead-ends is plenty for a demo.
- **How it drives (no line-following, no cells):** drive **straight until all
  three sensors hit tape** = a wall ahead — sensor-driven, *any* distance, no
  blind fixed advance. Then **decide**.
- **Concept:** at each wall pick the leftmost open way; record turns; collapse
  each dead-end `B` via the rotation table. The simplified list is the shortest
  path. Loop-free mazes only. Key subtlety to draw out: the rover **only stops
  when straight is blocked**, so `gStraight` is always false here and the priority
  collapses to **Left > Right > Back** — open straights are simply driven through
  (it can't see a side turn it isn't facing). `pickLSRB` is still written the full
  L>S>R>B way so the algorithm matches a line maze.
- **Why it pivots to "glance":** the 3 sensors point straight down at the front,
  so a side opening is invisible while driving. At a wall it pivots (the
  closed-loop **gyro** turn from Ex 13) to glance left, then right, creeping a
  little (`creepClear`) to feel open-vs-walled → fills `gLeft/gRight`. This makes
  Ex 13 (turn-by-angle) and Ex 11 (gyro bias) pay off again.
- **Two TODOs:** `pickLSRB`, `simplifyPath` (same as before — the lesson is the
  algorithm, not the plumbing).
- **Pitfalls:** simplify firing when `path[len-2]!='B'`; off-by-one in the
  3-turn collapse. **Hardware pitfalls (provided layer):** turns must be square —
  hold still during the power-up gyro bias calibration; fix `TAPE_THRESHOLD`/
  `TAPE_READS_HIGH` from Ex 8 so tape reads "on"; set `PROBE_CM` long enough to
  see into a passage but shorter than a corridor. On long straights the rover can
  veer (it's driving open-loop straight) and clip a side wall before reaching the
  end wall — the easy fix is gyro heading-hold during `driveToWall()`.
- **Checkpoint:** solves a simple tape-wall maze; printed path gets *shorter* as
  dead ends are eliminated.
- **Reference solution:** `~/projects/code/MazeSolverLSRB/` (full two-pass:
  explore then replay, with button start).

### Exercise 16 — Maze Solving: Trémaux
- **Concept:** mark passages; never take a 2-mark passage; use marks to escape
  **loops** (where LSRB fails). Needs the gyro to label junctions by position.
- **One TODO:** the decision block — the hardest reasoning of the course. Walk
  the three cases on a whiteboard with a looped example.
- **Honest caveat:** correctness rides on odometry; on this chassis it's a
  faithful *demonstration*. Tune `GYRO_SIGN`, `CM_PER_MS`, `CELL_CM`. Two
  junctions collapsing to one coordinate = odometry drift, the lesson itself.
- **Checkpoint:** solves a small maze *with a loop* that traps the LSRB solver.
- **Reference solution:** `~/projects/code/MazeSolverTremaux/`.

### Exercise 17 — Capstone: Mars Exploration Course
- **Concept:** integration, not new theory. Four mission phases (Leave Base →
  Cross Terrain → Avoid Obstacles → Reach Sample Zone) each reuse an earlier
  exercise. Build the field with tape, cones, and pool noodles.
- **Run the design process:** complete the course, find the weakest phase,
  **improve one thing**, retest, record before/after.
- **Checkpoint:** completes all four objectives; team names what they improved
  and the measured effect.

---

## Showcase & assessment (Mars camp "Rover Design Review")
Each team presents: biggest challenge, biggest success, most important lesson,
next improvement. Strong evidence:
- a **measured** before/after (square gap Ex 2→Ex 13; wobble Ex 3→Ex 4; spikes
  Ex 9),
- a tuned constant they can justify (`STOP_DISTANCE`, `Kp`, `A`, `GYRO_SIGN`),
- a clear explanation of **drift** and why calibration/fusion/closed-loop matter.

## Quick troubleshooting
| Symptom | Likely cause |
|---|---|
| Nothing moves | `pinMode` missing, `STBY` LOW, or dead battery |
| One wheel backward | swap that motor's direction boolean |
| Turns the wrong way / spins forever | flip `GYRO_SIGN` |
| Line logic inverted / chases off | flip the `onLine` comparison / `LINE_IS_HIGH` |
| Ultrasonic always 400 | TRIG/ECHO swapped, or no echo (too far/soft) |
| "MPU6050 NOT found" | I2C wiring, or a non-MPU6050 board revision |
| Junctions misread | recalibrate `CM_PER_MS`; tracking too fast/loose |
| TeleOp remote does nothing | wrong remote code set (print codes & update `#defines`); aim at the pin-9 receiver; weak IR in bright light |
| Behavior changed for no reason | **battery voltage dropped** — recharge, recalibrate |

## Pacing
Full camp = Day 0 warm-up + the five days as written. Half-days = core TODOs
only, skip stretch goals. Advanced/older students: fold Day 0 into Day 1,
combine Days 1–2, and spend the extra time on Trémaux (Ex 16) and capstone
polish.
