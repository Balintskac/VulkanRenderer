#pragma once

#include "vulkanSwapChain.g.h"

namespace winrt::renderDemo::implementation
{
    struct vulkanSwapChain : vulkanSwapChainT<vulkanSwapChain>
    {
        vulkanSwapChain() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::renderDemo::factory_implementation
{
    struct vulkanSwapChain : vulkanSwapChainT<vulkanSwapChain, implementation::vulkanSwapChain>
    {
    };
}
