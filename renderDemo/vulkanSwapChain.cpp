#include "pch.h"
#include "vulkanSwapChain.h"
#if __has_include("vulkanSwapChain.g.cpp")
#include "vulkanSwapChain.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::renderDemo::implementation
{
    int32_t vulkanSwapChain::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void vulkanSwapChain::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void vulkanSwapChain::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
