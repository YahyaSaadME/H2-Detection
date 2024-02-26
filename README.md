

Hydrogen Leakage Detection System
By:
Yahya Saad Vanekar 
(RA2311047010025)








Problem Statement
To Predict hydrogen Leakage from gas cylinder and  Leakage of hydrogen from a system of renewable energy generated from hydrogen.

Components Used
•	Arduino Nano (1)
•	MQ-8 hydrogen detection sensor (2)
•	LCD with I2C Module (1)
•	Relay (1)
•	Push button, Buzzer, PCBs, Jumpers, etc.

Circuit Diagrams
Full circuit diagram of Hydrogen Leakage detection system is given in the below fig.
 
In the above fig I have used potentiometers instead of MQ-8 sensor because of none availability of sensors in emulator and instead of Buzzer I have used LED for better visibility.

Advantages
•	There is a problem while detecting H2 gas that it will produce lot of heat which may damage the sensors so, I have used a relay to turn off the sensor when H2 is detected.
•	It can detect H2 gas from both the sensors as if one sensor detected H2 it will won’t affect the other sensor. It will print both if both detected H2.
•	I have given a push button to reset the system no need to turn off and turn on the devices until there is now H2 it will won’t buzz.
•	I have also tackled another problem which is the sensors can detect only in a vacuum space or in a closed space so, I have made a glass container to improve our systems detection efficiency.






Working Principle
The working demo of the model is given below. This will be the output if there is no hydrogen gas.
 
When H2 Detected
If H2 is detected in any one sensor it will print.
 
When H2 Detected on Both
If H2 is detected on both the sensors it will print as given below.
 
As per the demand I have made inside and outside so, it is printed as inside and outside. The code which is used in the project will first caliberate  the sensors and then it will start detecting the values. I have given 100 PPM as minimum PPM value it can be change as well.If you want to try you can use this project which is simulated on Hydrogen Advance - Wokwi ESP32, STM32, Arduino Simulator.
For code of the project you can access this repository given here 



Conclusion
I have solved Hydrogen leakage detection and it will buzz if leakage detected and it can reset dynamically. Both sensors will detect dynamically. Lastly efficiency of the detection has been enhanced by glass container.
