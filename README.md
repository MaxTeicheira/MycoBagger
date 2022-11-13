# MycoBagger
An open source semi automated bagging system for bulk mushroom substrate and water.
My goal is to lower the cost of mushroom bagging machines for small growers. The output will be lower, but I don't mind. I think I could get 50 bags an hour with this plan.

The arduino will be controlled through serial port, displaying information and updating from user inputs. 

## Hardware
- Arduino
- Solenoid controlled water valve
- Button
- Computer
- HX711 load cell setup with weighing platform
- In Progress:
  - Grain dispenser with 3d printed parts
  - Pellet dispenser with larger 3d printed parts and stronger servo
  - Maybe a better design with pnematics to power gates

## Use
Solenoid valve needs to be setup to dispense liquid into container on the scale. Button should be placed to be able to easily reach to start flow of water and in case of emergency stop. 

The bag for now will be filled with bulk material by hand with assistance from scale and a display that will be made to gauge. The grains can be fed my pvc pipe from hanging bag of grain. Gypsum will be added by hand.

The prompt will tell the user when bulk material is at the correct weight and prompt user to press button to start water.

The prompt will update to tell user current water amount and stop when reached the desired amount.

## TODO
- Automated grain dispensing
- Pellet dispensing
- Gypsum dispensing
- Different gate designs
- Enclosure to protect from liquid
- Better interface for easier use and simplified GUI
  - Add touchscreen to control and display info
