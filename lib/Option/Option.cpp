/**
 * @file Option.cpp
 * @author Storca (storca@mail.com)
 * @brief Class used to store options
 * @version 0.1
 * @date 2018-11-02
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "Option.h"

/**
 * @brief Construct a new Option<type>:: Option object
 * 
 * @tparam type int, bool, byte...
 * @param name Name of the option, will be used as display name on LCD screen
 * @param is_const Is the option constant
 */
template<class type>
Option<type>::Option(const char *name, bool is_const):
{
    _is_const = is_const;
    _name = name;
    _value = new type();
}

/**
 * @brief Set the value of the option
 * Note : if it is constant, the value will not be changed
 * 
 * @tparam type Type of the option
 * @param newVal New value of the option
 */
template<class type>
void Option<type>::setValue(type newVal)
{
    if(!_is_const)
    {
        *_value = newVal;
    }
}

/**
 * @brief Return the value of an option
 * 
 * @tparam type Type of the option
 * @return type Stored value of the option
 */
template<class type>
type Option<type>::getValue()
{
    return *_value;
}

/**
 * @brief Destroy the Option<type>:: Option object
 * 
 * @tparam type 
 */
template<class type>
Option<type>::~Option()
{
    delete _value;
}