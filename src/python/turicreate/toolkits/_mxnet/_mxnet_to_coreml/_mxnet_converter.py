# -*- coding: utf-8 -*-
# Copyright © 2017 Apple Inc. All rights reserved.
#
# Use of this source code is governed by a BSD-3-clause license that can
# be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
from __future__ import print_function as _
from __future__ import absolute_import as _
from __future__ import division as _
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

from . import _layers
import coremltools as _coremltools
import coremltools.models.datatypes as _datatypes
from coremltools.models import neural_network as _neural_network
from coremltools.proto import FeatureTypes_pb2 as _FeatureTypes_pb2
from coremltools.models import MLModel as _MLModel

import json as _json
import numpy as _np

_MXNET_LAYER_REGISTRY  = {
    'FullyConnected' : _layers.convert_dense,
    'Activation'     : _layers.convert_activation,
    'LeakyReLU'      : _layers.convert_leaky_relu,
    'SoftmaxOutput'  : _layers.convert_softmax,
    'Convolution'    : _layers.convert_convolution,
    'Pad'            : _layers.convert_padding,
    'Pooling'        : _layers.convert_pooling,
    'Flatten'        : _layers.convert_flatten,
    'transpose'      : _layers.convert_transpose,
    'Concat'         : _layers.convert_concat,
    'BatchNorm'      : _layers.convert_batchnorm,
    'elemwise_add'   : _layers.convert_elementwise_add,
    '_add'           : _layers.convert_elementwise_add,
    'elemwise_mul'   : _layers.convert_elementwise_mul,
    '_mul'           : _layers.convert_elementwise_mul,
    '_mul_scalar'    : _layers.convert_elementwise_mul_scalar,
    '_div_scalar'    : _layers.convert_elementwise_div_scalar,
    'Reshape'        : _layers.convert_reshape,
    'Deconvolution'  : _layers.convert_deconvolution,
    'Custom'         : _layers.convert_custom,
    'slice_axis'     : _layers.convert_slice_axis,
    'softmax'        : _layers.convert_softmax,
    'sigmoid'        : _layers.convert_sigmoid,
    'InstanceNorm'   : _layers.convert_instancenorm,
    'Embedding'      : _layers.convert_embedding,
    '_plus_scalar'   : _layers.convert_scalar_add,
    '_ones'          : _layers.convert_skip,
    '_zeros'         : _layers.convert_skip,
    'UpSampling'     : _layers.convert_upsample,
}

_MXNET_SKIP_LAYERS = [
    '_MulScalar',
    'Dropout',
]

def _mxnet_remove_batch(input_data):
    for blob in input_data:
        input_data[blob] = _np.reshape(input_data[blob], input_data[blob].shape[1:])
    return input_data

def check_error(model, path, shapes, output = 'softmax_output', verbose = True):
    """
    Check the difference between predictions from MXNet and CoreML.
    """
    import mxnet as _mxnet
    coreml_model = _coremltools.models.MLModel(path)
    input_data = {}
    input_data_copy = {}
    for ip in shapes:
        input_data[ip] = _np.random.rand(*shapes[ip]).astype('f')
        input_data_copy[ip] = _np.copy(input_data[ip])

    dataIter = _mxnet.io.NDArrayIter(input_data_copy)
    mx_out = model.predict(dataIter).flatten()

    e_out_dict = coreml_model.predict(_mxnet_remove_batch(input_data))
    e_out = e_out_dict[output].flatten()
    error = _np.linalg.norm(e_out - mx_out)

    if verbose:
        print("First few predictions from CoreML : %s" % e_out[0:10])
        print("First few predictions from MXNet  : %s" % e_out[0:10])
        print("L2 Error on random data %s" % error)
    return error

def _set_input_output_layers(builder, input_names, output_names):
    input_layers_indices = []
    output_layers_indices = []
    layers = builder.spec.neuralNetwork.layers
    for idx, l in enumerate(layers):
        if set(input_names).intersection(l.input):
            input_layers_indices.append(idx)
        if set(output_names).intersection(l.output):
            output_layers_indices.append(idx)

    builder.input_layers_indices = input_layers_indices
    builder.output_layers_indices = output_layers_indices
    builder.input_layers_is1d = [False for _ in input_names]
    builder.output_layers_is1d = [False for _ in output_names]

def _get_layer_converter_fn(layer):
    """Get the right converter function for MXNet
    """
    if layer in _MXNET_LAYER_REGISTRY:
        return _MXNET_LAYER_REGISTRY[layer]
    else:
        raise TypeError("MXNet layer of type %s is not supported." % layer)


