#pragma once

/// @file
/// Template method implementations for bouncing_ball.h.
/// Most users should only include that file, not this one.
/// For background, see http://drake.mit.edu/cxx_inl.html.

#include "drake/examples/BouncingBall/bouncing_ball.h"

#include "drake/common/drake_assert.h"
#include "drake/systems/framework/basic_vector.h"

namespace drake {
namespace bouncingball {

template <typename T>
BouncingBall<T>::BouncingBall() {
}

template <typename T>
T BouncingBall<T>::EvalGuard(const systems::Context<T>& context) const {
  DRAKE_ASSERT_VOID(systems::System<T>::CheckValidContext(context));

  // Evaluate the guard condition.
  const systems::VectorBase<T>& state =
    context.get_state().continuous_state->get_state();

  // The guard is satisfied (returns a non-positive value) when
  // the ball's position is less than or equal to zero and its
  // velocity is non-positive.
  return std::max(state.GetAtIndex(0), state.GetAtIndex(1));
  //return std::max(this->radius_ , this->radius_);
}

template <typename T>
void BouncingBall<T>::PerformReset(systems::Context<T>* context) const {
  DRAKE_ASSERT(context != nullptr);
  DRAKE_ASSERT_VOID(systems::System<T>::CheckValidContext(*context));

  // Define a pointer for the continuous state in the context.
  const auto result =
    context->get_mutable_state()->continuous_state->get_mutable_state();

  // Perform the reset: map the position to itself and negate the
  // velocity and attenuate by the coefficient of restitution.
  result->SetAtIndex(1,
     -1.0 * this->restitution_coef_ * context->get_mutable_state()->
     continuous_state->get_mutable_state()->GetAtIndex(1));
}

}  // namespace bouncingball
}  // namespace drake
