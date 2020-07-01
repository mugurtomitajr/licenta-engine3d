//
// Created by Mugur Tomita Jr. on 16/03/2020.
//

#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

template <class DstType, class SrcType>
bool IsType(const SrcType* src)
{
    return dynamic_cast<const DstType*>(src) != nullptr;
}

#endif //ENGINE_UTIL_H
