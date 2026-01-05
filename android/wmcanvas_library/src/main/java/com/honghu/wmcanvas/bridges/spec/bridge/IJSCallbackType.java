package com.honghu.wmcanvas.bridges.spec.bridge;

/**
 * <p>Communications between Javascript runtime and native are container dependent. Different environment has different standards.</p>
 * <p>This enum defines supported data types.</p>
 */

public enum  IJSCallbackType {
    Null,
    Boolean,
    Number,
    String,
    Map,
    Array,
}

