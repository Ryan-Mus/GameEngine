#include "SpriteSheetAnimatorComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TimeSingleton.h"

namespace dae
{
    SpriteSheetAnimator::SpriteSheetAnimator(GameObject* pOwner)
        : Component(pOwner)
        , m_pTextureComponent(nullptr)
        , m_CurrentAnimationName("")
        , m_CurrentAnimationIndex(0)
        , m_CurrentFrameIndex(0)
        , m_FrameTimer(0.0f)
        , m_SpeedMultiplier(1.0f)
        , m_IsPlaying(false)
        , m_Loop(true)
    {
        // Get the texture component from the owner
        m_pTextureComponent = GetOwner()->GetComponent<TextureComponent>();
        if (!m_pTextureComponent)
        {
            std::cerr << "SpriteSheetAnimator requires a TextureComponent on the same GameObject\n";
        }
    }

    void SpriteSheetAnimator::Update()
    {
        if (!m_IsPlaying || !m_pTextureComponent || m_Animations.empty())
            return;

        // Get the current animation
        Animation& currentAnimation = m_Animations[m_CurrentAnimationIndex];
        if (currentAnimation.frames.empty())
            return;

        // Get the current frame
        AnimationFrame& currentFrame = currentAnimation.frames[m_CurrentFrameIndex];

        // Update timer using your Time singleton
        float deltaTime = Time::GetInstance().GetDeltaTime();
        m_FrameTimer += deltaTime * m_SpeedMultiplier;

        // Check if we need to advance to the next frame
        if (m_FrameTimer >= currentFrame.duration)
        {
            m_FrameTimer -= currentFrame.duration;

            // Move to next frame
            m_CurrentFrameIndex++;

            // Check if we reached the end of the animation
            if (m_CurrentFrameIndex >= currentAnimation.frames.size())
            {
                if (currentAnimation.loop)
                {
                    // Loop back to first frame
                    m_CurrentFrameIndex = 0;
                }
                else
                {
                    // Stop at the last frame
                    m_CurrentFrameIndex = currentAnimation.frames.size() - 1;
                    m_IsPlaying = false;
                }
            }

            // Update the texture component with the new frame
            UpdateTextureComponent();
        }
    }

    void SpriteSheetAnimator::AddAnimation(const std::string& name, const std::vector<AnimationFrame>& frames, bool loop)
    {
        // Check if animation with this name already exists
        for (const auto& anim : m_Animations)
        {
            if (anim.name == name)
            {
                std::cerr << "Animation '" << name << "' already exists\n";
                return;
            }
        }

        // Add new animation
        Animation newAnimation;
        newAnimation.name = name;
        newAnimation.frames = frames;
        newAnimation.loop = loop;
        m_Animations.push_back(newAnimation);
    }

    void SpriteSheetAnimator::PlayAnimation(const std::string& name)
    {
        // Find the animation with the given name
        for (size_t i = 0; i < m_Animations.size(); ++i)
        {
            if (m_Animations[i].name == name)
            {
                // If we're already playing this animation, don't restart it
                if (m_IsPlaying && m_CurrentAnimationName == name)
                    return;

                m_CurrentAnimationName = name;
                m_CurrentAnimationIndex = i;
                m_CurrentFrameIndex = 0;
                m_FrameTimer = 0.0f;
                m_IsPlaying = true;
                m_Loop = m_Animations[i].loop;

                // Update texture component with the first frame
                UpdateTextureComponent();
                return;
            }
        }

        std::cerr << "Animation '" << name << "' not found\n";
    }

    void SpriteSheetAnimator::StopAnimation()
    {
        m_IsPlaying = false;
        m_CurrentFrameIndex = 0;
        m_FrameTimer = 0.0f;
    }

    void SpriteSheetAnimator::PauseAnimation()
    {
        m_IsPlaying = false;
    }

    void SpriteSheetAnimator::ResumeAnimation()
    {
        if (!m_CurrentAnimationName.empty())
            m_IsPlaying = true;
    }

	//String comparison not performance friendly
    Animation* SpriteSheetAnimator::FindAnimation(const std::string& name)
    {
        for (auto& anim : m_Animations)
        {
            if (anim.name == name)
                return &anim;
        }
        return nullptr;
    }

    void SpriteSheetAnimator::UpdateTextureComponent()
    {
        if (!m_pTextureComponent || m_Animations.empty() || m_CurrentAnimationIndex >= m_Animations.size())
            return;

        const Animation& currentAnimation = m_Animations[m_CurrentAnimationIndex];
        if (currentAnimation.frames.empty() || m_CurrentFrameIndex >= currentAnimation.frames.size())
            return;

        const AnimationFrame& frame = currentAnimation.frames[m_CurrentFrameIndex];
        m_pTextureComponent->SetSource(frame.x, frame.y, frame.width, frame.height);
    }
}