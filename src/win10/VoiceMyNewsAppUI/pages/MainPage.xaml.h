#pragma once

#include "pages/MainPage.g.h"

namespace voicemynews {
namespace app {
namespace win10 {
/// <summary>
/// This is the main page of voice my news application.
/// </summary>
public ref class MainPage sealed
{
public:
    MainPage();

#ifndef _DEBUG
private:
#else
public:
#endif
    void btnTest_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};
}
}
}
