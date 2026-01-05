package com.honghu.wmcanvas.bridges.spec.bridge;

/**
 * Factory for creating Callback data structures.
 */

public interface IJSCallbackDataFactory {
    IJSCallbackArray createJSCallbackArray();
    IJSCallbackMap createJSCallbackMap();
}

