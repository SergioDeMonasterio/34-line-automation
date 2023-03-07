#include <Arduino.h>
#include "../lib/board_config/board_config.h"
#include "../lib/board_config/system_constants.h"
#include "../lib/basic_functions/basic_functions.h"
#include "../lib/simple_actuator_control/simple_actuator_control.h"

// #include "../lib/pt78_insert_control/pt78_insert_control.h"
// #include "../lib/tube_feeder_control/tube_feeder_control.h"

// TubeFeederCtrlFSM tubeFeeder = TubeFeederCtrlFSM(inPins[0],   // Sensor de la caja con tubos: Todavia hay tubos en la caja?
//                                                  HIGH,        // / Sensor Detects = HIGH
//                                                  inPins[3],   // Tube feeder breaker!!
//                                                  outPins[0],  // Cilindro que saca tubo por tubo de la caja
//                                                  outPins[1],  // Cilindro que levanta los tubos
//                                                  outPins[2]); // Cilindro largo que mueve los tubos horizontal

// PT78_InsertCtrlFSM
//     pt78_inserter = PT78_InsertCtrlFSM(inPins[1],     // Sensor inductivo donde se insertan los plasticos de 7/8"
//                                LOW,                   // Sensor Detects = LOW
//                                outPins[3],            // Cilindro que remacha / crimper valve pin
//                                outPins[4],            // Cilindro que mueve el tubo / tube insert valve pin
//                                outPins[5],            // Cilindro que inserta los plug tips / plug tip feeder valve pin
//                                true,          // Crimping operation active = true/false
//                                300,           // Sensor confirmation time interval
//                                350,           // Insert valve on delay
//                                250,           // Crimper valve on delay
//                                450,           // Crimper valve off delay
//                                300);          // Insert valve off delay

// -------------------< 3/4" Crimper >------------------------------------------------------------------------
// 1. actuatorPinsAndTimes:
// {On/Off, BoardPins, startActTimes, stopActTimes} ENGLISH           / ESPAÑOL
// {TRUE,   outPin[2], _|----------------------|_ } PT Feeder valve   / Piston para insertar el plug tip
// {TRUE,   outPin[1], _______|----------------|_ } Tube Insert valve / Piston para mover el tubo
// {TRUE,   outPin[0], _____________|----|_______ } Crimping valve    / Piston para remachar
//                      |     |     |    |     |
//                  -------------------------------> t, [ms]
//                      1    550   800  1050  1350
actuatorPinsAndTimes crimper_34_PinsAndSwitchTimes[] = {{true, outPins[8], 1, 1350},     // PT Feeder valve
                                                        {true, outPins[7], 550, 1350},   // Tube Insert valve
                                                        {true, outPins[6], 800, 1050}};  // Crimping valve
SimpleActuatorControl crimper_34 = SimpleActuatorControl("3/4 Crimper",                  // Unit name
                                                         true,                           // Sensor driven operation
                                                         inPins[2],                      // Sensor input pin
                                                         HIGH,                           // Sensor active on HIGH
                                                         250,                            // Detecton debounce time
                                                         1400,                           // Total cycle time
                                                         3,                              // Number of actuators / cylinders
                                                         crimper_34_PinsAndSwitchTimes); // Timing matrix
// -------------------< End of: 3/4" Crimper  >---------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
#if BOARD_CHOICE == 1
  Serial.println("Arduino NANO Board in use");
#elif BOARD_CHOICE == 2
  Serial.println("Arduino MEGA Board in use");
#endif
  configAllPins();
  delay(1000);

  // pt78_inserter.start();
  // tubeFeeder.start();
  crimper_34.start();
}

void loop()
{

  // pt78_inserter.run();
  // tubeFeeder.run();
  crimper_34.run();
  // ------> TESTs <--------
  //// ---> Test 1
  // connect all input pins with the respective output pins
  // for (int i = 2; i < outPinQty; i++)
  // {
  //   connectPins(inPins[i], outPins[i]);
  //   digitalWrite(outPins[i], HIGH);
  // }
  //// ---> End: Test 1
  // // TEST 2: Air Valves
  // if (digitalRead(inPins[1]) == LOW) digitalWrite(outPins[3], FEEDER_LIFT_TUBE);
  // else digitalWrite(outPins[3], FEEDER_DOWN_TUBE);
  // // ----> TETS: Feeder Init Position <-----
  // digitalWrite(outPins[3], FEEDER_ARM_MOVE_IN);
  // digitalWrite(outPins[4], FEEDER_DOWN_TUBE);
  // digitalWrite(outPins[5], FEEDER_HORIZONTAL_INIT_POS);
  // // ----> END TET 2: Feeder Init Position <-----
}
