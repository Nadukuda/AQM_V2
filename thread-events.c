// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

// Generated events.
#include PLATFORM_HEADER
#include "stack/include/ember-types.h"
#include "hal/hal.h"


void stateEventHandler(void);
extern EmberEventControl stateEventControl;

void emZclOtaBootloadStorageEepromEraseEventHandler(void);
extern EmberEventControl emZclOtaBootloadStorageEepromEraseEventControl;

void emZclEzModeEventHandler(void);
extern EmberEventControl emZclEzModeEventControl;

void emZclOtaBootloadClientEventHandler(void);
extern EmberEventControl emZclOtaBootloadClientEventControl;

//void networkWatchdogEventHandler(void);
//extern EmberEventControl networkWatchdogEventControl;

void resetEventHandler(void);
extern EmberEventControl resetEventControl;

void si7021EventHandler(void);
extern EmberEventControl si7021EventControl;

void CORE_si7021EventHandler(void);
extern EmberEventControl CORE_si7021EventControl;

void vortexAqmEventHandler(void);
extern EmberEventControl vortexAqmEventControl;

void aqmEventHandler(void);
extern EmberEventControl aqmEventControl;

void gasSensorEventHandler(void);
extern EmberEventControl gasSensorEventControl;

void aqmMasterEventHandler(void);
extern EmberEventControl aqmMasterEventControl;

void aqmBufferEventHandler(void);
extern EmberEventControl aqmBufferEventControl;

//void SPS30EventHandler(void);
//extern EmberEventControl SPS30EventControl;


const EmberEventData emAppEvents[] = {
//  {&stateEventControl, stateEventHandler},
//  {&emZclOtaBootloadClientEventControl, emZclOtaBootloadClientEventHandler},
//  {&emZclOtaBootloadStorageEepromEraseEventControl, emZclOtaBootloadStorageEepromEraseEventHandler},
//  {&emZclEzModeEventControl, emZclEzModeEventHandler},
  //  {&networkWatchdogEventControl, networkWatchdogEventHandler},
  {&resetEventControl, resetEventHandler},
  {&si7021EventControl, si7021EventHandler},
  {&CORE_si7021EventControl, CORE_si7021EventHandler},
  {&vortexAqmEventControl, vortexAqmEventHandler},
  {&aqmEventControl, aqmEventHandler},
  {&gasSensorEventControl, gasSensorEventHandler},
  {&aqmMasterEventControl, aqmMasterEventHandler},
  {&aqmBufferEventControl, aqmBufferEventHandler},
  //  {&SPS30EventControl, SPS30EventHandler},
  {NULL, NULL}
};
