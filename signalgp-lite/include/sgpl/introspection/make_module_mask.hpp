#pragma once
#ifndef SGPL_INTROSPECTION_MAKE_MODULE_MASK_HPP_INCLUDE
#define SGPL_INTROSPECTION_MAKE_MODULE_MASK_HPP_INCLUDE

#include <cassert>
#include <vector>

#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

#include "get_module_length.hpp"
#include "get_module_pos.hpp"

namespace sgpl {

template<typename Spec>
std::vector<char> make_module_mask(
  const sgpl::Program<Spec>& program, const size_t module_idx
) {

  const size_t module_pos = sgpl::get_module_pos( program, module_idx );
  const size_t module_length = sgpl::get_module_length( program, module_idx );

  std::vector< char > instructionwise_is_module( module_pos, false );

  const std::vector< char > module( module_length, true );
  instructionwise_is_module.insert(
    std::end(instructionwise_is_module), std::begin(module), std::end(module)
  );

  instructionwise_is_module.resize( program.size(), false );

  assert( instructionwise_is_module.size() == program.size() );

  return instructionwise_is_module;

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_MAKE_MODULE_MASK_HPP_INCLUDE
