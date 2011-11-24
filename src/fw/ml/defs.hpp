#ifndef FIREWORKS_ML_DEFS_HPP
#define FIREWORKS_ML_DEFS_HPP

//Base header for Fireworks Meta Language

#include "fw/defs.hpp"

namespace Fw
{
    namespace ML
    {
        enum Type
        {
            T_Null,
            T_IntNumber,
            T_UIntNumber,
            T_FloatNumber,
            T_DoubleNumber,
            T_String,
            T_Boolean,
            T_Object,
            T_Array
        };

        class Node;
        class IntNumber;
        class UIntNumber;
        class DoubleNumber;
        class Boolean;
        class String;
        class Array;
        class Object;

        template <int type_id> class BaseTempl;
    }
}

#endif // FIREWORKS_ML_DEFS_HPP
