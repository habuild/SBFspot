#define VERSION "3.7.1"

// V3.7.1
// Fix makefile - make should copy *ALL* .sql files
// Fix undefined reference to 'boost::system::system_category()' introduced with PR #361
// ==> #403: issue compiling with nosql (undefined reference)
// Fix #409: Invalid timezone specified (updated date_time_zonespec.csv)
// Fix #412: skipping unresponsive inverter
// Fix #417: MQTT publishing issue

/***
*
* Complete modification history can be found at:
* https://github.com/SBFspot/SBFspot/wiki/Modification-History
*
***/