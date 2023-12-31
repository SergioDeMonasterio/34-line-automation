#include "../lib/simple_actuator_control/simple_actuator_control.h"

// -----> In- Out- Pin Definition <-----
// --> Input Pins
const uint8_t tubeFeederSensInPin = inPins[0];
const uint8_t crimper_78_SensInPin = inPins[1];
const uint8_t crimper_34_SensInPin = inPins[2];
const uint8_t machineBreakerInPin = inPins[3];
// --> Output Pins
// Tube Feeder
const uint8_t tubeFeederAct = outPins[0];
const uint8_t tubeLifterAct = outPins[1];
const uint8_t tubeHorMovAct = outPins[2];
// Crimper 7/8"
const uint8_t crimper_78_tipsFeederAct = outPins[3];
// const uint8_t crimper_78_tubeHolderAct = outPins[4];
// const uint8_t crimper_78_tubeInsertAct = outPins[5];
// Crimper 3/4"
const uint8_t crimper_34_tubeCrimpAct = outPins[6];
const uint8_t crimper_34_tubeInsertAct = outPins[7];
const uint8_t crimper_34_tipsFeederAct = outPins[8];
// -----> End: In- Out- Pin Definition <-----
// Automation Units Definition
// -------------------< Tube Feeder >------------------------------------------------------------------------
// 1. Sensor driven operation {true, false, tubeFeederSensInPin, LOW, 200}:
//    a) if tubes available => start the cycle and get the next tube -TRUE
//    b) no need in confirmation if the tube has been removed to finish the cycle - FALSE
//    c) Input pin - tubeFeederSensInPin, sens detects on HIGH, debounce time 200 ms
sensorOperationConfig tubeFeeder_sensConfig = {true, false, tubeFeederSensInPin, LOW, 200};
// 2. actuatorPinsAndTimes:
// {On/Off, BoardPins, startActTimes, stopActTimes}            / ENGLISH           / ESPAÑOL
// {TRUE,   outPin[3], _____________|--------|________________ } Tube feeder arm   / Piston para sacer el tubo de la caja
// {TRUE,   outPin[4], _|-------------------------|___________ } Tube lifter arm   / Piston para para levantar y bajar el tubo
// {TRUE,   outPin[5], _______|----------------------------|__ } Horizontal move   / Piston para mover los tubos a lo largo de la mesa
//                      |     |     |       |     |        |
//                  -------------------------------------------> t, [ms]
//                      1    300   500    1300   1500    2200
const short tubeFeeder_actQty = 3; // Number of actuators
actuatorPinsAndTimes
    tubeFeeder_PinsAndSwitchTimes[tubeFeeder_actQty] = {{true, tubeFeederAct, 500, 1300},     // Tube feeder arm
                                                        {true, tubeLifterAct, 1, 1500},   // Tube lifter arm
                                                        {true, tubeHorMovAct, 300, 2200}}; // Horizontal moving valve
SimpleActuatorControl tubeFeeder = SimpleActuatorControl("Tube Feeder",                     // Unit name
                                                         tubeFeeder_sensConfig,             // Sensor operation configuration
                                                         2500,                              // Total cycle time 3200 ms
                                                         tubeFeeder_actQty,                 // Number of actuators / cylinders
                                                         tubeFeeder_PinsAndSwitchTimes);    // Timing matrix
// -------------------< End of: Tube Feeder  >---------------------------------------------------------------

// -------------------< 7/8" Crimper >------------------------------------------------------------------------
// 1. Sensor driven operation: obj detect- and obj removed confirmation
sensorOperationConfig crimper_78_sensConfig = {true, true, crimper_78_SensInPin, LOW, 250};
// 2. actuatorPinsAndTimes:
// {On/Off, BoardPins, startActTimes, stopActTimes} ENGLISH              / ESPAÑOL
// {TRUE,   outPin[3], _|----------------------|_ } Hold tube actuator   / Piston para aplastar el tubo
// {TRUE,   outPin[4], _______|----------------|_ } Tube Insert actuator / Piston para mover el tubo
// {TRUE,   outPin[5], _____________|----------|_ } PT Feeder actuator   / Piston para insertar el plug tip
//                      |     |     |    |     |
//                  -------------------------------> t, [ms]
//                      1    350   600  1050  1350
const short crimper_78_actQty = 1; // Number of actuators
actuatorPinsAndTimes
    crimper_78_PinsAndSwitchTimes[crimper_78_actQty] = {{true, crimper_78_tipsFeederAct, 1, 600}}; // PT Feeder valve
SimpleActuatorControl crimper_78 = SimpleActuatorControl("7/8 Crimper",                               // Unit name
                                                         crimper_78_sensConfig,                       // Sensor operation configuration
                                                         1400,                                        // Total cycle time
                                                         crimper_78_actQty,                           // Number of actuators / cylinders
                                                         crimper_78_PinsAndSwitchTimes);              // Timing matrix
// -------------------< End of: 7/8" Crimper  >---------------------------------------------------------------

// -------------------< 3/4" Crimper >------------------------------------------------------------------------
unsigned int crimped_34_cnt = 0;
unsigned int crimped_34_cntPrev = 0;
// 1. Sensor driven operation: obj detect- and obj removed confirmation
sensorOperationConfig crimper_34_sensConfig = {true, true, crimper_34_SensInPin, LOW, 250};
const short crimper_34_actQty = 3; // Number of actuators
// 2. actuatorPinsAndTimes:
// {On/Off, BoardPins, startActTimes, stopActTimes}       ENGLISH              / ESPAÑOL
// {TRUE,   outPin[2], _|-------------------|___________ } PT Feeder actuator   / Piston para insertar el plug tip
// {TRUE,   outPin[1], _______|-------------|___________ } Tube Insert actuator / Piston para mover el tubo
// {TRUE,   outPin[0], ___________|-----|_______________ } Crimping actuator    / Piston para remachar
//                      |     |   |     |   |     |
//                  -------------------------------------> t, [ms]
//                      1    350  500  750 850   1150
actuatorPinsAndTimes
    crimper_34_PinsAndSwitchTimes[crimper_34_actQty] = {{true, crimper_34_tipsFeederAct, 1, 850},   // PT Feeder actuator
                                                        {true, crimper_34_tubeInsertAct, 350, 850}, // Tube Insert actuator
                                                        {true, crimper_34_tubeCrimpAct, 500, 750}}; // Crimping actuator
SimpleActuatorControl crimper_34 = SimpleActuatorControl("3/4 Crimper",                              // Unit name
                                                         crimper_34_sensConfig,                      // Sensor operation configuration
                                                         1150,                                       // Total cycle time
                                                         crimper_34_actQty,                          // Number of actuators / cylinders
                                                         crimper_34_PinsAndSwitchTimes);             // Timing matrix

// -------------------< End of: 3/4" Crimper  >---------------------------------------------------------------