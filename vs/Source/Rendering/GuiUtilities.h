#pragma once

namespace Tet 
{
	namespace GuiUtilities
	{
		void leaveSpace(unsigned numSlots);
		void renderText(const vec3& xyz, const std::string& title = "", char delimiter = ',');
	}
}
