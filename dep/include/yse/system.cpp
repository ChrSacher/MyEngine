/*
==============================================================================

system.cpp
Created: 27 Jan 2014 7:14:31pm
Author:  yvan

==============================================================================
*/

#include "internalHeaders.h"



#ifdef YSE_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace YSE{
  class systemImpl {
  public:
    systemImpl() : coInitialized(false) {}
    bool coInitialized; // for windows COM library
  };

  systemImpl & SystemImpl() {
    static systemImpl s;
    return s;
  }
}

YSE::system & YSE::System() {
  static YSE::system s;
  return s;
}

Bool YSE::system::init() {
  if (INTERNAL::Global().active) {
    INTERNAL::LogImpl().emit(E_DEBUG, "You're trying to initialize more than once!");
    return true;
  }
  // global objects should always be loaded before anything else!
  INTERNAL::Global().init();
  
  // on windows, the COM library has to be initialized. This may or may not
  // also happen in your application. It doesn't matter.
#if defined YSE_WINDOWS
  if (SUCCEEDED(CoInitialize(nullptr))) {
    SystemImpl().coInitialized = true;
    INTERNAL::LogImpl().emit(E_DEBUG, "COM library initialized");
  }
#endif
  if (DEVICE::Manager().init()) {
    INTERNAL::LogImpl().emit(E_DEBUG, "YSE System object initialized");

    // initialize channels
    CHANNEL::Manager().setChannelConf(CT_STEREO);
    CHANNEL::Manager().changeChannelConf();
    CHANNEL::Manager().master().createGlobal();
    CHANNEL::Manager().ambient().create("ambientChannel", CHANNEL::Manager().master());
    CHANNEL::Manager().FX().create("fxChannel", CHANNEL::Manager().master());
    CHANNEL::Manager().music().create("musicChannel", CHANNEL::Manager().master());
    CHANNEL::Manager().gui().create("guiChannel", CHANNEL::Manager().master());
    CHANNEL::Manager().voice().create("voiceChannel", CHANNEL::Manager().master());

    maxSounds(50);
    INTERNAL::Global().active = true;
    return true;
  }
  INTERNAL::LogImpl().emit(E_ERROR, "YSE System object failed to initialize");
  return false;
}

void YSE::system::update() {
  INTERNAL::Global().flagForUpdate();
}

void YSE::system::close() {
  if (INTERNAL::Global().active) {
    INTERNAL::Global().active = false;
    DEVICE::Manager().close();
    INTERNAL::Global().close();
  }
}

YSE::system& YSE::system::occlusionCallback(Flt(*func)(const YSE::Vec&, const YSE::Vec&)) {
  occlusionPtr = func;
  return *this;
}

YSE::occlusionFunc YSE::system::occlusionCallback() {
  return occlusionPtr;
}

YSE::system::system() : occlusionPtr(nullptr) {
}

YSE::system::~system() {
#if defined YSE_WINDOWS
  if (SystemImpl().coInitialized) {
    CoUninitialize();
    SystemImpl().coInitialized = false;
  }
#endif
}

YSE::system & YSE::system::underWaterFX(const channel & target) {
  INTERNAL::UnderWaterEffect().channel(target.pimpl);
  return *this;
}

YSE::system & YSE::system::setUnderWaterDepth(Flt value) {
  INTERNAL::UnderWaterEffect().setDepth(value);
  return *this;
}

YSE::system& YSE::system::maxSounds(Int value) {
  VirtualSoundFinder().setLimit(value);
  return *this;
}

Int YSE::system::maxSounds() {
  return VirtualSoundFinder().getLimit();
}

Flt YSE::system::cpuLoad() {
  return DEVICE::Manager().cpuLoad();
}

void YSE::system::sleep(UInt ms) {
#if defined YSE_WINDOWS
  Sleep(ms);
#else
  usleep(static_cast<useconds_t>(ms)* 1000);
#endif
}

YSE::reverb & YSE::system::getGlobalReverb() {
  return REVERB::Manager().getGlobalReverb();
}

const std::vector<YSE::DEVICE::interfaceObject> & YSE::system::getDevices() {
  return DEVICE::Manager().getDeviceList();
}

void YSE::system::openDevice(const deviceSetup & object, CHANNEL_TYPE conf) {
  DEVICE::Manager().openDevice(object);
  CHANNEL::Manager().setChannelConf(conf, object.getOutputChannels());

}

void YSE::system::closeCurrentDevice() {
  DEVICE::Manager().close();
}

UInt YSE::system::getNumDevices() {
  return DEVICE::Manager().getDeviceList().size();
}

const YSE::device & YSE::system::getDevice(UInt nr) {
  return DEVICE::Manager().getDeviceList()[nr];
}

const char * YSE::system::getDefaultDevice() {
  return DEVICE::Manager().getDefaultDeviceName().c_str();
}

const char * YSE::system::getDefaultHost() {
  return DEVICE::Manager().getDefaultTypeName().c_str();
}