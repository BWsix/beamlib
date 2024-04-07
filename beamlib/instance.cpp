#include "instance.h"

namespace Blib {
std::map<std::string, Instance*> instanceStore;

void Blib::Instance::PushChild(Instance *child) {
    child->transform.setParent(&transform);
    instanceStore.insert({child->name, child});
    children.push_back(child);
}
}
