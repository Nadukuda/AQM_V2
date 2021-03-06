// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

#ifdef CONFIGURATION_HEADER
  #include CONFIGURATION_HEADER
#endif // CONFIGURATION_HEADER
#include EMBER_AF_API_COAP_DISPATCH

EmberAfCoapDispatchHandler diagnosticAnswerHandler;
EmberAfCoapDispatchHandler emZclHandler;
EmberAfCoapDispatchHandler serverAdvertiseHandler;
EmberAfCoapDispatchHandler VMNMessageHandler;

const EmberAfCoapDispatchEntry emberAfCoapDispatchTable[] = {
  {EMBER_AF_COAP_DISPATCH_METHOD_ANY, "d/da", 4, diagnosticAnswerHandler},
  {EMBER_AF_COAP_DISPATCH_METHOD_ANY, "zcl/", 4, emZclHandler},
  {EMBER_AF_COAP_DISPATCH_METHOD_ANY, ".well-known/", 12, emZclHandler},
  {EMBER_AF_COAP_DISPATCH_METHOD_POST, "server/advertise", 16, serverAdvertiseHandler},
  {EMBER_AF_COAP_DISPATCH_METHOD_POST, "vmn", 3, VMNMessageHandler},
  {0, NULL, 0, NULL}, // terminator
};

//test
