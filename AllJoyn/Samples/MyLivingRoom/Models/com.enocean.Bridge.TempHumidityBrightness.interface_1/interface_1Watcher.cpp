//-----------------------------------------------------------------------------
// <auto-generated> 
//   This code was generated by a tool. 
// 
//   Changes to this file may cause incorrect behavior and will be lost if  
//   the code is regenerated.
//
//   Tool: AllJoynCodeGenerator.exe
//
//   This tool is located in the Windows 10 SDK and the Windows 10 AllJoyn 
//   Visual Studio Extension in the Visual Studio Gallery.  
//
//   The generated code should be packaged in a Windows 10 C++/CX Runtime  
//   Component which can be consumed in any UWP-supported language using 
//   APIs that are available in Windows.Devices.AllJoyn.
//
//   Using AllJoynCodeGenerator - Invoke the following command with a valid 
//   Introspection XML file and a writable output directory:
//     AllJoynCodeGenerator -i <INPUT XML FILE> -o <OUTPUT DIRECTORY>
// </auto-generated>
//-----------------------------------------------------------------------------
#include "pch.h"

using namespace concurrency;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Devices::AllJoyn;
using namespace com::enocean::Bridge::TempHumidityBrightness::interface_1;

interface_1Watcher::interface_1Watcher(AllJoynBusAttachment^ busAttachment) :
    m_aboutListener(nullptr)
{
    m_busAttachment = busAttachment;
    m_weak = new WeakReference(this);
    m_busAttachmentStateChangedToken.Value = 0;
}

interface_1Watcher::~interface_1Watcher()
{
    UnregisterFromBus();
}

void interface_1Watcher::UnregisterFromBus()
{
    if (nullptr != m_aboutListener)
    {
        PCSTR interfaces[] = { "com.enocean.Bridge.TempHumidityBrightness.interface_1" };
        alljoyn_busattachment_cancelwhoimplements_interfaces(
            AllJoynHelpers::GetInternalBusAttachment(m_busAttachment),
            interfaces,
            _countof(interfaces));

        alljoyn_busattachment_unregisteraboutlistener(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), m_aboutListener);
        alljoyn_aboutlistener_destroy(m_aboutListener);
        m_aboutListener = nullptr;
    }
    if ((nullptr != m_busAttachment) && (0 != m_busAttachmentStateChangedToken.Value))
    {
        m_busAttachment->StateChanged -= m_busAttachmentStateChangedToken;
    }
}

void interface_1Watcher::OnAnnounce(
    _In_ PCSTR name,
    _In_ uint16_t version,
    _In_ alljoyn_sessionport port,
    _In_ alljoyn_msgarg objectDescriptionArg,
    _In_ const alljoyn_msgarg aboutDataArg)
{
    UNREFERENCED_PARAMETER(version);
    UNREFERENCED_PARAMETER(aboutDataArg);

    alljoyn_aboutobjectdescription objectDescription = alljoyn_aboutobjectdescription_create_full(objectDescriptionArg);

    if (alljoyn_aboutobjectdescription_hasinterface(objectDescription, "com.enocean.Bridge.TempHumidityBrightness.interface_1"))
    {
        AllJoynServiceInfo^ args = ref new AllJoynServiceInfo(
            AllJoynHelpers::MultibyteToPlatformString(name),
            AllJoynHelpers::GetObjectPath(objectDescription, "com.enocean.Bridge.TempHumidityBrightness.interface_1"),
            port);
        Added(this, args);
    }
    alljoyn_aboutobjectdescription_destroy(objectDescription);
}

void interface_1Watcher::BusAttachmentStateChanged(_In_ AllJoynBusAttachment^ sender, _In_ AllJoynBusAttachmentStateChangedEventArgs^ args)
{
    if (args->State == AllJoynBusAttachmentState::Connected)
    {
        alljoyn_aboutlistener_callback callbacks = 
        {
            AllJoynHelpers::AnnounceHandler<interface_1Watcher>
        };
        m_aboutListener = alljoyn_aboutlistener_create(&callbacks, m_weak);

        alljoyn_busattachment_registeraboutlistener(AllJoynHelpers::GetInternalBusAttachment(sender), m_aboutListener);
        PCSTR interfaces[] = { "com.enocean.Bridge.TempHumidityBrightness.interface_1" };
        
        auto status = alljoyn_busattachment_whoimplements_interfaces(
            AllJoynHelpers::GetInternalBusAttachment(sender), 
            interfaces,
            _countof(interfaces));
        if (ER_OK != status)
        {
            StopInternal(status);
        }
    }
    else if (args->State == AllJoynBusAttachmentState::Disconnected)
    {
        StopInternal(ER_BUS_STOPPING);
    }
}

void interface_1Watcher::Start()
{
    if (nullptr == m_busAttachment)
    {
        StopInternal(ER_FAIL);
        return;
    }

    int32 result = AllJoynHelpers::CreateInterfaces(m_busAttachment, c_interface_1IntrospectionXml);
    if (result != AllJoynStatus::Ok)
    {
        StopInternal(result);
        return;
    }

    m_busAttachmentStateChangedToken = m_busAttachment->StateChanged += ref new TypedEventHandler<AllJoynBusAttachment^, AllJoynBusAttachmentStateChangedEventArgs^>(this, &interface_1Watcher::BusAttachmentStateChanged);
    m_busAttachment->Connect();
}

void interface_1Watcher::Stop()
{
    StopInternal(AllJoynStatus::Ok);
}

void interface_1Watcher::StopInternal(int32 status)
{
    UnregisterFromBus();
    Stopped(this, ref new AllJoynProducerStoppedEventArgs(status));
}