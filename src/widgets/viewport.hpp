/*
 * Created on 01.03.2021 by nefchen.
 */

#ifndef WIDGETS_VIEWPORT_HPP
#define WIDGETS_VIEWPORT_HPP

#include <memory>

#include "views/view.hpp"
#include "widgets/widget.hpp"
#include "widgets/viewport_handle.hpp"


namespace view
{
    struct Viewport: Widget
    {
        Viewport(View* view)
            : Widget(view), m_handle{std::make_shared<ViewportHandle>()}
        {};

        virtual void draw(SDL_Renderer* renderer) override
        {
            m_handle->m_render(renderer);
        };

        std::shared_ptr<ViewportHandle> m_handle{nullptr};
    };
}

#endif  // WIDGETS_VIEWPORT_HPP

