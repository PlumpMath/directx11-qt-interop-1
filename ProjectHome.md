# What is provided? #

The library creates a DirectX context in a QWidget similar to QGLWidget. Take a look at the <a href='http://code.google.com/p/directx11-qt-interop/source/browse/#git%2Fexample%2Fsrc%253Fstate%253Dclosed'>example application</a> for guidance.

  * <a href='http://code.google.com/p/directx11-qt-interop/source/browse/include/qdx11/BasicRenderer.h'>Basic Renderer</a>:      Context creation, frame stats, swapChain and resizement functionality.
  * <a href='http://code.google.com/p/directx11-qt-interop/source/browse/include/qdx11/Dx11Widget.h'>Dx11 Widget</a>:       Windows alternative to QGLWidget.
  * <a href='http://code.google.com/p/directx11-qt-interop/source/browse/include/qdx11/IRendererFactory.h'>IRendererFactory</a>: Interface to produce Renderer presented in Dx11 Widget.
  * <a href='http://code.google.com/p/directx11-qt-interop/source/browse/include/qdx11/PerformanceTimer.h'>Performance Timer</a>: Accurate Start/Stop Timer.

### Additional Library Dependencies ###
  * <a href='http://qt-project.org/downloads'>Qt 5.1.1</a>

  * Win8 SDK (for DirectX 11)

  * <a href='https://fx11.codeplex.com/'>Effects11 library</a>

### Build Tool ###
  * _CMake:_ http://www.cmake.org/

### System requirements ###
  * Windows 8/8.1
  * DirectX 11 capable GPU
