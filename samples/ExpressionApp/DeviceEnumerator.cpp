#include "DeviceEnumerator.h"

HRESULT GetVideoDeviceNames(std::vector<std::wstring>& deviceNames)
{
	HRESULT hr;

	std::shared_ptr<IMFAttributes> pAttributes;
	{
		IMFAttributes* prawAttributes = nullptr;
		hr = MFCreateAttributes(&prawAttributes, 1);
		if (FAILED(hr))
			return hr;

		pAttributes = std::shared_ptr<IMFAttributes>(prawAttributes, [](auto* p) { p->Release(); });
	}

	hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	if (FAILED(hr))
		return hr;

	IMFActivate** pprawDevice = nullptr;
	uint32_t count(0);
	hr = MFEnumDeviceSources(pAttributes.get(), &pprawDevice, &count);
	if (FAILED(hr))
		return hr;

	deviceNames.resize(count);
	for (int i(0); i < count; ++i)
	{
		wchar_t* buffer;
		uint32_t length;
		hr = pprawDevice[i]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &buffer, &length);
		if (FAILED(hr))
			return hr;

		deviceNames[i] = buffer;
	}

	if (pprawDevice != nullptr)
	{
		for (int i(0); i < count; ++i)
			pprawDevice[i]->Release();

		CoTaskMemFree(pprawDevice);
	}

	return S_OK;
}

// Returns a map of id and devices that can be used
HRESULT DeviceEnumerator::getDevicesMap()
{
	std::map<int, Device> deviceMap;

	HRESULT hr;
	hr = MFStartup(MF_VERSION);
	if (FAILED(hr))
		return hr;

	std::vector<std::wstring> deviceNames;
	hr = GetVideoDeviceNames(deviceNames);
	if (FAILED(hr))
		return hr;

	for (const auto& wstr : deviceNames)
		std::wcout << wstr << std::endl;

	hr = MFShutdown();
	if (FAILED(hr))
		return hr;


	return S_OK;
}