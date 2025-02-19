#pragma once
namespace dae
{
	class GameObject;
	class Component
	{
	public:
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void Render() const {}

		virtual ~Component() {}
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}
		GameObject* GetOwner() const { return m_pOwner; }
	private:
		GameObject* m_pOwner{};
	};
}

