# Copyright 2019-2021 Canaan Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# pylint: disable=invalid-name, unused-argument, import-outside-toplevel

import pytest
import onnx
from onnx import helper
from onnx import AttributeProto, TensorProto, GraphProto
from onnx_test_runner import OnnxTestRunner


def _make_module(dtype, mean, scale, seed, shape):
    inputs = []
    outputs = []
    initializers = []
    attributes_dict = {}
    nodes = []

    # input
    input = helper.make_tensor_value_info('input', dtype, shape)
    inputs.append('input')

    # output
    output = helper.make_tensor_value_info('output', dtype, shape)
    outputs.append('output')

    # dtype
    if dtype is not None:
        attributes_dict['dtype'] = dtype

    # mean
    if mean is not None:
        attributes_dict['mean'] = mean

    # scale
    if scale is not None:
        attributes_dict['scale'] = scale

    # seed
    if seed is not None:
        attributes_dict['seed'] = seed

    # shape
    if shape is not None:
        attributes_dict['shape'] = shape

    # RandomNormal node
    rn_output = helper.make_tensor_value_info('rn_output', dtype, shape)
    rn = onnx.helper.make_node(
        'RandomNormal',
        inputs=[],
        outputs=['rn_output'],
        **attributes_dict
    )
    nodes.append(rn)

    # add node
    add = onnx.helper.make_node(
        'Add',
        inputs=['input', 'rn_output'],
        outputs=['output'],
    )
    nodes.append(add)

    # graph
    graph_def = helper.make_graph(
        nodes,
        'test-model',
        [input],
        [output],
        initializer=initializers)

    model_def = helper.make_model(graph_def, producer_name='onnx')

    return model_def

dtypes = [
    TensorProto.FLOAT,
]

means = [
    None,
    1.0,
]

scales = [
    None,
    2.0,
]

seeds = [
    # None will lead to generate different random number and cannot be compared with onnx runtime
    # None,
    1.0,
]

shapes = [
    [1, 3, 16, 16]
]

@pytest.mark.parametrize('dtype', dtypes)
@pytest.mark.parametrize('mean', means)
@pytest.mark.parametrize('scale', scales)
@pytest.mark.parametrize('seed', seeds)
@pytest.mark.parametrize('shape', shapes)
def test_random_normal(dtype, mean, scale, seed, shape, request):
    model_def = _make_module(dtype, mean, scale, seed, shape)

    runner = OnnxTestRunner(request.node.name)
    model_file = runner.from_onnx_helper(model_def)
    runner.run(model_file)

if __name__ == "__main__":
    pytest.main(['-vv', 'test_random_normal.py'])
