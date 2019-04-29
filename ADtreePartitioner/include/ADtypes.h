//
// Created by matteo on 29/04/19.
//

#ifndef MESHPARTITIONER_ADTYPES_H
#define MESHPARTITIONER_ADTYPES_H

#include <CGAL/Surface_mesh.h>

struct ADnode
{
    unsigned int id_{};
    unsigned int level_;
    unsigned int value_;

    bool isValid_;
    bool isPropagated_;
    bool isAdopted_;

    std::vector<ADnode*> children_;
    ADnode* parent_;
    std::vector<ADnode*> descendants_;
    std::vector<ADnode*> relatives_;

    ADnode* next_;
    ADnode* prev_;

    explicit ADnode(const unsigned int id) : id_(id),
                                                   level_(0),
                                                   value_(1),
                                                   isValid_(true),
                                                   isPropagated_(false),
                                                   isAdopted_(false),
                                                   parent_(nullptr),
                                                   next_(nullptr),
                                                   prev_(nullptr)
    {
    }
};

enum class CreationMode
{
    Serial,
    Parallel
};

enum class SpanningTreeVariant {
    LtR,
    RtL,
    Alternated,
    Flippant
};

enum class AriadneTreeVariant {
    LtR,
    RtL,
    Alternated,
    Flippant
};

#endif //MESHPARTITIONER_ADTYPES_H
