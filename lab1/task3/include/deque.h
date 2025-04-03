#ifndef FUNDAMENTAL_ALGORITHMS_2_DEQUE_H
#define FUNDAMENTAL_ALGORITHMS_2_DEQUE_H

#include "../../container.h"
#include "../../task2/include/list.h"

namespace my_container {

    template <typename T>
    class Deque : public List<T> {
    public:
        Deque() = default;
        Deque(std::initializer_list<T> init) : List<T>(init) {}
        Deque(const Deque& other) : List<T>(other) {}
        Deque(Deque&& other) noexcept : List<T>(std::move(other)) {}

        Deque& operator=(const Deque& other) {
            List<T>::operator=(other);
            return *this;
        }

        Deque& operator=(Deque&& other) noexcept {
            List<T>::operator=(std::move(other));
            return *this;
        }

        Deque& operator=(std::initializer_list<T> init) {
            this->clear();
            for (const auto& item : init) {
                this->push_back(item);
            }
            return *this;
        }

        T& at(size_t pos) {
            if (pos >= this->size()) throw std::out_of_range("Deque");
            auto it = this->begin();
            std::advance(it, pos);
            return *it;
        }

        const T& at(size_t pos) const {
            if (pos >= this->size()) throw std::out_of_range("Deque index out of range");
            auto it = this->cbegin();
            std::advance(it, pos);
            return *it;
        }

        T& operator[](size_t pos) { return at(pos); }
        const T& operator[](size_t pos) const { return at(pos); }

        using List<T>::operator=;
        using List<T>::operator==;
        using List<T>::operator!=;
        using List<T>::operator<;
        using List<T>::operator<=>;

        using List<T>::front;
        using List<T>::back;

        using List<T>::begin;
        using List<T>::end;
        using List<T>::cbegin;
        using List<T>::cend;
        using List<T>::rbegin;
        using List<T>::rend;
        using List<T>::crbegin;
        using List<T>::crend;

        using List<T>::push_back;
        using List<T>::pop_back;
        using List<T>::push_front;
        using List<T>::pop_front;
        using List<T>::clear;
        using List<T>::resize;
        using List<T>::swap;

        typename List<T>::iterator insert(typename List<T>::iterator pos, const T& value) {
            return List<T>::insert(pos, value);
        }

        typename List<T>::iterator erase(typename List<T>::iterator pos) {
            return List<T>::erase(pos);
        }

        bool operator==(const Deque& other) const { return List<T>::operator==(other); }
        bool operator!=(const Deque& other) const { return List<T>::operator!=(other); }
        bool operator<(const Deque& other) const { return List<T>::operator<(other); }
        auto operator<=>(const Deque& other) const { return List<T>::operator<=>(other); }
    };

}

#endif //FUNDAMENTAL_ALGORITHMS_2_DEQUE_H
