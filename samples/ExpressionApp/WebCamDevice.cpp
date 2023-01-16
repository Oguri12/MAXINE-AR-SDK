#include "WebCamDevice.h"

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")

WebCamDevice::WebCamDevice()
{
	
}

WebCamDevice::~WebCamDevice()
{
	// èàóùÇ»Çµ
}

HRESULT WebCamDevice::getVideoDeviceNames(std::vector<std::wstring>& deviceNames)
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

int WebCamDevice::getDeviceIndex(const std::string& deviceName) {

    int result = 0;

    HRESULT hr;
    hr = MFStartup(MF_VERSION);
    if (FAILED(hr))
        return 0;

    std::vector<std::wstring> deviceNames;
    hr = getVideoDeviceNames(deviceNames);
    if (FAILED(hr))
        return 0;

    for (int i = 0; i < deviceNames.size(); i++) {
        if (_stricmp(convString(deviceNames[i]).c_str(), deviceName.c_str()) == 0) {
            return i;
        }
    }

    hr = MFShutdown();
    if (FAILED(hr))
        return 0;

    return 0;

}

std::string WebCamDevice::convString(const std::wstring& input)
{
    size_t i;
    char* buffer = new char[input.size() * MB_CUR_MAX + 1];
    wcstombs_s(&i, buffer, input.size() * MB_CUR_MAX + 1, input.c_str(), _TRUNCATE);
    std::string result = buffer;
    delete[] buffer;
    return result;
}