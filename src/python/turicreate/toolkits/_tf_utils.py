# -*- coding: utf-8 -*-
# Copyright © 2019 Apple Inc. All rights reserved.
#
# Use of this source code is governed by a BSD-3-clause license that can
# be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause

from __future__ import print_function as _
from __future__ import division as _
from __future__ import absolute_import as _
import numpy as np
	
def convert_shared_float_array_to_numpy(array):
	"""
	The initialization from C++ implementation is mapped to SharedFloatArray 
	in Python through Pybind. It must be converted to numpy arrays to be used
	in TensorFlow.

	Parameters
    ----------
    array: SharedFloatArray

    Returns
    -------
	return: Numpy Array
    SharedFloatArray casted as Numpy Array

	"""
	return np.array(array, copy=False, dtype=np.float32)

def convert_conv1d_coreml_to_tf(conv_weights):
	"""
	The Convolutional weights in CoreML specification converted to 
	the TensorFlow format for training in TensorFlow.

	Parameters
    ----------
    conv_weights: 4d numpy array of shape 
       [outputChannels, kernelChannels, kernelHeight, kernelWidth] 

    Returns
    -------
	return: 3d numpy array of shape
	   [kernelWidth, kernelChannels, outputChannels] 
	   since kernelHeight = 1 for conv1d

	"""

	conv_weights = np.transpose(conv_weights, (3, 1, 0, 2))
	return np.squeeze(conv_weights, axis=3)

def convert_lstm_weight_coreml_to_tf(i2h_i, i2h_c, i2h_f, i2h_o, h2h_i, h2h_c, h2h_f, h2h_o):
	"""
	The weights of four gates of LSTM - input, state, forget, output are 
	sent separately for input to hidden layer and hidden to hidden layer. 

	For tensorflow, the layers - input to hidden and hidden to hidden are 
	combined.
	Internally we need to combine the gates it in the order of input, state,
	forget and output. 

	Parameters
    ----------
    i2h_i, i2h_c, i2h_f, i2h_o, h2h_i, h2h_c, h2h_f, h2h_o: 
       2d numpy arrays of input to hidden layer 
	   and hidden to hidden layer split into their individual gates

	   i2h_gate : [hidden_units, input_depth]
	   h2h_gate : [hidden_units, hidden_units]

    Returns
    -------
	return: 2d numpy array of shape 
       [input_depth + hidden_units, hidden_units * 4] 

	"""
	i2h = np.concatenate((i2h_i, i2h_c, i2h_f, i2h_o), axis=0)
	h2h = np.concatenate((h2h_i, h2h_c, h2h_f, h2h_o), axis=0)
	lstm = np.concatenate((i2h, h2h), axis=1)
	return np.transpose(lstm)

def convert_lstm_bias_coreml_to_tf(h2h_i, h2h_c, h2h_f, h2h_o):
	"""
	The biases of four gates of LSTM - input, state, forget, output are 
	sent separately for hidden to hidden layer and are cincatenated for 
	TensorFlow in the specified order.

	Parameters
    ----------
    h2h_i, h2h_c, h2h_f, h2h_o: Separated out LSTM gates 
	    1d numpy arrays of shapes [hidden_units] 

    Returns
    -------
	return: 1d numpy array of the LSTM bias arranged 
       in the order - input, state, forget, output
       Shape [hidden_units * 4]


	"""
	lstm_bias  = np.concatenate((h2h_i, h2h_c, h2h_f, h2h_o), axis=0)
	return lstm_bias

def convert_dense_coreml_to_tf(dense_weights):
	"""
	The Dense layer weights from CoreML are [C_out, C_in, 1, 1] and need to be 
	converted to [C_in, C_out] for TensorFlow.

	Parameters
    ----------
    dense_weights: 4d numpy array of shape
	   [outputChannels, inChannels, 1, 1] 

    Returns
    -------
	return: 2d numpy array of shape 
       [outputChannels, kernelChannels, kernelHeight, kernelWidth] 

	"""
	dense_weights = np.transpose(dense_weights, (1, 0, 2, 3))
	return np.reshape(dense_weights, (dense_weights.shape[0], dense_weights.shape[1]))

def convert_conv1d_tf_to_coreml(conv_weights):
	"""
	Convolutional weights from TensorFlow in the format [kernelWidth, kernelChannels, outputChannels]
	are converted back in CoreML specifications [outputChannels, kernelChannels, kernelHeight, kernelWidth].

	Parameters
    ----------
    conv_weights: 3d numpy array of shape
	   [kernelWidth, kernelChannels, outputChannels] 
	   since kernelHeight = 1 for conv1d

    Returns
    -------
	return: 4d numpy array of shape 
       [outputChannels, kernelChannels, kernelHeight, kernelWidth] 

	"""
	conv_weights = np.expand_dims(conv_weights, axis=2)
	conv_weights = np.transpose(conv_weights, (3, 1, 2, 0))
	return conv_weights

def convert_lstm_weight_tf_to_coreml(lstm_weight, split):
	"""
	The weights of four gates of LSTM - input, state, forget, output are 
	sent separately for input to hidden layer and hidden to hidden layer
	to abide by CoreML specification.

	Parameters
    ----------
    lstm_weight: 2d numpy array of shape 
       [input_depth + hidden_units, hidden_units * 4] 

    Returns
    -------
	return: 2d numpy arrays of input to hidden layer 
	and hidden to hidden layer split into their individual gates

	i2h_gate : [hidden_units, input_depth]
	h2h_gate : [hidden_units, hidden_units]

	"""
	lstm_i2h , lstm_h2h = np.split(lstm_weight, [split])
	i2h_i, i2h_c, i2h_f, i2h_o = np.split(lstm_i2h, 4, axis=1)
	h2h_i, h2h_c, h2h_f, h2h_o = np.split(lstm_h2h, 4, axis=1)
	i2h_i = np.transpose(i2h_i)
	i2h_c = np.transpose(i2h_c)
	i2h_f = np.transpose(i2h_f)
	i2h_o = np.transpose(i2h_o)
	h2h_i = np.transpose(h2h_i)
	h2h_c = np.transpose(h2h_c)
	h2h_f = np.transpose(h2h_f)
	h2h_o = np.transpose(h2h_o)
	return i2h_i, i2h_c, i2h_f, i2h_o, h2h_i, h2h_c, h2h_f, h2h_o

def convert_lstm_bias_tf_to_coreml(lstm_bias):
	"""
	The biases of four gates of LSTM - input, state, forget, output are 
	sent separately for hidden to hidden layer to abide by CoreML specification.

	Parameters
    ----------
    lstm_bias: 1d numpy array of the LSTM bias arranged 
       in the order - input, state, forget, output
       Shape [hidden_units * 4]

    Returns
    -------
	return: Separated out LSTM gates 
	    1d numpy arrays of shapes [hidden_units]
	   

	"""
	h2h_i, h2h_c, h2h_f, h2h_o = np.split(lstm_bias, 4)
	return h2h_i, h2h_c, h2h_f, h2h_o

def convert_dense_tf_to_coreml(dense_weights):
	"""
	The Dense layer weights from TensorFlow are converted 
	back to CoreML specification.

	Parameters
    ----------
    dense_weights: 2d numpy array of shape 
       [outputChannels, kernelChannels, kernelHeight, kernelWidth] 

    Returns
    -------
	return: 4d numpy array of shape
	   [outputChannels, inChannels, 1, 1] 

	"""
	dense_weights = np.transpose(dense_weights)
	return np.reshape(dense_weights, (dense_weights.shape[0], dense_weights.shape[1], 1, 1))


