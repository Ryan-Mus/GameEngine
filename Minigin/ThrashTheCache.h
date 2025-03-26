#pragma once
#include <mutex>
#include <vector>

#include "Component.h"

namespace dae
{
    class ThrashTheCacheComponent final : public Component
    {
    public:
        ThrashTheCacheComponent(GameObject* pOwner): Component{ pOwner } {}
        void Render() const override;
        void RunBenchmarkEx1();
        void RunBenchmarkEx2(); 
        void RunBenchmarkEx2Alt();

    private:
        std::vector<float> m_TimingsEx1;
        std::vector<float> m_TimingsEx2;
        std::vector<float> m_TimingsEx2Alt;

        int m_NumRunsEx1 = 10;
        int m_NumRunsEx2 = 10; 

        std::mutex m_Mutex; // Bescherm gedeelde data
    };
}

struct tranform3D
{
    float matrix[16] =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

class gameObject3D
{
public:
    int id;
    tranform3D local;
};

class gameObject3DAlt
{
public:
    int id;
    tranform3D* local;
};