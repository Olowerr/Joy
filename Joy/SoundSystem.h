#pragma once
#include "Audio.h"

class SoundSystem
{
public:
    static SoundSystem& getInstance()
    {
        static SoundSystem instance;
        return instance;
    }
private:

    SoundSystem()
    {
        DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
        eflags |= DirectX::AudioEngine_Debug;
#endif
        audEngine = std::make_unique<DirectX::AudioEngine>(eflags);

        soundEffect[0] = std::make_unique<DirectX::SoundEffect>(audEngine.get(), L"../Resources/Sound/MenuLevelSound.wav");
        effect[0] = soundEffect[0]->CreateInstance();
        effect[0]->SetVolume(0.2f);

        soundEffect[1] = std::make_unique<DirectX::SoundEffect>(audEngine.get(), L"../Resources/Sound/EasyLevelSound.wav");
        effect[1] = soundEffect[1]->CreateInstance();
        effect[1]->SetVolume(0.2f);
    };

    std::unique_ptr<DirectX::AudioEngine> audEngine;
    std::unique_ptr<DirectX::SoundEffect> soundEffect[2];
    std::unique_ptr<DirectX::SoundEffectInstance> effect[2];

public:

    DirectX::SoundEffectInstance* GetEffect(int idx) // Get looping sounds.
    {
        return effect[idx].get();
    };

    void StopSounds() // Stop all sounds.
    {
        for (size_t i = 0; i < 2; i++)
        {
            effect[i]->Stop();
        }
    };
};