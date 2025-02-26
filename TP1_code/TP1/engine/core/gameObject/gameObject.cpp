


#include "gameObject.h"

// for deletion of unique_ptr in set. https://stackoverflow.com/questions/60220220/efficiently-erase-a-unique-ptr-from-an-unordered-set
template <typename T>
auto erase(std::unordered_set<std::unique_ptr<T>>& set, T* ptr)
{
    std::unique_ptr<T> stale_ptr{ptr};

    auto release = [](std::unique_ptr<T>* p) { p->release(); };
    std::unique_ptr<std::unique_ptr<T>, decltype(release)> release_helper{&stale_ptr, release};

    return set.erase(stale_ptr);
}

template <typename T>
auto extract_ptr(std::unordered_set<std::unique_ptr<T>>& set, T* ptr)
{
    std::unique_ptr<T> stale_ptr{ptr};

    auto release = [](std::unique_ptr<T>* p) { p->release(); };
    std::unique_ptr<std::unique_ptr<T>, decltype(release)> release_helper{&stale_ptr, release};

    return set.extract(stale_ptr);
}

void GameObject::setParent(GameObject & new_parent){
        std::unique_ptr<GameObject> ptr;


        if (parent != nullptr){
            ptr = std::move(
                extract_ptr(parent->children, this)
                );
        } else {
            ptr = std::unique_ptr<GameObject>(this);
        }

        parent = &new_parent;

        new_parent.children.insert(std::move(ptr));
    }