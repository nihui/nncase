/* Copyright 2019-2021 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "../module_builder.h"

using namespace nncase;
using namespace nncase::codegen;
using namespace nncase::codegen::stackvm;
using namespace nncase::ir;

void stackvm_module_builder::emit(batch_to_space &node, stackvm_op_builder &builder)
{
    auto &input = allocation(node.input());
    auto &output = allocation(node.output());
    builder.lea_buffer(input);
    builder.lea_buffer(output);

    builder.stshape(0, input.shape);
    builder.stshape(1, input.strides);
    builder.stshape(2, output.strides);
    builder.stshape(3, shape_t { (size_t)node.block_size_h(), (size_t)node.block_size_w() });
    builder.stpaddings(0, std::vector<padding> { padding { node.crop_h()[0], node.crop_h()[1] }, padding { node.crop_w()[0], node.crop_w()[1] } });
    builder.tensor_batch_to_space_(node.input().type(), 0, 1, 2, 3, 0);
}
