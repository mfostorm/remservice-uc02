// RemService.cpp — точка входа приложения
// Информационная система сервисного центра по ремонту техники «РемСервис»
// Учебная практика УП.02, вариант 20

#include "LoginForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(cli::array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew RemService::LoginForm());
    return 0;
}
