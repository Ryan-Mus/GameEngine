#pragma once

class Component
{
public:
	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void Render() const {};

	Component() = default;
	virtual ~Component() {};
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;
private:
};

