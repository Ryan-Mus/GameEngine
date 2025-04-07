#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include <iostream>

namespace dae
{
    class TextureComponent;

    struct AnimationFrame
    {
        int x;
        int y;
        int width;
        int height;
        float duration; // Duration in seconds
    };

    struct Animation
    {
        std::string name;
        std::vector<AnimationFrame> frames;
        bool loop = true;
    };

    class SpriteSheetAnimator final : public Component
    {
    public:
        explicit SpriteSheetAnimator(GameObject* pOwner);
        virtual ~SpriteSheetAnimator() = default;

        virtual void Update() override;

        // Animation control methods
        void AddAnimation(const std::string& name, const std::vector<AnimationFrame>& frames, bool loop = true);
        void PlayAnimation(const std::string& name);
        void StopAnimation();
        void PauseAnimation();
        void ResumeAnimation();
        bool IsPlaying() const { return m_IsPlaying; }
        const std::string& GetCurrentAnimationName() const { return m_CurrentAnimationName; }

        // Set animation speed multiplier (1.0f is normal speed)
        void SetSpeedMultiplier(float multiplier) { m_SpeedMultiplier = multiplier; }
        float GetSpeedMultiplier() const { return m_SpeedMultiplier; }

    private:
        std::vector<Animation> m_Animations;
        TextureComponent* m_pTextureComponent;

        std::string m_CurrentAnimationName;
        size_t m_CurrentAnimationIndex;
        size_t m_CurrentFrameIndex;

        float m_FrameTimer;
        float m_SpeedMultiplier;

        bool m_IsPlaying;
        bool m_Loop;

        // Helper methods
        Animation* FindAnimation(const std::string& name);
        void UpdateTextureComponent();
    };
}

