// Tencent is pleased to support the open source community by making pnnx available.
//
// Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "../pnnx_importer.h"
#include "nncase/importer/util.h"
#include "nncase/ir/ir_types.h"
#include <cassert>
#include <nncase/ir/graph.h>
#include <nncase/ir/op_utils.h>
#include <nncase/ir/ops/binary.h>
#include <nncase/ir/ops/bitcast.h>
#include <nncase/ir/ops/constant.h>
#include <nncase/ir/placeholders.h>
#include <stdexcept>

using namespace nncase;
using namespace nncase::importer;
using namespace nncase::ir;
using namespace pnnx;

void nncase::importer::pnnx_importer::convert_op_Input(const Operator &op)
{
    const auto &op_name = op.name;

    shape_t in_shape; // = op.outputs[0]->shape;
    for (auto v : op.outputs[0]->shape)
        in_shape.push_back(v);

    auto node = graph_.emplace<input_node>(dt_float32, in_shape);
    node->name(op_name + "(Input)");

    output_tensors_.emplace(op.outputs[0]->name, &node->output());
}