def convert(model, input_shape, class_labels=None, mode=None,
            preprocessor_args=None, builder=None, verbose=True):
    """Convert an MXNet model to the protobuf spec.

    Parameters
    ----------
    model: MXNet model
        A trained MXNet neural network model.

    input_shape: list of tuples
        A list of (name, shape) tuples, defining the input names and their
        shapes. The list also serves to define the desired order of the inputs.

    class_labels: A string or list of strings.
        As a string it represents the name of the file which contains the classification labels (one per line).
        As a list of strings it represents a list of categories that map the index of the output of a neural network to labels in a classifier.

    mode: str ('classifier', 'regressor' or None)
        Mode of the converted coreml model.
        When mode = 'classifier', a NeuralNetworkClassifier spec will be constructed.
        When mode = 'regressor', a NeuralNetworkRegressor spec will be constructed.

    builder: `NeuralNetworkBuilder`
        If `None`, a builder will be created internally. This also means the
        builder will not be finalized and returned as an `MLModel`.
        Post-processing arguments will be ignored and class labels will not be
        integrated. This option is meant for advanced users.

    verbose: bool
        Print exported layers.

    **kwargs :
        Provide keyword arguments for:
        - input shapes. Supplied as a dictionary object with keyword "input_shape".
        - pre-processing arguments: Supplied as a dictionary object with keyword "preprocessor_args". The parameters in the dictionary
            tell the converted coreml model how to pre-process any input before an inference is run on it.
            For the list of pre-processing arguments see
            http://pythonhosted.org/coremltools/generated/coremltools.models.neural_network.html#coremltools.models.neural_network.NeuralNetworkBuilder.set_pre_processing_parameters

    Returns
    -------
    model: A coreml model.
    """
    if not isinstance(input_shape, list):
         raise TypeError("Must provide a list for input shape. e.g input_shape=[('data', (3,224,224))]")

    def remove_batch(dim):
        return dim[1:]

    input_names, input_dims = zip(*input_shape)
    input_dims = list(map(remove_batch, input_dims))

    net = model.symbol

    # Infer shapes and store in a dictionary
    shapes = net.infer_shape(**dict(input_shape))
    arg_names = net.list_arguments()
    output_names = net.list_outputs()
    aux_names = net.list_auxiliary_states()
    shape_dict = {}
    for idx, op in enumerate(arg_names):
        shape_dict[op] = shapes[0][idx]
    for idx, op in enumerate(output_names):
        shape_dict[op] = shapes[1][idx]
    for idx, op in enumerate(aux_names):
        shape_dict[op] = shapes[2][idx]

    # Get the inputs and outputs
    output_dims = shapes[1]
    if mode is None:
        output_dims = list(map(remove_batch, output_dims))
    input_types = [_datatypes.Array(*dim) for dim in input_dims]
    output_types = [_datatypes.Array(*dim) for dim in output_dims]

    # Make the builder
    input_features = list(zip(input_names, input_types))
    output_features = list(zip(output_names, output_types))
    finalize = builder is None
    if builder is None:
        builder = _neural_network.NeuralNetworkBuilder(input_features, output_features, mode)
    # Get out the layers
    net = _json.loads(net.tojson())
    nodes = net['nodes']

    for i, node in enumerate(nodes):
        node['id'] = i

        if node['name'] in shape_dict:
            node['shape'] = shape_dict[node['name']]

        node['outputs'] = []
        if 'inputs' in node:
            for ip in node['inputs']:
                nodes[ip[0]]['outputs'].append([i, 0])
        else:
            node['inputs'] = []

    # Mark the head nodes
    for head in net['heads']:
        head_id = head[0]
        head_node = nodes[head_id]
        head_node['outputs'] = [head]
        head_node['name'] += "_output"
        head_node['shape'] = shape_dict[head_node['name']]

    # For skipped layers, make sure nodes are modified
    for node in nodes:
        op = node['op']
        inputs = node['inputs']
        outputs = node['outputs']
        if op in _MXNET_SKIP_LAYERS:
            nodes[inputs[0][0]]['outputs'][0] = outputs[0]
            nodes[outputs[0][0]]['inputs'][0] = inputs[0]

    # Find the input and output names for this node
    for idx, node in enumerate(nodes):
        op = node['op']
        if op == 'null' or op in _MXNET_SKIP_LAYERS:
            continue
        name = node['name']
        if verbose:
            print("%d : %s, %s" % (idx, name, op))
        converter_func = _get_layer_converter_fn(op)
        converter_func(net, node, model, builder)

    # Only finalize builder if it was created internally. Otherwise, leave it
    # up to the user.
    if finalize:
        # Set the right inputs and outputs
        _set_input_output_layers(builder, input_names, output_names)
        builder.set_input(input_names, input_dims)
        builder.set_output(output_names, output_dims)
        if preprocessor_args is not None:
            builder.set_pre_processing_parameters(**preprocessor_args)

        if class_labels is not None:
            if type(class_labels) is str:
                labels = [l.strip() for l in open(class_labels).readlines()]
            elif type(class_labels) is list:
                labels = class_labels
            else:
                raise TypeError("synset variable of unknown type. Type found: %s. Expected either string or list of strings." % type(class_labels))
            builder.set_class_labels(class_labels = labels)

        # Return the model
        return _coremltools.models.MLModel(builder.spec)
