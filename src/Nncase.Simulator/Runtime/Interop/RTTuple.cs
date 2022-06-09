﻿// Copyright (c) Canaan Inc. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nncase.Runtime.Interop;

/// <summary>
/// Runtime tuple.
/// </summary>
public class RTTuple : RTValue
{
    private RTValue[]? _fields;

    internal RTTuple(IntPtr handle)
        : base(handle)
    {
    }

    /// <summary>
    /// Gets fields.
    /// </summary>
    public unsafe RTValue[] Fields
    {
        get
        {
            if (_fields == null)
            {
                uint fieldsLength = 0;
                Native.TupleGetFields(Handle, null, ref fieldsLength);
                var fields = new IntPtr[fieldsLength];
                fixed (IntPtr* fieldsPtr = fields)
                {
                    Native.TupleGetFields(Handle, fieldsPtr, ref fieldsLength).ThrowIfFailed();
                    _fields = fields.Select(value => RTValue.FromHandle(value)).ToArray();
                }
            }

            return _fields;
        }
    }
}
