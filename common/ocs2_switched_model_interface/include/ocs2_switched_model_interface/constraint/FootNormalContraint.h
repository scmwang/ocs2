//
// Created by rgrandia on 29.04.20.
//

#pragma once

#include <ocs2_core/automatic_differentiation/CppAdInterface.h>
#include <ocs2_switched_model_interface/constraint/ConstraintTerm.h>

#include <ocs2_switched_model_interface/core/ComModelBase.h>
#include <ocs2_switched_model_interface/core/KinematicsModelBase.h>
#include <ocs2_switched_model_interface/core/SwitchedModel.h>
#include <ocs2_switched_model_interface/foot_planner/FootPhase.h>

namespace switched_model {

class FootNormalConstraint : public ocs2::ConstraintTerm<STATE_DIM, INPUT_DIM> {
  static constexpr size_t domain_dim_ = 1 + STATE_DIM + INPUT_DIM;
  static constexpr size_t range_dim_ = 6;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Base_t = ocs2::ConstraintTerm<STATE_DIM, INPUT_DIM>;
  using ad_interface_t = ocs2::CppAdInterface<scalar_t>;
  using ad_scalar_t = typename ad_interface_t::ad_scalar_t;
  using ad_dynamic_vector_t = typename ad_interface_t::ad_dynamic_vector_t;
  using dynamic_vector_t = typename ad_interface_t::dynamic_vector_t;

  using ad_com_model_t = ComModelBase<ad_scalar_t>;
  using ad_kinematic_model_t = KinematicsModelBase<ad_scalar_t>;

  static constexpr ad_interface_t::ApproximationOrder order_ = ad_interface_t::ApproximationOrder::First;
  static constexpr ocs2::ConstraintOrder constraintOrder_ = ocs2::ConstraintOrder::Linear;

  FootNormalConstraint(int legNumber, FootNormalConstraintMatrix settings, const ad_com_model_t& adComModel,
                       const ad_kinematic_model_t& adKinematicsModel, bool generateModel);

  FootNormalConstraint* clone() const override;

  void configure(const FootNormalConstraintMatrix& settings) { settings_ = settings; };

  size_t getNumConstraints(scalar_t time) const override { return 1; }
  scalar_array_t getValue(scalar_t time, const state_vector_t& state, const input_vector_t& input) const override;
  LinearApproximation_t getLinearApproximation(scalar_t time, const state_vector_t& state, const input_vector_t& input) const override;

 private:
  FootNormalConstraint(const FootNormalConstraint& rhs);

  static void adfunc(const ad_com_model_t& adComModel, const ad_kinematic_model_t& adKinematicsModel, int legNumber,
                     const ad_dynamic_vector_t& tapedInput, ad_dynamic_vector_t& o_footPositionVelocity);

  std::unique_ptr<ad_interface_t> adInterface_;
  FootNormalConstraintMatrix settings_;
};

}  // namespace switched_model
