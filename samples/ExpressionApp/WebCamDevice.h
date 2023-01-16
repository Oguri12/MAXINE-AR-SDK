#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <mfapi.h>
#include <mfidl.h>

class WebCamDevice
{
public:
	WebCamDevice();
	~WebCamDevice();

	int getDeviceIndex(const std::string& deviceName);

private:
	HRESULT getVideoDeviceNames(std::vector<std::wstring>& deviceNames);
	std::string convString(const std::wstring& input);
};