// Copyright (c) 2024 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "paddle/cinn/hlir/framework/pir/trivial_op_impl.h"

namespace cinn::fusion {

using TrivialOp = cinn::hlir::framework::pir::trivial_fusion_detail::TrivialOp;
using ReduceOp = cinn::hlir::framework::pir::trivial_fusion_detail::ReduceOp;
using FusibleOp = cinn::hlir::framework::pir::trivial_fusion_detail::FusibleOp;

struct FusibleOp2Expr {
  std::vector<ir::Expr> operator()(const TrivialOp& op) {
    return {op.GetFuncBody()};
  }
  std::vector<ir::Expr> operator()(const ReduceOp& op) {
    return {op.GetFuncBody()};
  }
};

struct GetSplitedExprFromFusionOp {
  std::vector<ir::Expr> operator()(const TrivialOp& op) {
    return {op.GetFuncBody()};
  }
  std::vector<ir::Expr> operator()(const ReduceOp& op) {
    const auto& t_r = SplitReduceOp(op);
    return {t_r.first.GetFuncBody(), t_r.second.GetFuncBody()};
  }
};

struct ApplyTransform {
  explicit ApplyTransform(const ir::Expr& expr) : expr_(expr) {}
  ir::Expr operator()(const UnsupportTransformPtr& transform) {
    PADDLE_THROW("Can not do UnsupportTransform");
  }
  ir::Expr operator()(const IdentityTransformPtr& transform) { return expr_; }
  ir::Expr operator()(const AppendDimTransformPtr& transform) {
    PADDLE_THROW("AppendDimTransform not implemented");
  }
  ir::Expr operator()(const DeleteDimTransformPtr& transform) {
    PADDLE_THROW("DeleteDimTransform not implemented");
  }

 private:
  ir::Expr expr_;
};

std::vector<ir::Expr> TopoSort(const std::vector<ir::Expr>& op_exprs);
std::vector<FusibleOp> DoPadding(const FusibleOp& fusion_op,
                                 const std::vector<int>& padding_pos);

}  // namespace cinn::fusion
