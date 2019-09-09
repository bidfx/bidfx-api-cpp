/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "include/price/price_field.h"

namespace bidfx_public_api::price
{
PriceField PriceField::null = PriceField();

PriceField::PriceField() : type_(PriceField::NOTHING)
{
}

PriceField::PriceField(const PriceField &to_copy) : type_(to_copy.type_)
{
    switch(type_)
    {
        case INTEGER:
            int_value_ = to_copy.int_value_;
            break;
        case LONG:
            long_value_ = to_copy.long_value_;
            break;
        case DOUBLE:
            double_value_ = to_copy.double_value_;
            break;
        case STRING:
            string_value_ = new std::string(*to_copy.string_value_);
            break;
        case NOTHING:
            break;
    }
}

PriceField::PriceField(PriceField &&to_move) : type_(to_move.type_)
{
    switch (type_)
    {
        case INTEGER:
            int_value_ = to_move.int_value_;
            break;
        case LONG:
            long_value_ = to_move.long_value_;
            break;
        case DOUBLE:
            double_value_ = to_move.double_value_;
            break;
        case STRING:
            string_value_ = to_move.string_value_;
            to_move.string_value_ = nullptr;
            break;
        case NOTHING:
            break;
    }
}

PriceField& PriceField::operator=(const PriceField &to_copy)
{
    if (this != &to_copy)
    {
        type_ = to_copy.type_;
        switch (type_)
        {
            case INTEGER:
                int_value_ = to_copy.int_value_;
                break;
            case LONG:
                long_value_ = to_copy.long_value_;
                break;
            case DOUBLE:
                double_value_ = to_copy.double_value_;
                break;
            case STRING:
                string_value_ = new std::string(*to_copy.string_value_);
                break;
            case NOTHING:
                break;
        }
    }
    return *this;
}

PriceField& PriceField::operator=(PriceField &&to_move)
{
    if (this != &to_move)
    {
        if (type_ == STRING)
        {
            delete string_value_;
        }

        type_ = to_move.type_;
        switch (type_)
        {
            case INTEGER:
                int_value_ = to_move.int_value_;
                break;
            case LONG:
                long_value_ = to_move.long_value_;
                break;
            case DOUBLE:
                double_value_ = to_move.double_value_;
                break;
            case STRING:
                string_value_ = to_move.string_value_;
                to_move.string_value_ = nullptr;
                break;
            case NOTHING:
                break;
        }
    }
    return *this;
}

PriceField::~PriceField()
{
    if (type_ == STRING)
    {
        delete string_value_;
    }
}

PriceField::PriceField(double value) : type_(PriceField::DOUBLE)
{
    double_value_ = value;
}

PriceField::PriceField(std::string value) : type_(PriceField::STRING)
{
    string_value_ = new std::string(value);
}

PriceField::PriceField(int32_t value) : type_(PriceField::INTEGER)
{
    int_value_ = value;
}

PriceField::PriceField(int64_t value) : type_(PriceField::LONG)
{
    long_value_ = value;
}

PriceField::Type PriceField::GetType() const
{
    return type_;
}

int32_t PriceField::GetInt() const
{
    if (type_ != INTEGER)
    {
        throw std::logic_error("price field is not of type Integer");
    }
    return int_value_;
}

int64_t PriceField::GetLong() const
{
    if (type_ != LONG)
    {
        throw std::logic_error("price field is not of type Long");
    }
    return long_value_;
}

double PriceField::GetDouble() const
{
    if (type_ != DOUBLE)
    {
        throw std::logic_error("price field is not of type Double");
    }
    return double_value_;
}

std::string PriceField::GetString() const
{
    if (type_ != STRING)
    {
        throw std::logic_error("price field is not of type String");
    }
    return *string_value_;
}

std::ostream& operator<<(std::ostream &os, const PriceField &pf)
{
    switch(pf.type_)
    {
        case PriceField::INTEGER:
            return os << pf.int_value_;
        case PriceField::LONG:
            return os << pf.long_value_;
        case PriceField::DOUBLE:
            return os << pf.double_value_;
        case PriceField::STRING:
            return os << *pf.string_value_;
        case PriceField::NOTHING:
            return os << "null";
    }
}

const bool PriceField::operator==(const PriceField &pf) const
{
    switch(type_)
    {
        case INTEGER:
            switch(pf.type_)
            {
                case INTEGER:
                    return int_value_ == pf.int_value_;
                case LONG:
                    return int_value_ == pf.long_value_;
                case DOUBLE:
                    return int_value_ == pf.double_value_;
                default:
                    return false;
            }
        case LONG:
            switch(pf.type_)
            {
                case INTEGER:
                    return long_value_ == pf.int_value_;
                case LONG:
                    return long_value_ == pf.long_value_;
                case DOUBLE:
                    return long_value_ == pf.double_value_;
                default:
                    return false;
            }
        case DOUBLE:
            switch(pf.type_)
            {
                case INTEGER:
                    return double_value_ == pf.int_value_;
                case LONG:
                    return double_value_ == pf.long_value_;
                case DOUBLE:
                    return double_value_ == pf.double_value_;
                default:
                    return false;
            }
        case STRING:
            switch (pf.type_)
            {
                case STRING:
                    return *string_value_ == *pf.string_value_;
                default:
                    return false;
            }
        case NOTHING:
            return pf.type_ == NOTHING;
    }
}
}