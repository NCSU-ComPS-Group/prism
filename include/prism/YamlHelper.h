//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#pragma once
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"

namespace prism
{

template <typename T>
T defaultValue();

template <typename T>
std::string getTypeName();

bool paramProvided(const std::string & param, const YAML::Node & node, const bool required);

template <typename T>
std::vector<T> getParams(const std::string & param, const YAML::Node & node, const bool required);
template <typename T>
T getParam(const std::string & param, const YAML::Node & node, const bool required);

const std::vector<std::string> getExtraParams(const YAML::Node & node,
                                              const std::vector<std::string> & allowed);
}
