/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <functional>
#include <memory>
#include <vector>
#include <map>

#include "types.hpp"


namespace comm
{
    template <typename... Args>
    using Slot = std::function<void(Args...)>;
    using Lifetime = std::shared_ptr<Id>;

    struct Dispatcher
    {
        void operator()(std::function<void(void)>&& action)
        {
            m_actions.push_back(std::move(action));
        };

        void emit()
        {
            if (m_actions.size() == 0)
            {
                return;
            }

            std::vector<std::function<void(void)>> call_and_discard_actions;
            call_and_discard_actions.swap(m_actions);

            for (auto& action: call_and_discard_actions)
            {
                action();
            }
        };

        private:
           std::vector<std::function<void(void)>> m_actions;
    };

    template <typename... Args>
    struct Signal
    {
        Signal()
        {};

        Signal(std::shared_ptr<Dispatcher> dispatcher)
            : m_dispatcher{dispatcher}
        {};

        Signal(Signal const& other)
        {
            m_dispatcher = other.m_dispatcher;
        };

        Signal(Signal&& other)
        {
            m_dispatcher = other.m_dispatcher;
            m_slot_id = other.m_slot_id;
            m_slots = std::move(other.m_slots);
        };

        Signal& operator=(Signal const& other)
        {
            m_dispatcher = other.m_dispatcher;

            return *this;
        };

        Signal& operator=(Signal&& other)
        {
            m_dispatcher = other.m_dispatcher;
            m_slot_id = other.m_slot_id;
            m_slots = std::move(other.m_slots);

            return *this;
        };

        ~Signal() = default;

        Lifetime connect(Slot<Args...>&& slot)
        {
            Lifetime lifetime{std::make_shared<Id>(m_slot_id++)};

            m_slots.emplace(
                m_slot_id,
                std::make_pair(lifetime, std::move(slot))
            );

            return lifetime;
        }

        void disconnect(Id slot_id)
        {
            m_slots.erase(slot_id);
        }

        void emit(Args... args)
        {
            for (auto& s: m_slots)
            {
                auto action = [lifetime = s.second.first, cback = s.second.second, args...]() {
                    if (!lifetime.expired())
                    {
                        cback(args...);
                    }
                };

                if (m_dispatcher == nullptr)
                {
                    action();
                }
                else
                {
                    (*m_dispatcher)(std::move(action));
                }
            }
        };

        std::shared_ptr<Dispatcher> m_dispatcher{nullptr};

        Id m_slot_id{0};
        std::map<Id, std::pair<std::weak_ptr<Id>, Slot<Args...>>> m_slots;
    };
}

#endif  // SIGNALS_HPP

