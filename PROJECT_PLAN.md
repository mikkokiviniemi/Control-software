# PROJECT PLAN (GROUP 2)

## PROJECT IDEA
Our group is going to build a firmware / control software for the production line. We will receive simulated sensor data
as input and we will translate the data into a human-readable form. Our software will control the production line automatically, with
the possibility to take manual control of each element. We deliver the human-readable sensor and control data to the UI. We will also receive
manual control signals from the UI. We send control data as output to the sensor simulation. 

## IMPLEMENTATION
Please explain how you’re going to implement the project. Please include the technologies, tools, methods etc. that you’re going to use.
Technologies used: C++, Cmake (?), MQTT, json, shared memory (SHM), doctest, Git
The software will be written with C++. Communication with the other parts will be done using shared memory (simulation) and MQTT (UI).
Git will be used for version control and doctest for unit testing.

Project flow and management will be done using GitHub projects. 

## FUNCTIONALITIES
### MVP
- a low-level functional control software that can be run without a screen.
- automatic control system
- data conversion and transfer
- manual mode: toggles for heaters, cooling etc.
- system reporting: failures
- communication with sensors & UI
- unit tests

  ![software flowchart](https://github.com/mikkokiviniemi/Control-software/blob/An-Man-patch-1/d6f0b768-059a-4389-a8e0-07641b3c305a.jpg?raw=true)
  
 ### Possible additional functionalities:
- energy consumption data as output
- optimization
- an SDK library from the controls

## ESTIMATED TIMELINE
- Day 1: project launch, Github project setup, planning the structure of SW, assignment of responsibilities to persons
- Day 2: creating dummy data and data conversion tools, starting working on communication with sensors & UI
- Day 3: working communication, starting automatic control system and manual controls
- Day 4: failure detection, continue building control and communication systems
- Day 5: mini-sprint wrap-up and continue working on MVP
- Day 6: creating mini presentation for rehearsal, if time allows start implementing additional functionalities
- Day 7: presentation rehearsal, MVP done, feedback from presentation 
- Day 8: prioritize additional features, finish MVP if there is something to be done
- Day 9: no new tasks, wrap-up existing tasks, make and rehearse final presentation
- Day 10: showtime!
- Documentation and testing done simultaneously with other tasks.
