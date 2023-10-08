# smart-parking

Developed an application in C++ using some Arduino UNO boards, ultrasonic sensors and bluetooth modules.

This app was created to overcome the problem of wasting time while looking for a parking space in a crowded place.

When someone wants to enter the parking, it verifies if there are any available parking slots and assigns the customer the closest one to the gate.

We used 3 types of modules consisting of different sensors.

One for entering/exiting the car park. This one uses a servo motor for the barrier control, a screen on which we display the information obtained during the day about the parking and two bluetooth modules with which we make the connection between the client and the application, respectively between the current module and the other modules.

One for the connection between parking spaces and gates. 
It uses two bluetooth modules to communicate with both barriers and other parking spaces. Specifically, it processes the information from all parking spaces and transmits it to the gate.

The last type of module is an intermediate module that provides communication between two other types of modules of the same type.

The whole data transmission process is shown in the powerpoint attached above.
