#pragma once
#include <stdexcept>
#include <SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);
	
	// ImGui rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().Render();
	
	
	//ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr) {
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float dstX, const float dstY, 
	const int srcX,const int srcY,const int scrWidth,const int scrHeight) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(dstX);
	dst.y = static_cast<int>(dstY);

	SDL_Rect src{};
	src.x = srcX;
	src.y = srcY;
	src.w = scrWidth;
	src.h = scrHeight;

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	dst.w = scrWidth;
	dst.h = scrHeight;
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float dstX, const float dstY,
	const int srcX, const int srcY, const int scrWidth, const int scrHeight,const int scale) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(dstX);
	dst.y = static_cast<int>(dstY);

	SDL_Rect src{};
	src.x = srcX;
	src.y = srcY;
	src.w = scrWidth;
	src.h = scrHeight;

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	dst.w = scale*scrWidth;
	dst.h = scale*scrHeight;
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void dae::Renderer::RenderRectangle(float x, float y, float width, float height, const SDL_Color& color, int lineWidth) const
{
	SDL_Rect rect{ static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
	
	// Save current color
	SDL_Color oldColor{};
	SDL_GetRenderDrawColor(m_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	
	// Set new color
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	
	// Draw rectangle with specified line width
	if (lineWidth == 1)
	{
		SDL_RenderDrawRect(m_renderer, &rect);
	}
	else
	{
		// Draw multiple concentric rectangles for thicker lines
		for (int i = 0; i < lineWidth; ++i)
		{
			SDL_Rect currentRect = {
				rect.x + i,
				rect.y + i,
				rect.w - 2 * i,
				rect.h - 2 * i
			};
			SDL_RenderDrawRect(m_renderer, &currentRect);
		}
	}
	
	// Restore original color
	SDL_SetRenderDrawColor(m_renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

void dae::Renderer::RenderFilledRectangle(float x, float y, float width, float height, const SDL_Color& color) const
{
	SDL_Rect rect{ static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
	
	// Save current color
	SDL_Color oldColor{};
	SDL_GetRenderDrawColor(m_renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	
	// Set new color
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	
	// Draw filled rectangle
	SDL_RenderFillRect(m_renderer, &rect);
	
	// Restore original color
	SDL_SetRenderDrawColor(m_renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
