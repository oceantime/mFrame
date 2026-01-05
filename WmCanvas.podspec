Pod::Spec.new do |s|

  s.name         = "WmCanvas"
  s.version      = "2.0.0"
  s.summary      = "WmCanvas Source."

  s.description  = <<-DESC
                   A cross-platform fast Canvas render engine.
                   DESC

  s.homepage     = ''
  s.license = {
    :type => 'Copyright',
    :text => <<-LICENSE
    LICENSE
  }

  s.author       = { '' => '' }

  s.platform     = :ios

  s.ios.deployment_target = '9.0'

 # s.source =  { :path => '.' }
  s.source =  { :git => "", :tag => s.version }

  s.source_files = 'ios/BridgeModule/*.{h,m,mm}',
                   'core/src/export.h',
                   'core/src/WmCanvas.{hpp,cpp}',
                   'core/src/WmCanvasManager.{h,cpp}',
                   'core/src/wmcanvas/*.*',
                   'core/src/wmcanvas/GL/*.*',
                   'core/src/support/CharacterSet.{h,cpp}',
                   'core/src/support/Encode.{h,cpp}',
                   'core/src/support/FileUtils.{h,cpp}',
                   'core/src/support/Log.{h,cpp}',
                   'core/src/support/Util.{h,cpp}',
                   'core/src/support/Value.{h,cpp}',
                   'core/src/platform/iOS/*.*'
                   
  s.public_header_files =  'ios/BridgeModule/*.h', 
                           'core/src/export.h',
                           'core/src/WmCanvas.hpp',
                           'core/src/WmCanvasManager.h',
                           'core/src/wmcanvas/**/*.h',
                           'core/src/support/CharacterSet.h',
                           'core/src/support/Encode.h',
                           'core/src/support/FileUtils.h',
                           'core/src/support/Log.h',
                           'core/src/support/Util.h',
                           'core/src/support/Value.h',
                           'core/src/platform/iOS/*.h'


  s.user_target_xcconfig  = { 'FRAMEWORK_SEARCH_PATHS' => "'$(PODS_ROOT)/WmCanvas'",  
                              'HEADER_SEARCH_PATHS' => "'$(PODS_ROOT)/WmCanvas'"}

  s.requires_arc = true

  s.frameworks = 'Foundation','UIKit','GLKit', 'CoreMedia', 'AVFoundation', 'OpenGLES', 'CoreText'

  s.library = 'c++'

  s.pod_target_xcconfig = { 'OTHER_CFLAGS'  => '-DIOS -DWMCANVAS', 'GCC_PREPROCESSOR_DEFINITIONS' => 'IOS' }

end
