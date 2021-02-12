/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef COMM_SIGNALS_HPP
#define COMM_SIGNALS_HPP

#include <functional>
#include <memory>
#include <vector>
#include <map>

#include "types/basic.hpp"


namespace comm
{
    template <typename... Args>
    using Slot = std::function<void(Args...)>;
    using Lifetime = std::shared_ptr<Id>;

    template <typename... Args>
    struct BoundSlot
    {
        std::weak_ptr<Id> m_lifetime;
        Slot<Args...> m_slot;
    };

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

        Signal& operator=(Signal const& other)
        {
            m_dispatcher = other.m_dispatcher;

            return *this;
        };

        Signal(Signal&& other)
        {
            m_dispatcher = other.m_dispatcher;
            m_slot_id = other.m_slot_id;

            m_slots = std::move(other.m_slots);
        };

        Signal& operator=(Signal&& other)
        {
            m_dispatcher = other.m_dispatcher;
            m_slot_id = other.m_slot_id;
            m_slots = std::move(other.m_slots);

            return *this;
        };

        ~Signal() = default;

        inline u32 count_connections() const
        {
            return m_slots.size();
        };

        inline Lifetime connect(Slot<Args...>&& slot)
        {
            auto lifetime{std::make_shared<Id>(++m_slot_id)};

            m_slots.emplace(
                m_slot_id,
                BoundSlot<Args...>{lifetime, std::move(slot)}
            );

            return lifetime;
        }

        inline void disconnect(Lifetime&& slot_lifetime)
        {
            m_slots.erase(*slot_lifetime);
        }

        inline void emit(Args... args)
        {
            if (m_emitting)
            {
                return;
            };

            m_emitting = true;
            for (auto iter{m_slots.begin()}; iter != m_slots.end(); )
            {
                auto& [id, bound_slot]{*iter};

                if (bound_slot.m_lifetime.expired())
                {
                    iter = m_slots.erase(iter);
                    continue;
                }

                auto action = [bs = bound_slot, args...]() {
                    if (!bs.m_lifetime.expired())
                    {
                        bs.m_slot(args...);
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

                ++iter;
            }
            m_emitting = false;
        };

        std::shared_ptr<Dispatcher> m_dispatcher{nullptr};

        private:
            bool m_emitting{false};

            Id m_slot_id{0};
            std::map<Id, BoundSlot<Args...>> m_slots;
    };
}

#endif  // COMM_SIGNALS_HPP

