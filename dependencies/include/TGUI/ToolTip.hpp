/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2023 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_TOOLTIP_HPP
#define TGUI_TOOLTIP_HPP

#include <TGUI/Config.hpp>
#include <TGUI/Vector2.hpp>
#include <TGUI/Duration.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGUI_MODULE_EXPORT namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Interface to set the settings for the tool tips
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API ToolTip
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the time that the mouse has to stand still before the tooltip becomes visible
        ///
        /// @param delay  Time before displaying tooltip
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void setInitialDelay(Duration delay);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the time that the mouse has to stand still before the tooltip becomes visible
        ///
        /// @return Time before displaying tooltip
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD static Duration getInitialDelay();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the default distance between the mouse position and the tool tip
        ///
        /// @param distance  Distance between mouse and tool tip
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void setDistanceToMouse(const Vector2f& distance);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the default distance between the mouse position and the tool tip
        ///
        /// @return Distance between mouse and tool tip
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD static Vector2f getDistanceToMouse();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets whether the tool tip is shown even when the widget is disabled
        ///
        /// @param show  Show the tool tip or not when the mouse is on top of a disabled widget
        ///
        /// Tool tips are shown on disabled widgets by default.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void setShowOnDisabledWidget(bool show);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the tool tip is shown even when the widget is disabled
        ///
        /// @return Would the tool tip be displayed when the mouse is on top of a disabled widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD static bool getShowOnDisabledWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        static Duration m_initialDelay;
        static Vector2f m_distanceToMouse;
        static bool m_showOnDisabledWidget;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TOOLTIP_HPP
