#pragma once

#include <Windows.h>

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <mfapi.h>
#include <mfidl.h>

#pragma comment(lib, "strmiids")

#include <map>

struct Device {
	int id; // This can be used to open the device in OpenCV
	std::string devicePath;
	std::string deviceName; // This can be used to show the devices to the user
};

class DeviceEnumerator {

public:

	DeviceEnumerator() = default;
	HRESULT getDevicesMap();

};
