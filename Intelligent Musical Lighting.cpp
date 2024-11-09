// Intelligent Musical Lighting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <endpointvolume.h>
#include <iostream>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "winmm.lib")

class AudioDetector {
private:
    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioClient* pAudioClient = nullptr;
    IAudioCaptureClient* pCaptureClient = nullptr;
    WAVEFORMATEX* pwfx = nullptr;

public:
    bool Initialize() {
        // Initialize COM
        HRESULT hr = CoInitializeEx(nullptr, COINIT_SPEED_OVER_MEMORY);
        if (FAILED(hr)) return false;

        // Create device enumerator
        hr = CoCreateInstance(
            __uuidof(MMDeviceEnumerator),
            nullptr,
            CLSCTX_ALL,
            __uuidof(IMMDeviceEnumerator),
            (void**)&pEnumerator
        );
        if (FAILED(hr)) return false;

        // Get default audio capture device
        hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
        if (FAILED(hr)) return false;

        // Activate audio client
        hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient);
        if (FAILED(hr)) return false;

        // Get audio format
        hr = pAudioClient->GetMixFormat(&pwfx);
        if (FAILED(hr)) return false;

        // Initialize audio client
        hr = pAudioClient->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_LOOPBACK,
            0,
            0,
            pwfx,
            nullptr
        );
        if (FAILED(hr)) return false;

        // Get capture client
        hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pCaptureClient);
        if (FAILED(hr)) return false;

        return true;
    }

    bool StartCapture() {
        if (!pAudioClient) return false;
        return SUCCEEDED(pAudioClient->Start());
    }
};



int main()
{
    std::cout << "Hello World!\n";

    return 0;
}
