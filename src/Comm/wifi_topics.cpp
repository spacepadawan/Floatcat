

#include "wifi_topics.h"


 Topic<Telemetry> telemetry(1000,"Telemetry");
 Topic<Telecommand> telecommand(300,"Telecommand");
 Topic<ThrusterCommand> thrusterCommandTopic(342, "ThrusterCommand");
 Topic<ControlCommand> controlCommandTopic(995, "ControlComand");
 Topic<Parameters> parameterTopicIn(801, "ParametersIn");
 Topic<Parameters> parameterTopicOut(800, "ParametersOut");


 HAL_UART gatewayWifi(UART_IDX3);


 #ifdef Wifi_WF121
 WF121 wf(&gatewayWifi);
 #endif

 #ifdef Wifi_ESP8266
 HAL_GPIO gpio0(GPIO_028);
 HAL_GPIO enable(GPIO_046);
 ESP8266 wf(&gatewayWifi,&enable,&gpio0);
 #endif


 LinkinterfaceWifi linkifwf(&wf);
 Gateway gw(&linkifwf);

